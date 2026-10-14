from pathlib import Path
import json,hashlib
root=Path.cwd();work=root/'work/static-box-query-v107';source=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');adapter=source/'src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp';h=source/'src/Rock3dEngine/header/physics/NativeStaticWheelBoxQueryV107.h';assert not h.exists();old=adapter.read_text();(work/'local-only-adapter.cpp').write_text(old)
h.write_text('''#pragma once
#include "NativeWheelBoxQueryV80.h"
#include "NativeWheelPairLifetimeV106.h"
// Original static wheel queries require a raw suspension-line/box broadphase
// overlap before the local face kernel's epsilon bounds can be consulted.
namespace motorrock::staticwheelboxv107 {
namespace f=motorrock::wheelrayv52;
using Box=motorrock::wheelboxqueryv80::Box;
using Vector=f::Vector;
inline bool candidate(const Box& box,const Vector& start,const Vector& end) {
    motorrock::wheelpairv106::Bounds line{},bounds{};
    for(unsigned axis=0;axis<3;++axis) {
        line[0][axis]=std::min(start[axis],end[axis]);line[1][axis]=std::max(start[axis],end[axis]);
        const float extent=f::add(f::add(f::mul(std::abs(box.columns[0][axis]),box.half[0]),f::mul(std::abs(box.columns[1][axis]),box.half[1])),f::mul(std::abs(box.columns[2][axis]),box.half[2]));
        bounds[0][axis]=f::sub(box.position[axis],extent);bounds[1][axis]=f::add(box.position[axis],extent);
    }
    return motorrock::wheelpairv106::overlap(line,bounds);
}
inline motorrock::wheelboxqueryv80::Result cast(const Box& box,const Vector& start,const Vector& end,const Vector& direction,float length) {
    return candidate(box,start,end)?motorrock::wheelboxqueryv80::cast(box,start,direction,length):motorrock::wheelboxqueryv80::Result{};
}
}
''')
s=old
for a,b in [('#include "physics/NativeWheelBoxQueryV80.h"','#include "physics/NativeWheelBoxQueryV80.h"\n#include "physics/NativeStaticWheelBoxQueryV107.h"'),('        wr::Vector shapeAxis{0,0,1};','        wr::Vector shapeAxis{0,0,1};\n        wr::Vector originalRayEndV107{};'),('                shapeAxis = nativeRay.axis;','                shapeAxis = nativeRay.axis;\n                originalRayEndV107 = nativeRay.end;'),('const bool originalStaticBoxV107=lock.GetBody().IsStatic()','const bool originalStaticBoxV107=nativeOrigin && nativePose && lock.GetBody().IsStatic()'),('                    const auto original=motorrock::wheelboxqueryv80::cast(box,rayStart,unitDirection,queryLength);','''                    const auto original=originalStaticBoxV107
                        ? motorrock::staticwheelboxv107::cast(box,rayStart,originalRayEndV107,unitDirection,queryLength)
                        : motorrock::wheelboxqueryv80::cast(box,rayStart,unitDirection,queryLength);''')]:
 assert s.count(a)==1,a;s=s.replace(a,b)
adapter.write_text(s)
p=work/'source-manifest.json';m=json.loads(p.read_text());m['scope']+=' Static candidates also require the original unpadded line/box AABB overlap; original source ray endpoint is retained before normalization. V106 helper reused for overlap.';m['files'][0]['after_sha256']=hashlib.sha256(adapter.read_bytes()).hexdigest();m['files'].append({'location':'adapter','path':str(h.relative_to(source)),'before_sha256':None,'after_sha256':hashlib.sha256(h.read_bytes()).hexdigest()});p.write_text(json.dumps(m,indent=2)+'\n')
