#include "NativeWheelBoxRayV59.h"
#include <cstring>
#include <cstdint>
#include <cstdio>
float f(uint32_t bits){float v;std::memcpy(&v,&bits,4);return v;}
int main(){using namespace motorrock::wheelboxrayv59;
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(981668463u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":0,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(981668463u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":1,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(981668463u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":2,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(981668463u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":3,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057071u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":4,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057071u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":5,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057071u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":6,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057071u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":7,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057114u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":8,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057114u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":9,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057114u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":10,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057114u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":11,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057500u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":12,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057500u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":13,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057500u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":14,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057500u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":15,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057930u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":16,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057930u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":17,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057930u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":18,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990057930u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":19,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990059218u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":20,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990059218u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":21,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990059218u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":22,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990059218u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":23,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990061366u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":24,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990061366u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":25,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990061366u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":26,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990061366u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":27,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990065661u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":28,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990065661u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":29,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990065661u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":30,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990065661u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":31,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990078546u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":32,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990078546u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":33,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990078546u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":34,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990078546u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":35,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990100021u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":36,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990100021u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":37,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990100021u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":38,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990100021u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":39,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990142970u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":40,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990142970u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":41,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990142970u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":42,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990142970u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":43,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990271819u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":44,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990271819u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":45,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990271819u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":46,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990271819u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":47,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990486568u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":48,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990486568u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":49,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990486568u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":50,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990486568u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":51,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990916064u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":52,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990916064u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":53,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990916064u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":54,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(990916064u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":55,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(992204555u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":56,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(992204555u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":57,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(992204555u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":58,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(992204555u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":59,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(994352038u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":60,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(994352038u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":61,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(994352038u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":62,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(994352038u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":63,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1011129254u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":64,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1011129254u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":65,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1011129254u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":66,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(990057071u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1011129254u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":67,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1016833507u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":68,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1016833507u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":69,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1016833507u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":70,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1016833507u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":71,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370378u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":72,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370378u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":73,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370378u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":74,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370378u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":75,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370383u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":76,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370383u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":77,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370383u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":78,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370383u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":79,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370432u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":80,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370432u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":81,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370432u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":82,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370432u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":83,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370485u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":84,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370485u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":85,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370485u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":86,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370485u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":87,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370646u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":88,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370646u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":89,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370646u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":90,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370646u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":91,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370915u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":92,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370915u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":93,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370915u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":94,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017370915u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":95,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017371452u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":96,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017371452u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":97,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017371452u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":98,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017371452u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":99,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017373062u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":100,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017373062u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":101,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017373062u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":102,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017373062u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":103,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017375747u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":104,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017375747u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":105,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017375747u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":106,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017375747u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":107,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017381115u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":108,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017381115u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":109,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017381115u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":110,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017381115u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":111,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017397222u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":112,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017397222u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":113,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017397222u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":114,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017397222u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":115,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017424065u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":116,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017424065u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":117,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017424065u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":118,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017424065u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":119,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017477752u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":120,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017477752u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":121,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017477752u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":122,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017477752u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":123,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017638813u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":124,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017638813u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":125,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017638813u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":126,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017638813u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":127,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017907249u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":128,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017907249u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":129,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017907249u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":130,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1017907249u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":131,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1022739087u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":132,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1022739087u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":133,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1022739087u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":134,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1017370378u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1022739087u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":135,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045153448u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":136,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045153448u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":137,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045153448u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":138,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045153448u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":139,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220557u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":140,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220557u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":141,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220557u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":142,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220557u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":143,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220558u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":144,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220558u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":145,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220558u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":146,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220558u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":147,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220564u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":148,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220564u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":149,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220564u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":150,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220564u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":151,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220570u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":152,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220570u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":153,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220570u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":154,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220570u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":155,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220591u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":156,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220591u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":157,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220591u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":158,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220591u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":159,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220624u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":160,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220624u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":161,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220624u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":162,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220624u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":163,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220691u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":164,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220691u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":165,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220691u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":166,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220691u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":167,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220893u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":168,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220893u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":169,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220893u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":170,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045220893u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":171,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045221228u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":172,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045221228u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":173,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045221228u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":174,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045221228u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":175,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045221899u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":176,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045221899u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":177,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045221899u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":178,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045221899u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":179,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045223912u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":180,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045223912u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":181,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045223912u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":182,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045223912u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":183,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045227268u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":184,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045227268u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":185,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045227268u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":186,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045227268u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":187,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045233979u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":188,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045233979u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":189,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045233979u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":190,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045233979u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":191,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045254111u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":192,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045254111u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":193,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045254111u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":194,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045254111u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":195,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045287666u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":196,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045287666u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":197,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045287666u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":198,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045287666u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":199,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045891646u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":200,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045891646u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":201,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045891646u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":202,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1045220557u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1045891646u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":203,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069539131u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":204,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069539131u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":205,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069539131u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":206,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069539131u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":207,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547520u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":208,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547520u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":209,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547520u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":210,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547520u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":211,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547520u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":212,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547520u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":213,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547520u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":214,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547520u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":215,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547521u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":216,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547521u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":217,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547521u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":218,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547521u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":219,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547522u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":220,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547522u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":221,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547522u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":222,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547522u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":223,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547524u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":224,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547524u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":225,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547524u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":226,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547524u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":227,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547528u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":228,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547528u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":229,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547528u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":230,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547528u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":231,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547537u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":232,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547537u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":233,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547537u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":234,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547537u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":235,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547562u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":236,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547562u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":237,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547562u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":238,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547562u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":239,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547604u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":240,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547604u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":241,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547604u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":242,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547604u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":243,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547688u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":244,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547688u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":245,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547688u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":246,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547688u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":247,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547939u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":248,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547939u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":249,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547939u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":250,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069547939u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":251,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069548359u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":252,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069548359u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":253,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069548359u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":254,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069548359u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":255,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069549198u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":256,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069549198u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":257,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069549198u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":258,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069549198u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":259,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069551714u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":260,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069551714u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":261,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069551714u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":262,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069551714u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":263,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069555909u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":264,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069555909u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":265,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069555909u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":266,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069555909u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":267,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069631406u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":268,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069631406u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":269,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069631406u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":270,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1069547520u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1069631406u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":271,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004276u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":272,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004276u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":273,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004276u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":274,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004276u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":275,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":276,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":277,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":278,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":279,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":280,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":281,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":282,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":283,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":284,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":285,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":286,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":287,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":288,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":289,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":290,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":291,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":292,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":293,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":294,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004800u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":295,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004801u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":296,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004801u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":297,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004801u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":298,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004801u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":299,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004801u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":300,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004801u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":301,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004801u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":302,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004801u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":303,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004803u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":304,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004803u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":305,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004803u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":306,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004803u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":307,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004805u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":308,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004805u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":309,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004805u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":310,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004805u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":311,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004810u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":312,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004810u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":313,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004810u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":314,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004810u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":315,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004826u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":316,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004826u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":317,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004826u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":318,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004826u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":319,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004852u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":320,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004852u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":321,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004852u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":322,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004852u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":323,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004905u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":324,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004905u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":325,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004905u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":326,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101004905u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":327,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101005062u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":328,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101005062u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":329,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101005062u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":330,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101005062u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":331,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101005324u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":332,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101005324u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":333,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101005324u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":334,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101005324u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":335,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101010043u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":336,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101010043u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":337,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101010043u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":338,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1101004800u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1101010043u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":339,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128791998u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":340,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128791998u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":341,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128791998u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":342,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128791998u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":343,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":344,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":345,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":346,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":347,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":348,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":349,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":350,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":351,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":352,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":353,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":354,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":355,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":356,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":357,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":358,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":359,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":360,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":361,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":362,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":363,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":364,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":365,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":366,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":367,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":368,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":369,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":370,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":371,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":372,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":373,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":374,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792064u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":375,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792065u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":376,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792065u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":377,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792065u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":378,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792065u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":379,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792065u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":380,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792065u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":381,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792065u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":382,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792065u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":383,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792067u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":384,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792067u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":385,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792067u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":386,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792067u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":387,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792071u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":388,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792071u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":389,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792071u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":390,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792071u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":391,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792077u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":392,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792077u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":393,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792077u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":394,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792077u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":395,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792097u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":396,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792097u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":397,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792097u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":398,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792097u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":399,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792130u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":400,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792130u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":401,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792130u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":402,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792130u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":403,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792719u),f(0u),f(1063675494u)}};
Vector direction={{f(0u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":404,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792719u),f(0u),f(1063675494u)}};
Vector direction={{f(2794462589u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":405,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792719u),f(0u),f(1063675494u)}};
Vector direction={{f(2989214839u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":406,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
{Box box;
box.half={{f(1128792064u),f(1050253722u),f(1050253722u)}};
box.position={{f(0u),f(0u),f(0u)}};
box.columns[0]={{f(1065353216u),f(0u),f(0u)}};
box.columns[1]={{f(0u),f(1065353216u),f(0u)}};
box.columns[2]={{f(0u),f(0u),f(1065353216u)}};
Vector start={{f(1128792719u),f(0u),f(1063675494u)}};
Vector direction={{f(3100751639u),f(0u),f(3212836864u)}};
auto got=cast(box,start,direction,10);std::printf("{\"query\":407,\"hit\":%s,\"distance\":%.9g,\"point\":[%.9g,%.9g,%.9g],\"normal\":[%.9g,%.9g,%.9g]}\n",got.hit?"true":"false",got.distance,got.point[0],got.point[1],got.point[2],got.normal[0],got.normal[1],got.normal[2]);}
}
