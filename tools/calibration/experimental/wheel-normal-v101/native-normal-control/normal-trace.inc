static void traceTriangleNormalsV101(NxTriangleMeshShape* shape,const std::string& name) {
 if(!shape || !std::getenv("NX_TRIANGLE_NORMAL_INPUT_TRACE_V101"))return;
 NxTriangleMesh& mesh=shape->getTriangleMesh();
 for(NxU32 index=0;index<mesh.getCount(0,NX_ARRAY_TRIANGLES);++index) {
  NxTriangle triangle;shape->getTriangle(triangle,0,0,index,false,false);
  NxVec3 raw=(triangle.verts[1]-triangle.verts[0])^(triangle.verts[2]-triangle.verts[0]);
  NxReal magnitude=raw.magnitude(),inverse=1.0f/magnitude;NxVec3 normal;triangle.normal(normal);
  std::ostringstream out;out<<"{\"triangle\":"<<index<<",\"vertices_words\":[";
  for(unsigned p=0;p<3;++p)for(unsigned a=0;a<3;++a){NxU32 word;const NxReal f=triangle.verts[p][a];memcpy(&word,&f,4);out<<(p||a?",":"")<<word;}
  const NxReal values[]={raw.x,raw.y,raw.z,magnitude,inverse,normal.x,normal.y,normal.z};out<<"],\"calculation_words\":[";
  for(unsigned i=0;i<8;++i){NxU32 word;memcpy(&word,values+i,4);out<<(i?",":"")<<word;}out<<"]}";
  motorrock::parity::event("calibration.local-triangle-normal-v101",name,out.str());
 }
 const NxU32 count=mesh.getCount(0,NX_ARRAY_NORMALS),stride=mesh.getStride(0,NX_ARRAY_NORMALS);
 const unsigned char* data=static_cast<const unsigned char*>(mesh.getBase(0,NX_ARRAY_NORMALS));
 std::ostringstream array;array<<"{\"count\":"<<count<<",\"stride\":"<<stride<<",\"words\":[";
 if(data && stride>=12)for(NxU32 i=0;i<count;++i)for(unsigned a=0;a<3;++a){NxU32 word;memcpy(&word,data+i*stride+4*a,4);array<<(i||a?",":"")<<word;}array<<"]}";
 motorrock::parity::event("calibration.cooked-normal-array-v101",name,array.str());
 NxRay ray(NxVec3(1.1f,.75f,100),NxVec3(0,0,-1));NxRaycastHit hit;
 if(shape->raycast(ray,1000,NX_RAYCAST_NORMAL|NX_RAYCAST_IMPACT|NX_RAYCAST_FACE_INDEX,hit,true)) {
  std::ostringstream out;out<<"{\"face\":"<<hit.faceID<<",\"words\":[";const NxReal values[]={hit.worldNormal.x,hit.worldNormal.y,hit.worldNormal.z,hit.worldImpact.x,hit.worldImpact.y,hit.worldImpact.z};
  for(unsigned i=0;i<6;++i){NxU32 word;memcpy(&word,values+i,4);out<<(i?",":"")<<word;}out<<"]}";motorrock::parity::event("calibration.sdk-ray-normal-v101",name,out.str());
 }
}
