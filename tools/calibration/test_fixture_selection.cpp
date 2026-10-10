#include "Fixture.h"
#include <cstdio>
static int executions=0;
static void execute(const motorrock::calibration::Case&) { ++executions; }
int main(int argc,char**argv) {
    if(argc!=2)return 2;
    const char* invalid[]={"list:force,not-a-case","list:force,force","list:force,,impulse","list:","list:force,"};
    for(const char* selection:invalid){
        {std::ofstream f(argv[1]);f<<"preserve existing capture";}
        char* args[]={argv[0],argv[1],const_cast<char*>(selection)};
        bool rejected=false;
        try{motorrock::calibration::run(3,args,execute,"selection-test");}
        catch(const std::runtime_error&){rejected=true;}
        std::ifstream f(argv[1]);std::string contents;std::getline(f,contents);
        if(!rejected||executions!=0||contents!="preserve existing capture")return 3;
    }
    char* args[]={argv[0],argv[1],const_cast<char*>("list:force,impulse")};
    if(motorrock::calibration::run(3,args,execute,"selection-test")!=0||executions!=2)return 4;
    std::remove(argv[1]);return 0;
}
