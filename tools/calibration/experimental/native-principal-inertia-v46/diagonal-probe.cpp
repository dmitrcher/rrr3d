#include <NxPhysics.h>
#include <NxUtilLib.h>
#include <windows.h>
#include <cstdio>
#include <float.h>
int main(int argc,char **argv) {
    if(argc!=3) return 2;
    NxPhysicsSDK *sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
    if(!sdk) return 3;
    FILE *in=0,*out=0;
    if(fopen_s(&in,argv[1],"r")||fopen_s(&out,argv[2],"w"))return 4;
    void **table=*reinterpret_cast<void***>(NxGetUtilLib());
    HMODULE core=GetModuleHandleA("PhysXCore.dll");
    fprintf(out,"diagonalizer_rva %lu fpu %u\n",(DWORD)table[32]-(DWORD)core,_controlfp(0,0));
    for(int type=0; type<6;++type) {
        float values[9];for(int i=0;i<9;++i)if(fscanf_s(in,"%f",&values[i])!=1)return 5;
        NxMat33 dense;dense.setRowMajor(values);
        NxVec3 principal;NxMat33 axes;
        bool ok=NxGetUtilLib()->NxDiagonalizeInertiaTensor(dense,principal,axes);
        fprintf(out,"principal %d %d %.9g %.9g %.9g",type,ok?1:0,principal.x,principal.y,principal.z);
        float m[9];axes.getRowMajor(m);for(int i=0;i<9;++i)fprintf(out," %.9g",m[i]);fprintf(out,"\n");
    }
    fclose(in);fclose(out);sdk->release();return 0;
}
