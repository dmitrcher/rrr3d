#include "OriginalGameCar.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>
struct NxVec3 {float x,y,z;NxVec3(float a=0,float b=0,float c=0):x(a),y(b),z(c){}float magnitude()const{return std::sqrt(x*x+y*y+z*z);}};
static NxVec3 operator*(float s,NxVec3 v){return {s*v.x,s*v.y,s*v.z};}
namespace px {struct Scene{static const NxVec3 cDefGravity;};const NxVec3 Scene::cDefGravity(0,0,-20);}
constexpr int NX_ACCELERATION=5;
struct NxActor {NxVec3 velocity,localAcceleration;bool force=false;void addForce(NxVec3 v,int mode){if(mode!=5||v.z!=-20)throw std::runtime_error("force contract");force=true;}NxVec3 getLinearVelocity(){return velocity;}void addLocalTorque(NxVec3 v,int mode){if(mode!=5)throw std::runtime_error("torque mode");localAcceleration=v;}};
struct PxActor {NxActor*actor;NxActor*GetNxActor(){return actor;}};
struct GameCar {float _springTime=0,_flyYTorque=0;bool _anyWheelContact=false;NxActor actor;PxActor GetPxActor(){return {&actor};}void JumpProgress(float deltaTime);};

void GameCar::JumpProgress(float deltaTime)
{
	NxActor* nxActor = GetPxActor().GetNxActor();

	_springTime = std::max(_springTime - deltaTime, 0.0f);

	if (!_anyWheelContact)
	{
		//если нет контакта необходжимо машину замедлять и быстро приземлять, чтобы она не улетала на прыжках в космос
		//nxActor->setLinearDamping(0.4f);
		nxActor->addForce(1.0f * px::Scene::cDefGravity, NX_ACCELERATION);

		//наклоняем машину вперед если она движется в OXY
		NxVec3 vel = nxActor->getLinearVelocity();
		vel.z = 0.0f;
		if (vel.magnitude() > 1.0f && _flyYTorque != 0 && _springTime == 0.0f)
			nxActor->addLocalTorque(NxVec3(0, _flyYTorque, 0), NX_ACCELERATION);
	}
}


static unsigned word(float v){unsigned u;std::memcpy(&u,&v,4);return u;}
int main(){namespace G=r3d::game::originalrace::source;unsigned c=0;
for(bool contact:{false,true})for(float speed:{0.F,std::nextafter(1.F,0.F),1.F,std::nextafter(1.F,2.F),5.F})for(float torque:{0.F,4.F,-4.F})for(bool spring:{false,true})for(float dt:{0.F,1.F/60.F,std::nextafter(1.5F,0.F),1.5F,std::nextafter(1.5F,2.F),2.F}){
GameCar original;original._anyWheelContact=contact;original._flyYTorque=torque;original._springTime=spring?1.5F:0.F;original.actor.velocity={speed,0,0};original.JumpProgress(dt);
G::GameCar source;G::GameCar::DynamicsDescription d;d.airbornePitchAcceleration=torque;source.ConfigureDynamics(d,{});if(spring)source.LockSpring();G::GameCar::FixedStepState st;st.anyWheelContact=contact;st.horizontalSpeed=speed;const auto command=source.OnFixedStepDrive(dt,{},st);
if(command.applyExtraGravity!=original.actor.force || word(command.airbornePitchAcceleration)!=word(original.actor.localAcceleration.y) || word(source.GetSpringTime())!=word(original._springTime)){std::cerr<<"source gate differs at "<<c<<'\n';return 1;}
std::cout<<"{\"case\":"<<c++<<",\"contact\":"<<(contact?"true":"false")<<",\"speed_word\":"<<word(speed)<<",\"torque_word\":"<<word(torque)<<",\"dt_word\":"<<word(dt)<<",\"spring_before\":"<<(spring?"true":"false")<<",\"spring_after_word\":"<<word(source.GetSpringTime())<<",\"force\":"<<(command.applyExtraGravity?"true":"false")<<",\"pitch_word\":"<<word(command.airbornePitchAcceleration)<<"}\n";
}return c==360?0:2;}
