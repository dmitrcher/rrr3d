#include "OriginalGameCar.h"
#include <fstream>
#include <string>
#include <cstring>
namespace source=r3d::game::originalrace::source;
static float value(unsigned u){float f;std::memcpy(&f,&u,4);return f;}static unsigned word(float f){unsigned u;std::memcpy(&u,&f,4);return u;}
int main(int argc,char**argv){if(argc!=3)return 2;std::ifstream input(argv[1]);std::string magic;unsigned count;input>>magic>>count;if(magic!="MOTORROCK_RPM_V174")return 2;std::ofstream out(argv[2]);for(unsigned i=0;i<count;++i){int maxRpm,idle,gear;unsigned diff,axle;input>>maxRpm>>idle>>diff>>gear>>axle;if(!input)return 3;source::GameCar car;source::GameCar::MotorDescription motor;motor.maximumRpm=maxRpm;motor.idlingRpm=idle;motor.differentialRatio=value(diff);motor.automaticGears=false;car.ConfigureMotor(motor);car.SetCurGear(gear);source::GameCar::FixedStepState state;state.drivenWheelAngularSpeed=value(axle);const auto result=car.OnFixedStepDrive(1.0F/60,{},state);if(result.gear!=gear)return 4;out<<"{\"event\":\"rpm.sample\",\"case\":"<<i<<",\"result_word\":"<<word(result.rpm)<<"}\n";}return out.good()?0:4;}
