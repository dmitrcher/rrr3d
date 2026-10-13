//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2018 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.


#pragma once
// Adapted from NVIDIA PhysX 3.4 GuContactBoxBox.cpp (BSD-3-Clause above).
// Standalone float math; no PhysX SDK/runtime dependency. Nx 2.8.4 equality
// must be established against the independent Windows captures.
#include <array>
#include <cmath>
#include <cstdint>
#include <cfloat>
namespace r3d::physics::sourcebox {
using PxReal=float; using PxU32=std::uint32_t; using PxI32=std::int32_t;
inline float PxAbs(float v) { return std::abs(v); }
struct PxVec3 {
 float x=0,y=0,z=0;
 PxVec3()=default; PxVec3(float a,float b,float c):x(a),y(b),z(c){}
 PxVec3 operator+(PxVec3 b)const{return {x+b.x,y+b.y,z+b.z};}
 PxVec3 operator-(PxVec3 b)const{return {x-b.x,y-b.y,z-b.z};}
 PxVec3 operator-()const{return {-x,-y,-z};}
 PxVec3 operator*(float k)const{return {x*k,y*k,z*k};}
 PxVec3& operator+=(PxVec3 b){x+=b.x;y+=b.y;z+=b.z;return *this;}
 PxVec3& operator-=(PxVec3 b){x-=b.x;y-=b.y;z-=b.z;return *this;}
 float dot(PxVec3 b)const{return x*b.x+y*b.y+z*b.z;}
 float magnitudeSquared()const{return dot(*this);}
};
inline PxVec3 operator*(float k,PxVec3 a){return a*k;}
struct Matrix3 {
 PxVec3 column0{1,0,0},column1{0,1,0},column2{0,0,1};
 PxVec3 operator*(PxVec3 v)const{return column0*v.x+column1*v.y+column2*v.z;}
 Matrix3 transpose()const{return {{column0.x,column1.x,column2.x},{column0.y,column1.y,column2.y},{column0.z,column1.z,column2.z}};}
};
namespace Cm {
struct Matrix34 {
 Matrix3 m; PxVec3 p;
 PxVec3 transform(PxVec3 v)const{return m*v+p;}
 Matrix34 getInverseRT()const {Matrix3 t=m.transpose();return {t,-(t*p)};}
 Matrix34 operator*(const Matrix34& b)const{return {{m*b.m.column0,m*b.m.column1,m*b.m.column2},transform(b.p)};}
};
}
// PhysX 2.8.4 box face SAT reconstructed from original x86 instructions.
// A radii retain the final abs(C)+epsilon before its FSTS memory rounding;
// B radii reload rounded coefficients. Keep the original constants and stores.
inline float nativeRoundV72(double value) { volatile float rounded=static_cast<float>(value); return rounded; }
inline float nativeAddV72(float a,float b) { return nativeRoundV72(double(a)+double(b)); }
inline float nativeMulV72(float a,float b) { return nativeRoundV72(double(a)*double(b)); }
inline double nativeExtendedAddV72(double a,double b) { volatile double rounded=a+b; return rounded; }
inline double nativeExtendedMulV72(double a,double b) { volatile double rounded=a*b; return rounded; }
struct NativeFaceAxesV72 { float rotation[3][3],absolute[3][3],distances[6],overlaps[6]; };
inline NativeFaceAxesV72 nativeFaceAxesV72(PxVec3 ha,PxVec3 hb,const Cm::Matrix34& a,const Cm::Matrix34& b,float distance) {
 NativeFaceAxesV72 out{};
 const PxVec3 aa[]={a.m.column0,a.m.column1,a.m.column2};
 const PxVec3 bb[]={b.m.column0,b.m.column1,b.m.column2};
 const float ea[]={ha.x,ha.y,ha.z},eb[]={hb.x,hb.y,hb.z};
 const float epsilon=1e-6f;
 const PxVec3 delta{nativeRoundV72(double(b.p.x)-a.p.x),nativeRoundV72(double(b.p.y)-a.p.y),nativeRoundV72(double(b.p.z)-a.p.z)};
 for(int i=0;i<3;++i) {
  for(int j=0;j<3;++j) {
   const float x=nativeMulV72(aa[i].x,bb[j].x),y=nativeMulV72(aa[i].y,bb[j].y),z=nativeMulV72(aa[i].z,bb[j].z);
   out.rotation[i][j]=i==0?nativeAddV72(nativeAddV72(x,y),z):nativeAddV72(nativeAddV72(z,y),x);
   out.absolute[i][j]=nativeAddV72(PxAbs(out.rotation[i][j]),epsilon);
  }
  out.distances[i]=nativeAddV72(nativeAddV72(nativeMulV72(aa[i].z,delta.z),nativeMulV72(aa[i].x,delta.x)),nativeMulV72(aa[i].y,delta.y));
  double radius=nativeExtendedAddV72(nativeExtendedMulV72(eb[0],out.absolute[i][0]),nativeExtendedMulV72(eb[1],out.absolute[i][1]));
  radius=nativeExtendedAddV72(radius,nativeExtendedMulV72(eb[2],nativeExtendedAddV72(PxAbs(out.rotation[i][2]),epsilon)));
  radius=nativeExtendedAddV72(radius,ea[i]);
  out.overlaps[i]=nativeAddV72(nativeRoundV72(double(nativeRoundV72(radius))-PxAbs(out.distances[i])),distance);
 }
 for(int j=0;j<3;++j) {
  double projection=nativeExtendedAddV72(nativeExtendedMulV72(bb[j].x,delta.x),nativeExtendedMulV72(bb[j].y,delta.y));
  projection=nativeExtendedAddV72(projection,nativeExtendedMulV72(bb[j].z,delta.z));
  out.distances[3+j]=nativeRoundV72(projection);
  double radius=nativeExtendedAddV72(nativeExtendedMulV72(ea[0],out.absolute[0][j]),nativeExtendedMulV72(ea[1],out.absolute[1][j]));
  radius=nativeExtendedAddV72(radius,nativeExtendedMulV72(ea[2],out.absolute[2][j]));
  radius=nativeExtendedAddV72(radius,eb[j]);
  out.overlaps[3+j]=nativeAddV72(nativeRoundV72(double(nativeRoundV72(radius))-PxAbs(out.distances[3+j])),distance);
 }
 return out;
}

// Original PhysX 2.8.4 box face helper, RVA 228278..228343: inverse
// translation sums X,Z,Y for the first column and Z,Y,X for the others.
// Matrix storage remains column-major; these are explicit scalar SSE stores.
inline Cm::Matrix34 nativeInverseRTV76(const Cm::Matrix34& a) {
 const auto& p=a.p; const auto& x=a.m.column0; const auto& y=a.m.column1; const auto& z=a.m.column2;
 const auto sum=[](float first,float second,float third){return nativeAddV72(nativeAddV72(first,second),third);};
 return {a.m.transpose(),{
  sum(nativeMulV72(-p.x,x.x),nativeMulV72(-p.z,x.z),nativeMulV72(-p.y,x.y)),
  sum(nativeMulV72(-p.z,y.z),nativeMulV72(-p.y,y.y),nativeMulV72(-p.x,y.x)),
  sum(nativeMulV72(-p.z,z.z),nativeMulV72(-p.y,z.y),nativeMulV72(-p.x,z.x))}};
}

struct ContactPoint {PxVec3 point,normal;float separation;};
struct ContactBuffer {
 std::array<ContactPoint,64> contacts{};std::uint32_t count=0;
 void reset(){count=0;}
 void contact(PxVec3 p,PxVec3 n,float d){if(count<contacts.size())contacts[count++]={p,n,d};}
};
#define PX_FORCE_INLINE inline
#define PX_RESTRICT
#define PX_X86 0
#define PX_OSX 1
#define PX_MAX_REAL FLT_MAX
#define MAX_NB_CTCS	8 + 12*5 + 6*4
#define ABS_GREATER(x, y)			(PxAbs(x) > (y))
#define ABS_SMALLER_EQUAL(x, y)		(PxAbs(x) <= (y))
//#define AIR(x)					((PxU32&)(x)&SIGN_BITMASK)
//#define ABS_GREATER(x, y)			(AIR(x) > IR(y))
//#define ABS_SMALLER_EQUAL(x, y)		(AIR(x) <= IR(y))

#if PX_X86 && !PX_OSX

	// Some float optimizations ported over from novodex.

	//returns non zero if the value is negative.
	#define PXC_IS_NEGATIVE(x) (((PxU32&)(x)) & 0x80000000)

#else

	//On most platforms using the integer rep is worse(produces LHSs) since the CPU has more registers.

	//returns non zero if the value is negative.
	#define PXC_IS_NEGATIVE(x) ((x) < 0.0f)

#endif


enum
{
	AXIS_A0, AXIS_A1, AXIS_A2,
	AXIS_B0, AXIS_B1, AXIS_B2
};

struct VertexInfo
{
	PxVec3	pos;
	bool	penetrate;
	bool	area;
};


/*static PxI32 doBoxBoxContactGeneration(PxVec3 ctcPts[MAX_NB_CTCS], PxReal depths[MAX_NB_CTCS], PxVec3* ctcNrm,
									 const PxVec3& extents0, const PxVec3& extents1,
									 PxU32& collisionData,
									 const Cm::Matrix34& transform0, const Cm::Matrix34& transform1, PxReal contactDistance, bool nativeSATV72, bool nativeClipV74, bool nativeInverseV76);*/

static PxI32 doBoxBoxContactGeneration(ContactBuffer& contactBuffer,
									 const PxVec3& extents0, const PxVec3& extents1,
									 PxU32& collisionData,
									 const Cm::Matrix34& transform0, const Cm::Matrix34& transform1, PxReal contactDistance, bool nativeSATV72, bool nativeClipV74, bool nativeInverseV76);

// face => 4 vertices of a face of the cube (i.e. a quad)
static PX_FORCE_INLINE PxReal IsInYZ(const PxReal y, const PxReal z, const VertexInfo** PX_RESTRICT face)
{
	// Warning, indices have been remapped. We're now actually like this:
	//
	// 3+------+2
	//  |   |  |
	//  |   *--|
	//  | (y,z)|
	// 0+------+1
	PxReal PreviousY = face[3]->pos.y;
	PxReal PreviousZ = face[3]->pos.z;

	// Loop through quad vertices
	for(PxI32 i=0; i<4; i++)
	{
		const PxReal CurrentY = face[i]->pos.y;
		const PxReal CurrentZ = face[i]->pos.z;

		// |CurrentY - PreviousY      y - PreviousY|
		// |CurrentZ - PreviousZ      z - PreviousZ|
		// => similar to backface culling, check each one of the 4 triangles are consistent, in which case
		// the point is within the parallelogram.
		if((CurrentY - PreviousY)*(z - PreviousZ) - (CurrentZ - PreviousZ)*(y - PreviousY) >= 0.0f)	return -1.0f;

		PreviousY = CurrentY;
		PreviousZ = CurrentZ;
	}

	PxReal x = face[0]->pos.x;
	{
		const PxReal ay = y - face[0]->pos.y;
		const PxReal az = z - face[0]->pos.z;

		PxVec3 b = face[1]->pos - face[0]->pos;	// ### could be precomputed ?
		x += b.x * (ay*b.y + az*b.z) / b.magnitudeSquared();		// ### could be precomputed ?

		b = face[3]->pos - face[0]->pos;		// ### could be precomputed ?
		x += b.x * (ay*b.y + az*b.z) / b.magnitudeSquared();		// ### could be precomputed ?
	}

	return x;
}

// Original x86 edge interpolation stores the product before adding it.
// ARM fused multiply-add gives different contact coordinates on tilted edges.
inline float edgeInterpolateV74(float start,float end,float fraction,bool nativeClipV74) {
 if(!nativeClipV74)return start+(end-start)*fraction;
 const float difference=nativeRoundV72(double(end)-start);
 const float product=nativeMulV72(difference,fraction);
 return nativeAddV72(start,product);
}
// Test with respect to the quad defined by (0,-y1,-z1) and (0,y1,z1)
//  +------+ y1    y
//  |      |       |
//  |  *   |       |
//  |      |       |
//  +------+ -y1   *-----z
static PxI32 generateContacts(//PxVec3 ctcPts[], PxReal depths[],
								ContactBuffer& contactBuffer, const PxVec3& contactNormal,
								PxReal y1, PxReal z1, const PxVec3& box2,
								const Cm::Matrix34& transform0, const Cm::Matrix34& transform1, PxReal contactDistance, bool nativeClipV74, bool nativeInverseV76)
{
//	PxI32 NbContacts=0;
	contactBuffer.reset();
	y1 += contactDistance;
	z1 += contactDistance;

	const Cm::Matrix34 trans1to0 = (nativeInverseV76? nativeInverseRTV76(transform0) : transform0.getInverseRT()) * transform1;

	VertexInfo vtx[8];	// The 8 cube vertices
//	PxI32 i;

	//     6+------+7
	//     /|     /|
	//    / |    / |
	//   / 4+---/--+5
	// 2+------+3 /    y   z
	//  | /    | /     |  /
	//  |/     |/      |/
	// 0+------+1      *---x

	{
		const PxVec3 ex = trans1to0.m.column0 * box2.x;
		const PxVec3 ey = trans1to0.m.column1 * box2.y;
		const PxVec3 ez = trans1to0.m.column2 * box2.z;

		/*
		vtx[0].pos = mat.pos - ex - ey - ez;
		vtx[1].pos = mat.pos + ex - ey - ez;
		vtx[2].pos = mat.pos - ex + ey - ez;
		vtx[3].pos = mat.pos + ex + ey - ez;
		vtx[4].pos = mat.pos - ex - ey + ez;
		vtx[5].pos = mat.pos + ex - ey + ez;
		vtx[6].pos = mat.pos - ex + ey + ez;
		vtx[7].pos = mat.pos + ex + ey + ez;
		*/

		// 12 vector ops = 12*3 = 36 FPU ops
		vtx[0].pos = vtx[2].pos = vtx[4].pos = vtx[6].pos = trans1to0.p - ex;
		vtx[1].pos = vtx[3].pos = vtx[5].pos = vtx[7].pos = trans1to0.p + ex;

		PxVec3 e = ey+ez;
		vtx[0].pos -= e;
		vtx[1].pos -= e;
		vtx[6].pos += e;
		vtx[7].pos += e;

		e = ey-ez;
		vtx[2].pos += e;
		vtx[3].pos += e;
		vtx[4].pos -= e;
		vtx[5].pos -= e;
	}

	// Create vertex info for 8 vertices
	for(PxU32 i=0; i<8; i++)
	{
		// Vertex suivant
		VertexInfo& p = vtx[i];
		// test the point with respect to the x = 0 plane
		//		if(p.pos.x < 0)
		if(p.pos.x < -contactDistance)	//if(PXC_IS_NEGATIVE(p.pos.x))
		{
			p.area		= false;
			p.penetrate	= false;
			continue;
		}

		{
			// we penetrated the quad plane
			p.penetrate = true;
			// test to see if we are in the quad
			// PxAbs => thus we test Y with respect to -Y1 and +Y1 (same for Z)
			//			if(PxAbs(p->pos.y) <= y1 && PxAbs(p->pos.z) <= z1)
			if(ABS_SMALLER_EQUAL(p.pos.y, y1) && ABS_SMALLER_EQUAL(p.pos.z, z1))
			{
				// the point is inside the quad
				p.area=true;
				// Since we are testing with respect to x = 0, the penetration is directly the x coordinate.
//				depths[NbContacts] = p.pos.x;

				// We take the vertex as the impact point
//				ctcPts[NbContacts++] = p.pos;
				contactBuffer.contact(p.pos, contactNormal, -p.pos.x);
			}
			else
			{
				p.area=false;
			}
		}
	}

	// Teste 12 edges on the quad
	static const PxI32 indices[]={ 0,1, 1,3, 3,2, 2,0, 4,5, 5,7, 7,6, 6,4, 0,4, 1,5, 2,6, 3,7, };
	const PxI32* runningLine = indices;
	const PxI32* endLine = runningLine+24;
	while(runningLine!=endLine)
	{
		// The two vertices of the current edge
		const VertexInfo* p1 = &vtx[*runningLine++];
		const VertexInfo* p2 = &vtx[*runningLine++];

		// Penetrate|Area|Penetrate|Area => 16 cases

		// We only take the edges that at least penetrated the quad's plane into account.
		if(p1->penetrate || p2->penetrate)
			//		if(p1->penetrate + p2->penetrate)	// One branch only
		{
			// If at least one of the two vertices is not in the quad...
			if(!p1->area || !p2->area)
				//			if(!p1->area + !p2->area)	// One branch only
			{
				// Test y
				if(p1->pos.y > p2->pos.y)		{ const VertexInfo* tmp=p1; p1=p2; p2=tmp; }
				// Impact on the +Y1 edge of the quad
				if(p1->pos.y < +y1 && p2->pos.y >= +y1)
					// => a point under Y1, the other above
				{
					// Case 1
					PxReal a = (+y1 - p1->pos.y)/(p2->pos.y - p1->pos.y);
					PxReal z = edgeInterpolateV74(p1->pos.z,p2->pos.z,a,nativeClipV74);
					if(PxAbs(z) <= z1)
					{
						PxReal x = edgeInterpolateV74(p1->pos.x,p2->pos.x,a,nativeClipV74);
						if(x+contactDistance>=0.0f)
						{
//							depths[NbContacts] = x;
//							ctcPts[NbContacts++] = PxVec3(x, y1, z);
							contactBuffer.contact(PxVec3(x, y1, z), contactNormal, -x);
						}
					}
				}
				// Impact on the edge -Y1 of the quad
				if(p1->pos.y < -y1 && p2->pos.y >= -y1)
				{
					// Case 2
					PxReal a = (-y1 - p1->pos.y)/(p2->pos.y - p1->pos.y);
					PxReal z = edgeInterpolateV74(p1->pos.z,p2->pos.z,a,nativeClipV74);
					if(PxAbs(z) <= z1)
					{
						PxReal x = edgeInterpolateV74(p1->pos.x,p2->pos.x,a,nativeClipV74);
						if(x+contactDistance>=0.0f)
						{
//							depths[NbContacts] = x;
//							ctcPts[NbContacts++] = PxVec3(x, -y1, z);
							contactBuffer.contact(PxVec3(x, -y1, z), contactNormal, -x);
						}
					}
				}

				// Test z
				if(p1->pos.z > p2->pos.z)		{ const VertexInfo* tmp=p1; p1=p2; p2=tmp; }
				// Impact on the edge +Z1 of the quad
				if(p1->pos.z < +z1 && p2->pos.z >= +z1)
				{
					// Case 3
					PxReal a = (+z1 - p1->pos.z)/(p2->pos.z - p1->pos.z);
					PxReal y = edgeInterpolateV74(p1->pos.y,p2->pos.y,a,nativeClipV74);
					if(PxAbs(y) <= y1)
					{
						PxReal x = edgeInterpolateV74(p1->pos.x,p2->pos.x,a,nativeClipV74);
						if(x+contactDistance>=0.0f)
						{
//							depths[NbContacts] = x;
//							ctcPts[NbContacts++] = PxVec3(x, y, z1);
							contactBuffer.contact(PxVec3(x, y, z1), contactNormal, -x);
						}
					}
				}
				// Impact on the edge -Z1 of the quad
				if(p1->pos.z < -z1 && p2->pos.z >= -z1)
				{
					// Case 4
					PxReal a = (-z1 - p1->pos.z)/(p2->pos.z - p1->pos.z);
					PxReal y = edgeInterpolateV74(p1->pos.y,p2->pos.y,a,nativeClipV74);
					if(PxAbs(y) <= y1)
					{
						PxReal x = edgeInterpolateV74(p1->pos.x,p2->pos.x,a,nativeClipV74);
						if(x+contactDistance>=0.0f)
						{
//							depths[NbContacts] = x;
//							ctcPts[NbContacts++] = PxVec3(x, y, -z1);
							contactBuffer.contact(PxVec3(x, y, -z1), contactNormal, -x);
						}
					}
				}
			}

			// The case where one point penetrates the plane, and the other is not in the quad.
			if((!p1->penetrate && !p2->area) || (!p2->penetrate && !p1->area))
			{
				// Case 5
				PxReal a = (-p1->pos.x)/(p2->pos.x - p1->pos.x);
				PxReal y = edgeInterpolateV74(p1->pos.y,p2->pos.y,a,nativeClipV74);
				if(PxAbs(y) <= y1)
				{
					PxReal z = edgeInterpolateV74(p1->pos.z,p2->pos.z,a,nativeClipV74);
					if(PxAbs(z) <= z1)
					{
//						depths[NbContacts] = 0;
//						ctcPts[NbContacts++] = PxVec3(0, y, z);
						contactBuffer.contact(PxVec3(0, y, z), contactNormal, 0);
					}
				}
			}

		}
	}

	{
		// 6 quads => 6 faces of the cube
		static const PxI32 face[][4]={ {0,1,3,2}, {1,5,7,3}, {5,4,6,7}, {4,0,2,6}, {2,3,7,6}, {0,4,5,1} };
		PxI32 addflg=0;
		for(PxU32 i=0; i<6 && addflg!=0x0f; i++)
		{
			const PxI32* p = face[i];
			const VertexInfo* q[4];
			if((q[0]=&vtx[p[0]])->penetrate && (q[1]=&vtx[p[1]])->penetrate && (q[2]=&vtx[p[2]])->penetrate && (q[3]=&vtx[p[3]])->penetrate)
			{
				if(!q[0]->area || !q[1]->area || !q[2]->area || !q[3]->area)
				{
					if(!(addflg&1))	{ PxReal x = IsInYZ(-y1, -z1, q); if(x>=0.0f)	{ addflg|=1; contactBuffer.contact(PxVec3(x, -y1, -z1), contactNormal, -x); /*depths[NbContacts]=x; ctcPts[NbContacts++] = PxVec3(x, -y1, -z1);*/ } }
					if(!(addflg&2))	{ PxReal x = IsInYZ(+y1, -z1, q); if(x>=0.0f)	{ addflg|=2; contactBuffer.contact(PxVec3(x, +y1, -z1), contactNormal, -x); /*depths[NbContacts]=x; ctcPts[NbContacts++] = PxVec3(x, +y1, -z1);*/ } }
					if(!(addflg&4))	{ PxReal x = IsInYZ(-y1, +z1, q); if(x>=0.0f)	{ addflg|=4; contactBuffer.contact(PxVec3(x, -y1, +z1), contactNormal, -x); /*depths[NbContacts]=x; ctcPts[NbContacts++] = PxVec3(x, -y1, +z1);*/ } }
					if(!(addflg&8))	{ PxReal x = IsInYZ(+y1, +z1, q); if(x>=0.0f)	{ addflg|=8; contactBuffer.contact(PxVec3(x, +y1, +z1), contactNormal, -x); /*depths[NbContacts]=x; ctcPts[NbContacts++] = PxVec3(x, +y1, +z1);*/ } }
				}
			}
		}
	}

//	for(i=0; i<NbContacts; i++)
	for(PxU32 i=0; i<contactBuffer.count; i++)
//		ctcPts[i] = transform0.transform(ctcPts[i]);	// local to world
		contactBuffer.contacts[i].point = transform0.transform(contactBuffer.contacts[i].point);	// local to world

	//PX_ASSERT(NbContacts);	//if this did not make contacts then something went wrong in theory, but even the old code without distances had this flaw!
//	return NbContacts;
	return PxI32(contactBuffer.count);
}

//static PxI32 doBoxBoxContactGeneration(PxVec3 ctcPts[MAX_NB_CTCS], PxReal depths[MAX_NB_CTCS], PxVec3* ctcNrm,
static PxI32 doBoxBoxContactGeneration(ContactBuffer& contactBuffer,
									 const PxVec3& extents0, const PxVec3& extents1,
									 PxU32& collisionData,
									 const Cm::Matrix34& transform0, const Cm::Matrix34& transform1, PxReal contactDistance, bool nativeSATV72, bool nativeClipV74, bool nativeInverseV76)
{
	PxReal aafC[3][3];			// matrix C = A^T B, c_{ij} = Dot(A_i,B_j)
	PxReal aafAbsC[3][3];		// |c_{ij}|
	PxReal afAD[3];			// Dot(A_i,D)

	PxReal d1[6];
	PxReal overlap[6];

	PxVec3 kD = transform1.p - transform0.p;

	const PxVec3& axis00 = transform0.m.column0;
	const PxVec3& axis01 = transform0.m.column1;
	const PxVec3& axis02 = transform0.m.column2;
	const PxVec3& axis10 = transform1.m.column0;
	const PxVec3& axis11 = transform1.m.column1;
	const PxVec3& axis12 = transform1.m.column2;

	PxReal d0;
 if(nativeSATV72) {
  const auto native=nativeFaceAxesV72(extents0,extents1,transform0,transform1,contactDistance);
  for(int i=0;i<3;++i) {
   afAD[i]=native.distances[i];
   for(int j=0;j<3;++j) { aafC[i][j]=native.rotation[i][j];aafAbsC[i][j]=native.absolute[i][j]; }
  }
  for(int i=0;i<6;++i) { d1[i]=native.distances[i];overlap[i]=native.overlaps[i];if(PXC_IS_NEGATIVE(overlap[i]))return 0; }
 } else {
	// Perform Class I tests

	aafC[0][0]	= axis00.dot(axis10);
	aafC[0][1]	= axis00.dot(axis11);
	aafC[0][2]	= axis00.dot(axis12);
	afAD[0]		= axis00.dot(kD);
	aafAbsC[0][0] = 1e-6f + PxAbs(aafC[0][0]);
	aafAbsC[0][1] = 1e-6f + PxAbs(aafC[0][1]);
	aafAbsC[0][2] = 1e-6f + PxAbs(aafC[0][2]);
	d1[AXIS_A0] = afAD[0];
	d0 = extents0.x + extents1.x*aafAbsC[0][0] + extents1.y*aafAbsC[0][1] + extents1.z*aafAbsC[0][2];
	overlap[AXIS_A0] = d0 - PxAbs(d1[AXIS_A0]) + contactDistance;
	if(PXC_IS_NEGATIVE(overlap[AXIS_A0]))		return 0;

	aafC[1][0]	= axis01.dot(axis10);
	aafC[1][1]	= axis01.dot(axis11);
	aafC[1][2]	= axis01.dot(axis12);
	afAD[1]		= axis01.dot(kD);
	aafAbsC[1][0] = 1e-6f + PxAbs(aafC[1][0]);
	aafAbsC[1][1] = 1e-6f + PxAbs(aafC[1][1]);
	aafAbsC[1][2] = 1e-6f + PxAbs(aafC[1][2]);
	d1[AXIS_A1] = afAD[1];
	d0 = extents0.y + extents1.x*aafAbsC[1][0] + extents1.y*aafAbsC[1][1] + extents1.z*aafAbsC[1][2];
	overlap[AXIS_A1] = d0 - PxAbs(d1[AXIS_A1]) + contactDistance;
	if(PXC_IS_NEGATIVE(overlap[AXIS_A1]))		return 0;

	aafC[2][0]	= axis02.dot(axis10);
	aafC[2][1]	= axis02.dot(axis11);
	aafC[2][2]	= axis02.dot(axis12);
	afAD[2]		= axis02.dot(kD);
	aafAbsC[2][0] = 1e-6f + PxAbs(aafC[2][0]);
	aafAbsC[2][1] = 1e-6f + PxAbs(aafC[2][1]);
	aafAbsC[2][2] = 1e-6f + PxAbs(aafC[2][2]);
	d1[AXIS_A2] = afAD[2];
	d0 = extents0.z + extents1.x*aafAbsC[2][0] + extents1.y*aafAbsC[2][1] + extents1.z*aafAbsC[2][2];
	overlap[AXIS_A2] = d0 - PxAbs(d1[AXIS_A2]) + contactDistance;
	if(PXC_IS_NEGATIVE(overlap[AXIS_A2]))		return 0;

	// Perform Class II tests

	d1[AXIS_B0] = axis10.dot(kD);
	d0 = extents1.x + extents0.x*aafAbsC[0][0] + extents0.y*aafAbsC[1][0] + extents0.z*aafAbsC[2][0];
	overlap[AXIS_B0] = d0 - PxAbs(d1[AXIS_B0]) + contactDistance;
	if(PXC_IS_NEGATIVE(overlap[AXIS_B0]))		return 0;

	d1[AXIS_B1] = axis11.dot(kD);
	d0 = extents1.y + extents0.x*aafAbsC[0][1] + extents0.y*aafAbsC[1][1] + extents0.z*aafAbsC[2][1];
	overlap[AXIS_B1] = d0 - PxAbs(d1[AXIS_B1]) + contactDistance;
	if(PXC_IS_NEGATIVE(overlap[AXIS_B1]))		return 0;

	d1[AXIS_B2] = axis12.dot(kD);
	d0 = extents1.z + extents0.x*aafAbsC[0][2] + extents0.y*aafAbsC[1][2] + extents0.z*aafAbsC[2][2];
	overlap[AXIS_B2] = d0 - PxAbs(d1[AXIS_B2]) + contactDistance;
	if(PXC_IS_NEGATIVE(overlap[AXIS_B2]))		return 0;

 }

	// Perform Class III tests - we don't need to store distances for those ones.
	// We only test those axes when objects are likely to be separated, i.e. when they where previously non-colliding. For stacks, we'll have
	// to do full contact generation anyway, and those tests are useless - so we skip them. This is similar to what I did in Opcode.
	if(!collisionData)	// separated or first run
	{
		PxReal d = afAD[2]*aafC[1][0] - afAD[1]*aafC[2][0];
		d0 = contactDistance + extents0.y*aafAbsC[2][0] + extents0.z*aafAbsC[1][0] + extents1.y*aafAbsC[0][2] + extents1.z*aafAbsC[0][1];
		if(ABS_GREATER(d, d0))	return 0;

		d = afAD[2]*aafC[1][1] - afAD[1]*aafC[2][1];
		d0 = contactDistance + extents0.y*aafAbsC[2][1] + extents0.z*aafAbsC[1][1] + extents1.x*aafAbsC[0][2] + extents1.z*aafAbsC[0][0];
		if(ABS_GREATER(d, d0))	return 0;

		d = afAD[2]*aafC[1][2] - afAD[1]*aafC[2][2];
		d0 = contactDistance + extents0.y*aafAbsC[2][2] + extents0.z*aafAbsC[1][2] + extents1.x*aafAbsC[0][1] + extents1.y*aafAbsC[0][0];
		if(ABS_GREATER(d, d0))	return 0;

		d = afAD[0]*aafC[2][0] - afAD[2]*aafC[0][0];
		d0 = contactDistance + extents0.x*aafAbsC[2][0] + extents0.z*aafAbsC[0][0] + extents1.y*aafAbsC[1][2] + extents1.z*aafAbsC[1][1];
		if(ABS_GREATER(d, d0))	return 0;

		d = afAD[0]*aafC[2][1] - afAD[2]*aafC[0][1];
		d0 = contactDistance + extents0.x*aafAbsC[2][1] + extents0.z*aafAbsC[0][1] + extents1.x*aafAbsC[1][2] + extents1.z*aafAbsC[1][0];
		if(ABS_GREATER(d, d0))	return 0;

		d = afAD[0]*aafC[2][2] - afAD[2]*aafC[0][2];
		d0 = contactDistance + extents0.x*aafAbsC[2][2] + extents0.z*aafAbsC[0][2] + extents1.x*aafAbsC[1][1] + extents1.y*aafAbsC[1][0];
		if(ABS_GREATER(d, d0))	return 0;

		d = afAD[1]*aafC[0][0] - afAD[0]*aafC[1][0];
		d0 = contactDistance + extents0.x*aafAbsC[1][0] + extents0.y*aafAbsC[0][0] + extents1.y*aafAbsC[2][2] + extents1.z*aafAbsC[2][1];
		if(ABS_GREATER(d, d0))	return 0;

		d = afAD[1]*aafC[0][1] - afAD[0]*aafC[1][1];
		d0 = contactDistance + extents0.x*aafAbsC[1][1] + extents0.y*aafAbsC[0][1] + extents1.x*aafAbsC[2][2] + extents1.z*aafAbsC[2][0];
		if(ABS_GREATER(d, d0))	return 0;

		d = afAD[1]*aafC[0][2] - afAD[0]*aafC[1][2];
		d0 = contactDistance + extents0.x*aafAbsC[1][2] + extents0.y*aafAbsC[0][2] + extents1.x*aafAbsC[2][1] + extents1.y*aafAbsC[2][0];
		if(ABS_GREATER(d, d0))	return 0;
	}

	/* djs - tempUserData can be zero when it gets here
	- maybe if there was no previous axis?
	- which causes stack corruption, and thence a crash, in .NET

	PT:	right! At first tempUserData wasn't ever supposed to be zero, but then I used that
	value to mark separation of boxes, and forgot to update the code below. Now I think
	the test is redundant with the one performed above, and the line could eventually
	be merged in the previous block. I'll do that later when removing all the #defines.
	*/
	// NB: the "16" here has nothing to do with MAX_NB_CTCS. Don't touch.
	if(collisionData) // if initialized & not previously separated
		overlap[collisionData-1] *= 0.999f;	 // Favorise previous axis  .999 is too little.

	PxReal	minimum = PX_MAX_REAL;
	PxI32	minIndex = 0;

	for(PxU32 i=AXIS_A0; i<6; i++)
	{
		PxReal d = overlap[i];

		if(d>=0.0f && d<minimum)	{ minimum=d; minIndex=PxI32(i); }		// >=0 !!  otherwise bug at sep = 0
	}

	collisionData = PxU32(minIndex + 1);	// Leave "0" for separation

#if PX_X86
	const PxU32 sign = PXC_IS_NEGATIVE(d1[minIndex]);
#else
	const PxU32 sign = PxU32(PXC_IS_NEGATIVE(d1[minIndex]));
#endif
	Cm::Matrix34 trs;
	PxVec3 ctcNrm;

	switch(minIndex)
	{
	default:
		return 0;

	case AXIS_A0:
//		*ctcNrm = axis00;
		if(sign)
		{
			ctcNrm = axis00;
			trs.m = transform0.m;
			trs.p = transform0.p - extents0.x*axis00;
		}
		else
		{
//			*ctcNrm = -*ctcNrm;
			ctcNrm = -axis00;

			trs.m.column0 = -axis00;
			trs.m.column1 = -axis01;
			trs.m.column2 = axis02;
			trs.p = transform0.p + extents0.x*axis00;
		}
//		return generateContacts(ctcPts, depths, extents0.y, extents0.z, extents1, trs, transform1, contactDistance, nativeClipV74, nativeInverseV76);
		return generateContacts(contactBuffer, ctcNrm, extents0.y, extents0.z, extents1, trs, transform1, contactDistance, nativeClipV74, nativeInverseV76);

	case AXIS_A1:
//		*ctcNrm = axis01;
		trs.m.column2 = axis00;	// Factored out
		if(sign)
		{
			ctcNrm = axis01;
			trs.m.column0 = axis01;
			trs.m.column1 = axis02;
			trs.p = transform0.p - extents0.y*axis01;
		}
		else
		{
//			*ctcNrm = -*ctcNrm;
			ctcNrm = -axis01;

			trs.m.column0 = -axis01;
			trs.m.column1 = -axis02;
			trs.p = transform0.p + extents0.y*axis01;
		}
//		return generateContacts(ctcPts, depths, extents0.z, extents0.x, extents1, trs, transform1, contactDistance, nativeClipV74, nativeInverseV76);
		return generateContacts(contactBuffer, ctcNrm, extents0.z, extents0.x, extents1, trs, transform1, contactDistance, nativeClipV74, nativeInverseV76);

	case AXIS_A2:
//		*ctcNrm = axis02;
		trs.m.column2 = axis01;	// Factored out

		if(sign)
		{
			ctcNrm = axis02;
			trs.m.column0 = axis02;
			trs.m.column1 = axis00;
			trs.p = transform0.p - extents0.z*axis02;
		}
		else
		{
//			*ctcNrm = -*ctcNrm;
			ctcNrm = -axis02;

			trs.m.column0 = -axis02;
			trs.m.column1 = -axis00;
			trs.p = transform0.p + extents0.z*axis02;
		}
//		return generateContacts(ctcPts, depths, extents0.x, extents0.y, extents1, trs, transform1, contactDistance, nativeClipV74, nativeInverseV76);
		return generateContacts(contactBuffer, ctcNrm, extents0.x, extents0.y, extents1, trs, transform1, contactDistance, nativeClipV74, nativeInverseV76);

	case AXIS_B0:
//		*ctcNrm = axis10;
		if(sign)
		{
			ctcNrm = axis10;
			trs.m.column0 = -axis10;
			trs.m.column1 = -axis11;
			trs.m.column2 = axis12;
			trs.p = transform1.p + extents1.x*axis10;
		}
		else
		{
//			*ctcNrm = -*ctcNrm;
			ctcNrm = -axis10;
			trs.m = transform1.m;
			trs.p = transform1.p - extents1.x*axis10;

		}
//		return generateContacts(ctcPts, depths, extents1.y, extents1.z, extents0, trs, transform0, contactDistance, nativeClipV74, nativeInverseV76);
		return generateContacts(contactBuffer, ctcNrm, extents1.y, extents1.z, extents0, trs, transform0, contactDistance, nativeClipV74, nativeInverseV76);

	case AXIS_B1:
//		*ctcNrm = axis11;
		trs.m.column2 = axis10;	// Factored out
		if(sign)
		{
			ctcNrm = axis11;
			trs.m.column0 = -axis11;
			trs.m.column1 = -axis12;
			trs.p = transform1.p + extents1.y*axis11;
		}
		else
		{
//			*ctcNrm = -*ctcNrm;
			ctcNrm = -axis11;

			trs.m.column0 = axis11;
			trs.m.column1 = axis12;
			trs.m.column2 = axis10;
			trs.p = transform1.p - extents1.y*axis11;
		}
//		return generateContacts(ctcPts, depths, extents1.z, extents1.x, extents0, trs, transform0, contactDistance, nativeClipV74, nativeInverseV76);
		return generateContacts(contactBuffer, ctcNrm, extents1.z, extents1.x, extents0, trs, transform0, contactDistance, nativeClipV74, nativeInverseV76);

	case AXIS_B2:
//		*ctcNrm = axis12;
		trs.m.column2 = axis11;	// Factored out

		if(sign)
		{
			ctcNrm = axis12;
			trs.m.column0 = -axis12;
			trs.m.column1 = -axis10;
			trs.p = transform1.p + extents1.z*axis12;
		}
		else
		{
//			*ctcNrm = -*ctcNrm;
			ctcNrm = -axis12;

			trs.m.column0 = axis12;
			trs.m.column1 = axis10;
			trs.p = transform1.p - extents1.z*axis12;
		}

//		return generateContacts(ctcPts, depths, extents1.x, extents1.y, extents0, trs, transform0, contactDistance, nativeClipV74, nativeInverseV76);
		return generateContacts(contactBuffer, ctcNrm, extents1.x, extents1.y, extents0, trs, transform0, contactDistance, nativeClipV74, nativeInverseV76);
	}
}

inline bool contacts(ContactBuffer& out, PxVec3 a, PxVec3 b,
 const Cm::Matrix34& ta,const Cm::Matrix34& tb,PxU32& axis,float distance=0,bool nativeSATV72=false,bool nativeClipV74=false,bool nativeInverseV76=false) {
 return doBoxBoxContactGeneration(out,a,b,axis,ta,tb,distance,nativeSATV72,nativeClipV74,nativeInverseV76)>0;
}
#undef MAX_NB_CTCS
#undef ABS_GREATER
#undef ABS_SMALLER_EQUAL
#undef PXC_IS_NEGATIVE
#undef PX_FORCE_INLINE
#undef PX_RESTRICT
#undef PX_X86
#undef PX_OSX
#undef PX_MAX_REAL
}
