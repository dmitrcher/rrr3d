#include "OriginalPhysicsReplay.h"
#include "SourceWorldFixtureV124.h"
#include "OriginalRaceSession.h"
#include "ParityTrace.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>

namespace rrr3d::diagnostics
{
namespace
{
using namespace r3d::physics;
constexpr float dt = 1.0F / 60.0F;
constexpr int frameCount = 900;

VehicleInput inputForTick(int tick)
{
    VehicleInput input;
    input.manualSteering = true;
    if (tick >= 120 && tick < 300) input.throttle = 1.0F;
    if (tick >= 360 && tick < 420) input.brake = 1.0F;
    if (tick >= 420 && tick < 480) input.reverse = 1.0F;
    if (tick >= 480 && tick < 600)
    {
        input.throttle = 0.5F;
        input.steering = tick < 540 ? -0.5F : 0.5F;
    }
    if (tick >= 600 && tick < 660) input.brake = 1.0F;
    return input;
}

void vector(std::ostream& out, Vec3 v)
{
    out << '[' << v.x << ',' << v.y << ',' << v.z << ']';
}

bool finite(Vec3 v)
{
    return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
}
}

bool replaySoloPhysics(const r3d::game::originalrace::Race& race,
                       const r3d::physics::WorldDescription& description)
{
    using namespace r3d::game::originalrace;
    try
    {
        if (!motorrock::parity::enabled())
            throw std::runtime_error("RRR3D_TRACE_FILE must select a replay log");
        {
            std::ofstream trace(std::getenv("RRR3D_TRACE_FILE"), std::ios::app);
            if (!trace) throw std::runtime_error("cannot write the solo replay log");
        }
        if (race.racers.size() != 1 || !race.racers.front().human ||
            description.spawns.size() != 1)
            throw std::runtime_error("solo replay requires exactly one human spawn");
        const char* fixturePathV124=std::getenv("RRR3D_WORLD_FIXTURE_FILE_V124");
        if(fixturePathV124 && *fixturePathV124)exportSourceWorldV124(description,fixturePathV124);
        std::string error;
        auto world = createOriginalVehicleWorld(description, error);
        if (!world) throw std::runtime_error(error);
        if (world->vehicleCount() != 1)
            throw std::runtime_error("solo replay created an opponent");
        OriginalRaceSession session(race);
        session.setCampaign(false);
        session.setExternalVehicleFixedStep(true);
        session.reset();
        session.setDebugHumanAiControl(false);
        if (session.racers().size() != 1)
            throw std::runtime_error("solo session contains an opponent");
        const auto initialResetCount = world->vehicle().resetCount;

        // Deliberately do not call update()/raceFixedStep(): these own AI,
        // bonuses, damage and route-based respawns. Commands go directly to
        // the same source GameCar callback used by the interactive game.
        const char* actuatorsFlagV124=std::getenv("RRR3D_REAL_WORLD_ACTUATORS_V124");
        const bool actuatorsV124=actuatorsFlagV124 && std::string(actuatorsFlagV124)=="1";
        int tick = 0;
        unsigned substep = 0;
        world->setVehicleFixedStepController(
            [&](std::size_t racer, float seconds, const VehicleInput& input,
                const VehicleFixedStepState& state) {
                VehicleDriveCommand command;
                if(actuatorsV124) {
                    // Fixed raw actuators isolate the real-map backend from
                    // the GameCar driver arithmetic under separate audit.
                    command.engineRpm=1000;command.gear=1;
                    if(tick>=120 && tick<300)command.motorTorque=1000;
                    if((tick>=360 && tick<420)||(tick>=600 && tick<660))command.brakeTorque=6000;
                    if(tick>=420 && tick<480)command.motorTorque=-800;
                    if(tick>=480 && tick<600){command.motorTorque=500;command.steeringAngle=tick<540?-.2F:.2F;}
                } else command=session.racerFixedStepDrive(racer,seconds,input,state);
                std::ostringstream row;
                row.imbue(std::locale::classic());
                row << std::setprecision(9) << "{\"tick\":" << tick
                    << ",\"substep\":" << substep++
                    << ",\"dt\":" << seconds
                    << ",\"motor_torque\":" << command.motorTorque
                    << ",\"brake_torque\":" << command.brakeTorque
                    << ",\"steering_angle\":" << command.steeringAngle
                    << ",\"gear\":" << command.gear
                    << ",\"rpm\":" << command.engineRpm << '}';
                motorrock::parity::event("solo.drive", race.levelPath, row.str());
                return command;
            });
        std::ostringstream begin;
        begin.imbue(std::locale::classic());
        begin << std::setprecision(9)
              << "{\"protocol\":\"solo-gamecar-v1\",\"ticks\":" << frameCount
              << ",\"dt\":" << dt
              << ",\"vehicles\":1,\"opponents\":0,\"human_ai\":false"
                 ",\"initial_reset_count\":" << initialResetCount
              <<
                 ",\"gameplay_updates\":false,\"rendered\":false,\"seed\":"
              << motorrock::parity::seedFromEnvironment(42)
              << ",\"car\":" << motorrock::parity::quote(race.vehicle.record)
              << ",\"collision_meshes\":" << description.collisionMeshes.size()
              << ",\"decorations\":" << description.decorations.size() << '}';
        if(actuatorsV124)motorrock::parity::event("solo.actuator-mode",race.levelPath,"{\"protocol\":\"raw-actuator-v124\",\"gamecar_driver_used\":false}");
        motorrock::parity::event("solo.begin", race.levelPath, begin.str());

        for (; tick < frameCount; ++tick)
        {
            substep = 0;
            const auto input = inputForTick(tick);
            Vec3 impulse;
            if (tick == 720) impulse.y = 2.0F;
            if (tick == 780) impulse.z = 2.0F;
            world->addLinearVelocity(0, impulse);
            world->step(dt, input);
            const auto& state = world->vehicle();
            if (state.resetCount != initialResetCount)
                throw std::runtime_error("unexpected solo vehicle reset");
            const auto& q = state.body.rotation;
            if (!finite(state.body.position) || !finite(state.linearVelocity) ||
                !finite(state.angularMomentum) || !std::isfinite(q.x) ||
                !std::isfinite(q.y) || !std::isfinite(q.z) || !std::isfinite(q.w))
                throw std::runtime_error("non-finite solo vehicle state");
            session.synchronizeRacerPhysicsState(0, state);
            std::ostringstream row;
            row.imbue(std::locale::classic());
            row << std::setprecision(9) << "{\"tick\":" << tick
                << ",\"time_seconds\":" << (tick + 1) / 60.0
                << ",\"input\":{\"throttle\":" << input.throttle
                << ",\"reverse\":" << input.reverse
                << ",\"brake\":" << input.brake
                << ",\"steering\":" << input.steering
                << ",\"manual_steering\":true},\"delta_velocity\":";
            vector(row, impulse);
            row << ",\"position\":"; vector(row, state.body.position);
            row << ",\"velocity\":"; vector(row, state.linearVelocity);
            row << ",\"angular_momentum\":"; vector(row, state.angularMomentum);
            row << ",\"rotation\":[" << q.x << ',' << q.y << ',' << q.z << ',' << q.w
                << "],\"body_awake\":" << (state.bodyAwake ? "true" : "false")
                << ",\"resets\":" << state.resetCount << ",\"wheels\":[";
            for (std::size_t i = 0; i < state.wheelContacts.size(); ++i)
            {
                if (i) row << ',';
                const auto& w = state.wheelContacts[i];
                row << "{\"contact\":" << (w.hasContact ? "true" : "false")
                    << ",\"suspension\":" << w.suspensionLength
                    << ",\"suspension_force\":" << w.suspensionForce
                    << ",\"longitudinal_slip\":" << w.longitudinalSlip
                    << ",\"lateral_slip\":" << w.lateralSlip
                    << ",\"longitudinal_impulse\":" << w.longitudinalImpulse
                    << ",\"lateral_impulse\":" << w.lateralImpulse
                    << ",\"angular_speed\":" << state.wheelAngularSpeeds.at(i) << '}';
            }
            row << "]}";
            motorrock::parity::event("solo.sample", race.levelPath, row.str());
        }
        motorrock::parity::event("solo.end", race.levelPath, "{\"ticks\":900,\"completed\":true}");
        std::cout << "Solo physics replay complete: " << race.levelPath
                  << ", 900 ticks, one vehicle, no AI; GameCar and map physics only\n";
        return true;
    }
    catch (const std::exception& error)
    {
        motorrock::parity::event("solo.error", error.what());
        std::cerr << "Solo physics replay failed: " << error.what() << '\n';
        return false;
    }
}
}
