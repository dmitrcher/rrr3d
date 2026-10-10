#include "OriginalJungleAudit.h"
#include "OriginalEnvironment.h"
#include "OriginalRace.h"
#include "OriginalRaceSession.h"
#include "physics/OriginalVehiclePhysics.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <vector>

namespace rrr3d::diagnostics
{
bool auditJungleStarts(const r3d::resource::ResourceFileSystem& resources)
{
    using namespace r3d::game::originalrace;
    using namespace r3d::physics;
    try
    {
        const auto catalog = loadOriginalRace(resources, 0);
        std::size_t maps = 0, tested = 0, failed = 0;
        for (std::size_t track = 0; track < catalog.trackCatalog.size(); ++track)
        {
            if (catalog.trackCatalog[track].worldType != "wtWorld5")
                continue;
            const auto race = loadOriginalRace(resources, track);
            const auto description = makePhysicsDescription(race, resources);
            std::string error;
            auto world = createOriginalVehicleWorld(description, error);
            if (!world)
                throw std::runtime_error(error);
            std::vector<float> roadHeights;
            for (const auto& spawn : description.spawns)
            {
                WorldRayCastQuery query;
                query.origin = spawn.position;
                query.direction = {0, 0, -1};
                query.maximumDistance = 1000;
                query.trackPlaneOnly = true;
                const auto hit = world->raycast(query);
                roadHeights.push_back(hit.hit ? hit.position.z : -1000);
            }
            // This is an initial-placement/collision audit, not an AI race:
            // no invented waypoint driver, acceleration or cheat immunity.
            for (int step = 0; step < 120; ++step)
                world->step(1.0F / 60.0F, VehicleInput{});
            std::cout << race.levelPath << " start";
            for (std::size_t racer = 0; racer < world->vehicleCount(); ++racer)
            {
                const auto& state = world->vehicle(racer);
                const auto& spawn = description.spawns[racer];
                const auto& pos = state.body.position;
                const float offset = std::hypot(pos.x - spawn.position.x, pos.y - spawn.position.y);
                const bool valid = roadHeights[racer] > 0 && std::isfinite(pos.z) &&
                    pos.z > roadHeights[racer] && pos.z < spawn.position.z + 1 &&
                    offset < 3 && state.contactCount > 0;
                ++tested;
                if (!valid) ++failed;
                std::cout << ' ' << racer << ':' << (valid ? "OK" : "FAIL")
                          << "(z=" << pos.z << ",road=" << roadHeights[racer]
                          << ",offset=" << offset << ",wheels=" << state.contactCount << ')';
            }
            ++maps;
            std::cout << std::endl;
        }
        std::cout << "Jungle starts: maps=" << maps << " cars=" << tested
                  << " failures=" << failed << std::endl;
        return maps == 16 && tested > 0 && failed == 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Jungle start audit: " << error.what() << std::endl;
        return false;
    }
}

bool auditAllMapAi(
    const r3d::resource::ResourceFileSystem& resources,
    std::optional<std::size_t> selectedTrack)
{
    using namespace r3d::game::originalrace;
    using namespace r3d::physics;
    constexpr float stepSeconds = 1.0F / 60.0F;
    constexpr int auditFrames = 900;
    constexpr float maximumMinimumRouteProgress = 0.15F;
    constexpr float pileRadius = 5.0F;
    constexpr int maximumPileFrames = 300;
    constexpr int maximumTotalPileFrames = 360;
    try
    {
        const auto catalog = loadOriginalRace(resources, 0);
        std::size_t failedMaps = 0U;
        const std::size_t firstTrack = selectedTrack.value_or(0U);
        const std::size_t endTrack = selectedTrack.has_value()
            ? std::min(firstTrack + 1U, catalog.trackCatalog.size())
            : catalog.trackCatalog.size();
        if (firstTrack >= endTrack)
            throw std::runtime_error("selected track is out of range");
        for (std::size_t track = firstTrack; track < endTrack; ++track)
        {
            auto race = loadOriginalRace(resources, track);
            const auto& auditPath = !race.tracePaths.empty()
                ? race.tracePaths.front()
                : race.tracePath;
            float auditPathLength = 0.0F;
            for (std::size_t node = 1U; node < auditPath.size(); ++node)
            {
                const auto findPoint = [&](std::uint32_t id) {
                    return std::find_if(
                        race.tracePoints.begin(), race.tracePoints.end(),
                        [id](const auto& point) {
                            return point.id == id;
                        });
                };
                const auto first = findPoint(auditPath[node - 1U]);
                const auto second = findPoint(auditPath[node]);
                if (first == race.tracePoints.end() ||
                    second == race.tracePoints.end())
                    continue;
                auditPathLength += std::hypot(
                    second->position.x - first->position.x,
                    second->position.y - first->position.y);
            }
            const float minimumRouteProgress = std::min(
                maximumMinimumRouteProgress,
                20.0F / std::max(auditPathLength, 1.0F));
            // Navigation is what this audit measures. Weapons and touch
            // damage can destroy an otherwise correctly routed car and make
            // a route failure depend on random combat instead of map data.
            for (auto& racer : race.racers)
                racer.loadout.clear();
            race.touchBorderDamage = {};
            race.touchCarDamage = {};

            const auto description = makePhysicsDescription(race, resources);
            std::string error;
            auto world = createOriginalVehicleWorld(description, error);
            if (!world)
                throw std::runtime_error(error);

            bool startsValid =
                description.spawns.size() == world->vehicleCount();
            for (std::size_t spawnIndex = 0U;
                 spawnIndex < description.spawns.size(); ++spawnIndex)
            {
                const auto& spawn = description.spawns[spawnIndex];
                WorldRayCastQuery query;
                query.origin = spawn.position;
                query.origin.z += 4.0F;
                query.direction = {0.0F, 0.0F, -1.0F};
                query.maximumDistance = 24.0F;
                query.trackPlaneOnly = true;
                const auto hit = world->raycast(query);
                const float directionLength = std::hypot(
                    spawn.direction.x, spawn.direction.y);
                startsValid = startsValid && hit.hit &&
                    std::isfinite(spawn.position.x) &&
                    std::isfinite(spawn.position.y) &&
                    std::isfinite(spawn.position.z) &&
                    directionLength > 0.5F;
                if (!hit.hit)
                {
                    std::cout << "All-map AI invalid start " << track
                              << ':' << spawnIndex << " pos="
                              << spawn.position.x << ','
                              << spawn.position.y << ','
                              << spawn.position.z << '\n';
                }
            }

            source::WorldEventPump eventPump;
            source::Environment environment;
            eventPump.SetEnvironment(&environment);
            source::GameModeState gameMode;
            eventPump.SetGameMode(&gameMode);
            OriginalRaceSession session(race, false, &eventPump);
            session.setCampaign(true);
            session.setDebugHumanAiControl(true);
            session.setExternalRaceFixedStep(true);
            session.setExternalProjectilePhysics(true);
            session.setExternalVehicleFixedStep(true);
            session.setExternalRaceLateProgress(true);
            session.setWorldRaycast(
                [&world](const WorldRayCastQuery& query) {
                    return world->raycast(query);
                });

            std::vector<std::size_t> resetCounts(
                world->vehicleCount(), 0U);
            bool respawnsValid = true;
            const auto validateRespawn =
                [&world, &respawnsValid](
                    std::size_t racer, Vec3 position, Vec3 direction) {
                    WorldRayCastQuery query;
                    query.origin = position;
                    query.origin.z += 4.0F;
                    query.direction = {0.0F, 0.0F, -1.0F};
                    query.maximumDistance = 24.0F;
                    query.trackPlaneOnly = true;
                    const auto hit = world->raycast(query);
                    respawnsValid = respawnsValid &&
                        racer < world->vehicleCount() && hit.hit &&
                        std::isfinite(position.x) &&
                        std::isfinite(position.y) &&
                        std::isfinite(position.z) &&
                        std::hypot(direction.x, direction.y) > 0.5F;
                };
            world->setWorldFixedStepController(
                [&session, &resetCounts, &validateRespawn](
                    float seconds,
                    const std::vector<VehicleState>& vehicles,
                    std::vector<VehicleInput>& inputs,
                    std::vector<VehicleResetCommand>& resets,
                    std::vector<VehicleLinearVelocityCommand>& velocities,
                    const std::vector<ProjectileBodyState>&,
                    std::vector<ProjectileBodyCommand>& projectileCommands) {
                    session.raceFixedStep(
                        seconds, vehicles, inputs, resets);
                    for (const auto& reset : resets)
                    {
                        validateRespawn(
                            reset.vehicle, reset.position, reset.direction);
                        if (reset.vehicle < resetCounts.size())
                            ++resetCounts[reset.vehicle];
                    }
                    for (const auto& request :
                         session.takeVelocityRequests())
                    {
                        velocities.push_back(
                            {request.racer, request.delta});
                    }
                    auto commands = session.takeProjectileBodyCommands();
                    projectileCommands.insert(
                        projectileCommands.end(),
                        std::make_move_iterator(commands.begin()),
                        std::make_move_iterator(commands.end()));
                });
            world->setVehicleFixedStepController(
                [&session](std::size_t racer, float seconds,
                           const VehicleInput& input,
                           const VehicleFixedStepState& state) {
                    return session.racerFixedStepDrive(
                        racer, seconds, input, state);
                });
            // The runtime binds native fixed-step ownership before
            // StartRace resets its source Player/GameCar graph. Do the same
            // here so the audit does not retain the constructor's headless
            // fixed-step state.
            session.reset();
            session.setCampaign(true);
            session.setDebugHumanAiControl(true);

            std::vector<VehicleState> vehicles(world->vehicleCount());
            std::vector<float> maximumProgress(
                world->vehicleCount(), 0.0F);
            int pileFrames = 0;
            int longestPile = 0;
            int totalPileFrames = 0;
            int racingFrames = 0;
            for (int frame = 0; frame < auditFrames; ++frame)
            {
                for (std::size_t racer = 0U;
                     racer < world->vehicleCount(); ++racer)
                    vehicles[racer] = world->vehicle(racer);

                session.update(stepSeconds, vehicles, RaceControl{});
                for (const auto& respawn : session.takeRespawns())
                {
                    validateRespawn(
                        respawn.racer, respawn.position,
                        respawn.direction);
                    if (respawn.racer < resetCounts.size())
                        ++resetCounts[respawn.racer];
                    world->resetVehicle(
                        respawn.racer, respawn.position,
                        respawn.direction);
                }
                for (const auto& request :
                     session.takeAngularVelocityRequests())
                    world->addAngularVelocity(
                        request.racer, request.delta);
                for (const auto& request :
                     session.takeAngularMomentumRequests())
                    world->setAngularMomentum(
                        request.racer, request.momentum);
                auto inputs = session.vehicleInputs();
                world->applyProjectileBodyCommands(
                    session.takeProjectileBodyCommands());
                world->step(stepSeconds, inputs);

                for (std::size_t racer = 0U;
                     racer < world->vehicleCount(); ++racer)
                {
                    vehicles[racer] = world->vehicle(racer);
                    session.synchronizeRacerPhysicsState(
                        racer, vehicles[racer]);
                    if (racer < session.racers().size())
                    {
                        maximumProgress[racer] = std::max(
                            maximumProgress[racer],
                            session.racers()[racer].car.GetLap());
                    }
                }
                session.lateProgress(stepSeconds, vehicles);

                std::size_t largestPile = 0U;
                if (session.phase() == RacePhase::Racing)
                {
                    ++racingFrames;
                    // Ignore the launch itself: four cars legitimately fit
                    // inside this radius on the two-row starting grid.
                    if (racingFrames <= 120)
                        continue;
                    for (std::size_t first = 0U;
                         first < vehicles.size(); ++first)
                    {
                        std::size_t nearby = 0U;
                        for (const auto& candidate : vehicles)
                        {
                            const float dx = candidate.body.position.x -
                                vehicles[first].body.position.x;
                            const float dy = candidate.body.position.y -
                                vehicles[first].body.position.y;
                            if (std::hypot(dx, dy) <= pileRadius &&
                                std::abs(candidate.speed) < 3.0F)
                                ++nearby;
                        }
                        largestPile = std::max(largestPile, nearby);
                    }
                }
                if (largestPile >= 4U)
                {
                    ++pileFrames;
                    ++totalPileFrames;
                }
                else
                    pileFrames = 0;
                longestPile = std::max(longestPile, pileFrames);
            }

            // The first and second grid rows can initially report different
            // normalized positions on short closing tiles. Requiring two
            // independent racers to advance catches total route loss while
            // the spatial pile detector handles partial-pack convergence.
            const std::size_t requiredProgressing =
                std::min<std::size_t>(2U, maximumProgress.size());
            std::size_t progressing = 0U;
            for (std::size_t racer = 0U;
                 racer < maximumProgress.size(); ++racer)
            {
                if (maximumProgress[racer] >= minimumRouteProgress)
                    ++progressing;
            }
            const auto maximumResets = resetCounts.empty()
                ? 0U
                : *std::max_element(
                      resetCounts.begin(), resetCounts.end());
            const bool passed = startsValid && respawnsValid &&
                progressing >= requiredProgressing &&
                longestPile <= maximumPileFrames &&
                totalPileFrames <= maximumTotalPileFrames &&
                maximumResets <= 3U;
            if (!passed)
                ++failedMaps;
            std::cout << "All-map AI " << (passed ? "OK " : "FAIL ")
                      << track << ' ' << race.levelPath
                      << " starts=" << startsValid
                      << " respawns=" << respawnsValid
                      << " progressing=" << progressing << '/'
                      << maximumProgress.size()
                      << " pileFrames=" << longestPile
                      << '/' << totalPileFrames
                      << " maxResets=" << maximumResets << '\n';
        }
        std::cout << "All-map AI audit: maps="
                  << (endTrack - firstTrack)
                  << " failures=" << failedMaps << std::endl;
        return !catalog.trackCatalog.empty() && failedMaps == 0U;
    }
    catch (const std::exception& error)
    {
        std::cerr << "All-map AI audit: " << error.what() << std::endl;
        return false;
    }
}
}
