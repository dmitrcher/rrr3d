#include "OriginalGameCar.h"
#include <fstream>
#include <cstring>
#include <cmath>
namespace source=r3d::game::originalrace::source;
static unsigned word(float f){unsigned u;std::memcpy(&u,&f,4);return u;}
int main(int argc,char**argv){if(argc!=2)return 2;std::ofstream out(argv[1]);const float torques[]={2000.0F,17.3F,600.0F,3500.0F,1000.0F,1700.0F};
for(float maximum:torques)for(int gear=0;gear<6;++gear){source::GameCar car;source::GameCar::MotorDescription motor;motor.maximumTorque=maximum;motor.originalMotorSem=.7F;motor.hasOriginalTorqueFactors=true;motor.torqueEfficiency=motor.originalMotorSem*1.15F;motor.differentialRatio=3.42F;motor.automaticGears=false;car.ConfigureMotor(motor);car.SetCurGear(gear);source::GameCar::FixedStepInput input;input.manualSteering=true;if(gear==0)input.reverse=1;else input.throttle=1;const auto result=car.OnFixedStepDrive(1.0F/60,input,{});if(result.gear!=gear)return 3;out<<"{\"event\":\"motor.sample\",\"maximum_torque_word\":"<<word(maximum)<<",\"sem_word\":"<<word(motor.originalMotorSem)<<",\"differential_word\":"<<word(motor.differentialRatio)<<",\"gear\":"<<gear<<",\"result_word\":"<<word(std::abs(result.motorTorque))<<"}\n";}
return out.good()?0:4;}
