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
#include <array>
#include <vector>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cfloat>
#include <cstring>
#include <algorithm>
#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#endif
#include <cassert>
namespace r3d::physics::sourcemesh {
using PxReal=float;using PxU32=std::uint32_t;using PxU8=std::uint8_t;
struct PxVec3 {
 float x=0,y=0,z=0;PxVec3()=default;explicit PxVec3(float v):x(v),y(v),z(v){};PxVec3(float a,float b,float c):x(a),y(b),z(c){}
 float& operator[](int i){if(i==0)return x;if(i==1)return y;assert(i==2);return z;}float operator[](int i)const{if(i==0)return x;if(i==1)return y;assert(i==2);return z;}
 PxVec3 operator+(PxVec3 b)const{return {x+b.x,y+b.y,z+b.z};}PxVec3 operator-(PxVec3 b)const{return {x-b.x,y-b.y,z-b.z};}
 PxVec3 operator-()const{return {-x,-y,-z};}PxVec3 operator*(float k)const{return {x*k,y*k,z*k};}PxVec3 operator/(float k)const{return *this*(1.0f/k);}
 PxVec3& operator+=(PxVec3 b){x+=b.x;y+=b.y;z+=b.z;return *this;}PxVec3& operator-=(PxVec3 b){x-=b.x;y-=b.y;z-=b.z;return *this;}
 float dot(PxVec3 b)const{return x*b.x+y*b.y+z*b.z;}PxVec3 cross(PxVec3 b)const{return {y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x};}
 float magnitudeSquared()const{return dot(*this);}float magnitude()const{return std::sqrt(magnitudeSquared());}PxVec3 getNormalized()const{return *this/magnitude();}
};
inline PxVec3 operator*(float k,PxVec3 a){return a*k;}inline float PxAbs(float a){return std::abs(a);}inline float PxMax(float a,float b){return std::max(a,b);}
struct PxMat33 {
 PxVec3 column0{1,0,0},column1{0,1,0},column2{0,0,1};
 float& operator()(int r,int c){if(c==0)return column0[r];if(c==1)return column1[r];assert(c==2);return column2[r];}
 PxVec3 transform(PxVec3 v)const{return column0*v.x+column1*v.y+column2*v.z;}
 PxVec3 transformTranspose(PxVec3 v)const{return {column0.dot(v),column1.dot(v),column2.dot(v)};}
 PxMat33 getTranspose()const{return {{column0.x,column1.x,column2.x},{column0.y,column1.y,column2.y},{column0.z,column1.z,column2.z}};}
};
namespace Cm {struct Matrix34 {
 PxMat33 m;PxVec3 p;Matrix34()=default;Matrix34(PxMat33 a,PxVec3 b):m(a),p(b){};Matrix34(PxVec3 a,PxVec3 b,PxVec3 c,PxVec3 d):m{a,b,c},p(d){}
 PxVec3 transform(PxVec3 v)const{return m.transform(v)+p;}PxVec3 rotate(PxVec3 v)const{return m.transform(v);}PxVec3 rotateTranspose(PxVec3 v)const{return m.transformTranspose(v);}
 Matrix34 getInverseRT()const{PxMat33 t=m.getTranspose();return {t,-t.transform(p)};}
 Matrix34 operator*(const Matrix34& b)const{return {rotate(b.m.column0),rotate(b.m.column1),rotate(b.m.column2),transform(b.p)};}
};}
struct PxPlane {
 PxVec3 n;float d;PxPlane(PxVec3 a,float b):n(a),d(b){};PxPlane(PxVec3 a,PxVec3 b,PxVec3 c):n((b-a).cross(c-a).getNormalized()),d(-n.dot(a)){};
 float distance(PxVec3 v)const{return n.dot(v)+d;}
};
struct ContactPoint {PxVec3 point,normal;float separation;PxU32 internalFaceIndex1=0;bool nativeHullReference=false;};
struct ContactBuffer {
 static constexpr unsigned MAX_CONTACTS=64;std::array<ContactPoint,MAX_CONTACTS> contacts{};unsigned count=0;
 ContactPoint* contact(){return count<MAX_CONTACTS?&contacts[count++]:nullptr;}
};
namespace intrinsics {
 inline float selectMin(float a,float b){return std::min(a,b);}inline float selectMax(float a,float b){return std::max(a,b);}inline float fsel(float a,float b,float c){return a>=0?b:c;}
}
namespace Ps {
 inline float degToRad(float a){return a*0.017453292519943295f;}inline PxVec3 cross001(PxVec3 a){return {-a.y,a.x,0};}
 inline void closestAxis(PxVec3 a,PxU32& i,PxU32& j){int k=0;if(std::abs(a.y)>std::abs(a.x))k=1;if(std::abs(a.z)>std::abs(a[k]))k=2;i=(k+1)%3;j=(k+2)%3;}
}
#define PX_FORCE_INLINE inline
#define PX_RESTRICT
#define PX_MAX_F32 FLT_MAX
#ifdef _MSC_VER
#define PxAlloca _alloca
#else
#define PxAlloca alloca
#endif
#define PxMemZero(p,n) std::memset(p,0,n)
#define PX_ASSERT assert
#define CONTACT_REDUCTION

/*
void gVisualizeLocalLine(const PxVec3& a, const PxVec3& b, const Cm::Matrix34& m, PxsContactManager& manager)	//temp debug
{
	Cm::RenderOutput out = manager.getContext()->getRenderOutput();
	out << 0xffffff << m << Cm::RenderOutput::LINES << a << b;
}
*/

#ifdef CONTACT_REDUCTION
static PX_FORCE_INLINE PxReal dot2D(const PxVec3& v0, const PxVec3& v1)
{	
	return v0.x * v1.x + v0.y * v1.y;
}

static void ContactReductionAllIn(	ContactBuffer& contactBuffer, PxU32 nbExistingContacts, PxU32 numIn,
									const PxMat33& rotT,
									const PxVec3* PX_RESTRICT vertices, const PxU8* PX_RESTRICT indices)
{
	// Number of contacts created by current call
	const PxU32 nbNewContacts = contactBuffer.count - nbExistingContacts;

	if(nbNewContacts<=4)
		return;	// no reduction for less than 4 verts

	// We have 3 different numbers here:
	// - numVerts = number of vertices in the convex polygon we're dealing with
	// - numIn = number of those that were "inside" the other convex polygon (should be <= numVerts)
	// - contactBuffer.count = total number of contacts *from both polygons* (that's the catch here)
	// The fast path can only be chosen when the contact buffer contains all the verts from current polygon,
	// i.e. when contactBuffer.count == numIn == numVerts

	ContactPoint* PX_RESTRICT ctcs = contactBuffer.contacts.data() + nbExistingContacts;

	if(numIn == nbNewContacts)
	{
		// Codepath 1: all vertices generated a contact

		PxReal deepestSeparation = ctcs[0].separation;
		PxU32 deepestIndex = 0;
		for(PxU32 i=1; i<nbNewContacts; ++i)
		{
			if(deepestSeparation > ctcs[i].separation)
			{
				deepestSeparation = ctcs[i].separation;
				deepestIndex = i;
			}
		}

		PxU32 index = 0;
		const PxU32 step = (numIn<<16)>>2;	// Fixed point math, don't use floats here please
		bool needsExtraPoint = true;
		for(PxU32 i=0;i<4;i++)
		{
			const PxU32 contactIndex = index>>16;
			ctcs[i] = ctcs[contactIndex];
			if(contactIndex==deepestIndex)
				needsExtraPoint = false;
			index += step;
		}

		if(needsExtraPoint)
		{
			ctcs[4] = ctcs[deepestIndex];
			contactBuffer.count = nbExistingContacts + 5;
		}
		else
		{
			contactBuffer.count = nbExistingContacts + 4;
		}

/*	PT: TODO: investigate why this one does not work
		PxU32 index = deepestIndex<<16;
		const PxU32 step = (numIn<<16)>>2;	// Fixed point math, don't use floats here please
		for(PxU32 i=0;i<4;i++)
		{
			PxU32 contactIndex = index>>16;
			if(contactIndex>=numIn)
				contactIndex -= numIn;
			ctcs[i] = ctcs[contactIndex];
			index += step;
		}
		contactBuffer.count = nbExistingContacts + 4;*/
	}
	else
	{
		// Codepath 2: all vertices are "in" but only some of them generated a contact

		// WARNING: this path doesn't work when the buffer contains vertices from both polys.

		// TODO: precompute those axes
		const PxU32 nbAxes = 8;
		PxVec3 dirs[nbAxes];
		float angle = 0.0f;
		const float angleStep = Ps::degToRad(180.0f/float(nbAxes));
		for(PxU32 i=0;i<nbAxes;i++)
		{
			dirs[i] = PxVec3(cosf(angle), sinf(angle), 0.0f);
			angle += angleStep;
		}

		float dpmin[nbAxes];
		float dpmax[nbAxes];
		for(PxU32 i=0;i<nbAxes;i++)
		{
			dpmin[i] = PX_MAX_F32;
			dpmax[i] = -PX_MAX_F32;
		}

		for(PxU32 i=0;i<nbNewContacts;i++)
		{
			const PxVec3& p = vertices[indices[i]];

			// Transform to 2D
			const PxVec3 p2d = rotT.transform(p);

			for(PxU32 j=0;j<nbAxes;j++)
			{
				const float dp = dot2D(dirs[j], p2d);
				dpmin[j] = intrinsics::selectMin(dpmin[j], dp);
				dpmax[j] = intrinsics::selectMax(dpmax[j], dp);
			}
		}

		PxU32 bestAxis = 0;
		float maxVariance = dpmax[0] - dpmin[0];
		for(PxU32 i=1;i<nbAxes;i++)
		{
			const float variance = dpmax[i] - dpmin[i];
			if(variance>maxVariance)
			{
				maxVariance = variance;
				bestAxis = i;
			}
		}

		const PxVec3 u = dirs[bestAxis];
		const PxVec3 v = PxVec3(-u.y, u.x, 0.0f);
		// PxVec3(1.0f, 0.0f, 0.0f)		=> PxVec3(0.0f, 1.0f, 0.0f)
		// PxVec3(0.0f, 1.0f, 0.0f)		=> PxVec3(-1.0f, 0.0f, 0.0f)
		// PxVec3(-1.0f, 1.0f, 0.0f)	=> PxVec3(-1.0f, -1.0f, 0.0f)
		// PxVec3(1.0f, 1.0f, 0.0f)		=> PxVec3(-1.0f, 1.0f, 0.0f)

		float dpminu = PX_MAX_F32;
		float dpmaxu = -PX_MAX_F32;
		float dpminv = PX_MAX_F32;
		float dpmaxv = -PX_MAX_F32;
		PxU32 indexMinU = 0;
		PxU32 indexMaxU = 0;
		PxU32 indexMinV = 0;
		PxU32 indexMaxV = 0;

		for(PxU32 i=0;i<nbNewContacts;i++)
		{
			const PxVec3& p = vertices[indices[i]];

			// Transform to 2D
			const PxVec3 p2d = rotT.transform(p);

			const float dpu = dot2D(u, p2d);
			const float dpv = dot2D(v, p2d);

			if(dpu<dpminu)
			{
				dpminu=dpu;
				indexMinU = i;
			}
			if(dpu>dpmaxu)
			{
				dpmaxu=dpu;
				indexMaxU = i;
			}

			if(dpv<dpminv)
			{
				dpminv=dpv;
				indexMinV = i;
			}
			if(dpv>dpmaxv)
			{
				dpmaxv=dpv;
				indexMaxV = i;
			}
		}

		if(indexMaxU == indexMinU)
			indexMaxU = 0xffffffff;
		if(indexMinV == indexMinU || indexMinV == indexMaxU)
			indexMinV = 0xffffffff;
		if(indexMaxV == indexMinU || indexMaxV == indexMaxU || indexMaxV == indexMinV)
			indexMaxV = 0xffffffff;
		
		PxU32 newCount = 0;
		for(PxU32 i=0;i<nbNewContacts;i++)
		{
			if(		i==indexMinU
				||	i==indexMaxU
				||	i==indexMinV
				||	i==indexMaxV)
			{
				ctcs[newCount++] = ctcs[i];
			}
		}
		contactBuffer.count = nbExistingContacts + newCount;
	}
}
#endif

// PT: please leave that function in the same translation unit as the calling code
inline PxMat33 findRotationMatrixFromZ(const PxVec3& to)
{
	PxMat33 result;

	const PxReal e = to.z;
	const PxReal f = PxAbs(e);

	if(f <= 0.9999f)
	{
		// PT: please keep the normal case first for PS3 branch prediction

		// Normal case, to and from are not parallel or anti-parallel
		const PxVec3 v = Ps::cross001(to);
		const PxReal h = 1.0f/(1.0f + e); /* optimization by Gottfried Chen */
		const PxReal hvx = h * v.x;
		const PxReal hvz = h * v.z;
		const PxReal hvxy = hvx * v.y;
		const PxReal hvxz = hvx * v.z;
		const PxReal hvyz = hvz * v.y;

		result(0,0) = e + hvx*v.x;
		result(0,1) = hvxy - v.z;
		result(0,2) = hvxz + v.y;

		result(1,0) = hvxy + v.z;
		result(1,1) = e + h*v.y*v.y;
		result(1,2) = hvyz - v.x;

		result(2,0) = hvxz - v.y;
		result(2,1) = hvyz + v.x;
		result(2,2) = e + hvz*v.z;
	}
	else
	{
		//Vectors almost parallel
		// PT: TODO: simplify code below
		PxVec3 from(0.0f, 0.0f, 1.0f);
		PxVec3 absFrom(0.0f, 0.0f, 1.0f);

		if(absFrom.x < absFrom.y)
		{
			if(absFrom.x < absFrom.z)
				absFrom = PxVec3(1.0f, 0.0f, 0.0f);
			else
				absFrom = PxVec3(0.0f, 0.0f, 1.0f);
		}
		else
		{
			if(absFrom.y < absFrom.z)
				absFrom = PxVec3(0.0f, 1.0f, 0.0f);
			else
				absFrom = PxVec3(0.0f, 0.0f, 1.0f);
		}

		PxVec3 u, v;
		u.x = absFrom.x - from.x; u.y = absFrom.y - from.y; u.z = absFrom.z - from.z;
		v.x = absFrom.x - to.x; v.y = absFrom.y - to.y; v.z = absFrom.z - to.z;

		const PxReal c1 = 2.0f / u.dot(u);
		const PxReal c2 = 2.0f / v.dot(v);
		const PxReal c3 = c1 * c2 * u.dot(v);

		for(unsigned int i = 0; i < 3; i++)
		{
			for(unsigned int j = 0; j < 3; j++)
			{
				result(i,j) = - c1*u[i]*u[j] - c2*v[i]*v[j] + c3*v[i]*u[j];
			}
			result(i,i) += 1.0f;
		}
	}
	return result;
}

// Experimental depth projection: original x86 mulss/addss round separately.
// Keep this local to polygon projection; never change global compiler settings.
namespace nativeprojection {
inline float store(float v){volatile float out=v;return out;}
inline float mul(float a,float b){return store(a*b);}
inline float add(float a,float b){return store(a+b);}
inline bool enabled(){const char* flag=std::getenv("RRR3D_EXPERIMENT_NATIVE_POLYGON_DEPTH");return flag && std::strcmp(flag,"1")==0;}
inline float dot(PxVec3 a,PxVec3 b){return add(add(mul(a.x,b.x),mul(a.y,b.y)),mul(a.z,b.z));}
inline PxVec3 transpose(const PxMat33& a,PxVec3 b){return {dot(a.column0,b),dot(a.column1,b),dot(a.column2,b)};}
inline Cm::Matrix34 flatten(const PxMat33& a,const Cm::Matrix34& b){
 PxVec3 p=transpose(a,b.p);
 // Native Matrix34 multiply evaluates translation Y in Y + X + Z order.
 p.y=add(add(mul(a.column1.y,b.p.y),mul(a.column1.x,b.p.x)),mul(a.column1.z,b.p.z));
 return {transpose(a,b.m.column0),transpose(a,b.m.column1),transpose(a,b.m.column2),p};
}
inline float z(PxVec3 src,const Cm::Matrix34& mat){return add(dot(src,{mat.m.column0.z,mat.m.column1.z,mat.m.column2.z}),mat.p.z);}
}

// PT: using this specialized version avoids doing an explicit transpose, which reduces LHS
PX_FORCE_INLINE Cm::Matrix34 transformTranspose(const PxMat33& a, const Cm::Matrix34& b)
{
	if(nativeprojection::enabled())return nativeprojection::flatten(a,b);
	return Cm::Matrix34(a.transformTranspose(b.m.column0), a.transformTranspose(b.m.column1), a.transformTranspose(b.m.column2), a.transformTranspose(b.p));
}

// Helper function to transform x/y coordinate of point. 
PX_FORCE_INLINE void transform2D(float& x, float& y, const PxVec3& src, const Cm::Matrix34& mat)
{
	x = src.x * mat.m.column0.x + src.y * mat.m.column1.x + src.z * mat.m.column2.x + mat.p.x;
	y = src.x * mat.m.column0.y + src.y * mat.m.column1.y + src.z * mat.m.column2.y + mat.p.y;
}

// Helper function to transform x/y coordinate of point. Use transposed matrix
PX_FORCE_INLINE void transform2DT(float& x, float& y, const PxVec3& src, const PxMat33& mat)
{
	x = mat.column0.dot(src);
	y = mat.column1.dot(src);
}

// Helper function to transform z coordinate of point.
PX_FORCE_INLINE PxReal transformZ(const PxVec3& src, const Cm::Matrix34& mat)
{
	if(nativeprojection::enabled())return nativeprojection::z(src,mat);
	return src.x * mat.m.column0.z + src.y * mat.m.column1.z + src.z * mat.m.column2.z + mat.p.z;
}

static void transformVertices(	float& minX, float& minY,
								float& maxX, float& maxY,
								float* PX_RESTRICT verts2D,
								PxU32 nb, const PxVec3* PX_RESTRICT vertices, const PxU8* PX_RESTRICT indices, const PxMat33& RotT)
{
	// PT: using local variables is important to reduce LHS.
	float lminX = FLT_MAX;
	float lminY = FLT_MAX;
	float lmaxX = -FLT_MAX;
	float lmaxY = -FLT_MAX;

	// PT: project points, compute min & max at the same time
	for(PxU32 i=0; i<nb; i++)
	{
		float x,y;
		transform2DT(x, y, vertices[indices[i]], RotT);
		lminX = intrinsics::selectMin(lminX, x);
		lminY = intrinsics::selectMin(lminY, y);
		lmaxX = intrinsics::selectMax(lmaxX, x);
		lmaxY = intrinsics::selectMax(lmaxY, y);
		verts2D[i*2+0] = x;
		verts2D[i*2+1] = y;
	}

	// DE702
		// Compute center of polygon
		const float cx = (lminX + lmaxX)*0.5f;
		const float cy = (lminY + lmaxY)*0.5f;
		// We'll scale the polygon by epsilon
		const float epsilon = 1.e-6f;
		// Adjust bounds to take care of scaling
		lminX -= epsilon;
		lminY -= epsilon;
		lmaxX += epsilon;
		lmaxY += epsilon;
	//~DE702

	// PT: relocate polygon to positive quadrant
	for(PxU32 i=0; i<nb; i++)
	{
		const float x = verts2D[i*2+0];
		const float y = verts2D[i*2+1];

		// PT: original code suffering from DE702 (relocation)
//		verts2D[i*2+0] = x - lminX;
//		verts2D[i*2+1] = y - lminY;

		// PT: theoretically proper DE702 fix (relocation + scaling)
		const float dx = x - cx;
		const float dy = y - cy;
//		const float coeff = epsilon * intrinsics::recipSqrt(dx*dx+dy*dy);
//		verts2D[i*2+0] = x - lminX + dx * coeff;
//		verts2D[i*2+1] = y - lminY + dy * coeff;

		// PT: approximate but faster DE702 fix. We multiply by epsilon so this is good enough.
		verts2D[i*2+0] = x - lminX + intrinsics::fsel(dx, epsilon, -epsilon);
		verts2D[i*2+1] = y - lminY + intrinsics::fsel(dy, epsilon, -epsilon);
	}
	lmaxX -= lminX;
	lmaxY -= lminY;

	minX = lminX;
	minY = lminY;
	maxX = lmaxX;
	maxY = lmaxY;
}

//! Dedicated triangle version
PX_FORCE_INLINE bool pointInTriangle2D(	float px, float pz,
										float p0x, float p0z,
										float e10x, float e10z,
										float e20x, float e20z)
{ 
	const float a = e10x*e10x + e10z*e10z;
	const float b = e10x*e20x + e10z*e20z;
	const float c = e20x*e20x + e20z*e20z;
	const float ac_bb = (a*c)-(b*b);

	const float vpx = px - p0x;
	const float vpz = pz - p0z;

	const float d = vpx*e10x + vpz*e10z;
	const float e = vpx*e20x + vpz*e20z;

	const float x = (d*c) - (e*b);
	const float y = (e*a) - (d*b);
	const float z = x + y - ac_bb;

	// Same as: if(x>0.0f && y>0.0f && z<0.0f)	return TRUE;
	//			else							return FALSE;
//		return (( IR(z) & ~(IR(x)|IR(y)) ) & SIGN_BITMASK) != 0;
	if(x>0.0f && y>0.0f && z<0.0f)	return true;
	else							return false;
}


	enum OutCode
	{
		OUT_XP	= (1<<0),
		OUT_XN	= (1<<1),
		OUT_YP	= (1<<2),
		OUT_YN	= (1<<3)
	};

static
//PX_FORCE_INLINE
bool PointInConvexPolygon2D_OutCodes(const float* PX_RESTRICT pgon2D, PxU32 numVerts, const PxReal tx, const PxReal ty, const PxReal maxX, const PxReal maxY, PxU8& outCodes)
{
	PxU32 out = 0;
	if(tx<0.0f)	out |= OUT_XN;
	if(ty<0.0f)	out |= OUT_YN;
	if(tx>maxX)	out |= OUT_XP;
	if(ty>maxY)	out |= OUT_YP;
	outCodes = PxU8(out);
	if(out)
		return false;

	if(numVerts==3)
		return pointInTriangle2D(	tx, ty,
									pgon2D[0], pgon2D[1],
									pgon2D[2] - pgon2D[0],
									pgon2D[3] - pgon2D[1],
									pgon2D[4] - pgon2D[0],
									pgon2D[5] - pgon2D[1]);

#define X 0
#define Y 1

	const PxReal* PX_RESTRICT vtx0_ = pgon2D + (numVerts-1)*2;
	const PxReal* PX_RESTRICT vtx1_ = pgon2D;

	const int* PX_RESTRICT ivtx0 = reinterpret_cast<const int*>(vtx0_);
	const int* PX_RESTRICT ivtx1 = reinterpret_cast<const int*>(vtx1_);
	//const int itx = (int&)tx;
	//const int ity = (int&)ty;
//	const int ity = PX_SIR(ty);
		const int* tmp = reinterpret_cast<const int*>(&ty);
		const int ity = *tmp;

	// get test bit for above/below X axis
	int yflag0 = ivtx0[Y] >= ity;

	int InsideFlag = 0;

	while(numVerts--)
	{
		const int yflag1 = ivtx1[Y] >= ity;
		if(yflag0 != yflag1)
		{
			const PxReal* PX_RESTRICT vtx0 = reinterpret_cast<const PxReal*>(ivtx0);
			const PxReal* PX_RESTRICT vtx1 = reinterpret_cast<const PxReal*>(ivtx1);
			if( ((vtx1[Y]-ty) * (vtx0[X]-vtx1[X]) > (vtx1[X]-tx) * (vtx0[Y]-vtx1[Y])) == yflag1 )
			{
				if(InsideFlag == 1) return false;

				InsideFlag++;
			}
		}
		yflag0 = yflag1;
		ivtx0 = ivtx1;
		ivtx1 += 2;
	}
#undef X
#undef Y

	return InsideFlag & 1;
}

// v34 candidate: observed Native edge path normalizes plane normal and the
// colliding segment before division. Each SSE scalar primitive stores float32;
// reciprocal sqrt uses an extended intermediate as in the x87 source path.
// No recorded coordinates or case identifiers are used. Tilted parity pending.
namespace nativeedge {
struct StageTrace {PxVec3 v1,p1,p2,dir,p3,p4,planeNormal,normalizedDirection,before,after;float planeConstant,coefficient,d3,denominator,ratio,distance;unsigned i,j;bool accepted;};
inline thread_local bool stageTraceEnabled=false;
inline thread_local std::vector<StageTrace> stageTraceRecords;
inline float store(double a){volatile float r=static_cast<float>(a);return r;}
inline float add(float a,float b){return store(double(a)+b);}
inline float sub(float a,float b){return store(double(a)-b);}
inline float mul(float a,float b){return store(double(a)*b);}
inline float divide(float a,float b){return store(double(a)/b);}
inline float dot(PxVec3 a,PxVec3 b){return add(add(mul(a.y,b.y),mul(a.z,b.z)),mul(a.x,b.x));}
inline PxVec3 unit(PxVec3 a){
 const float length2=add(add(mul(a.x,a.x),mul(a.z,a.z)),mul(a.y,a.y));
 if(!(length2>0))return PxVec3(0);
 const double inverse=1.0/std::sqrt(double(length2));
 return {store(double(a.x)*inverse),store(double(a.y)*inverse),store(double(a.z)*inverse)};
}
inline bool intersect(PxVec3 v1,const PxPlane& plane,PxVec3 p1,PxVec3 p2,PxVec3 dir,PxVec3 p3,PxVec3 p4,float& distance,PxVec3& point,unsigned i,unsigned j,float coefficient){
 const float d3=add(dot(plane.n,p3),plane.d);
 if(mul(d3,add(dot(plane.n,p4),plane.d))>0)return false;
 const auto v2=unit({sub(p4.x,p3.x),sub(p4.y,p3.y),sub(p4.z,p3.z)});
 const float denominator=add(add(mul(plane.n.x,v2.x),mul(plane.n.z,v2.z)),mul(plane.n.y,v2.y));
 if(denominator==0)return false;
 const float ratio=divide(d3,denominator);
 for(int a=0;a<3;++a)point[a]=sub(p3[a],mul(v2[a],ratio));
 const auto before=point;
 distance=mul(sub(mul(v1[i],sub(point[j],p1[j])),mul(v1[j],sub(point[i],p1[i]))),coefficient);
 if(distance<0)return false;
 for(int a=0;a<3;++a)point[a]=sub(point[a],mul(distance,dir[a]));
 const float x=mul(sub(p1.x,point.x),sub(p2.x,point.x)),z=mul(sub(p1.z,point.z),sub(p2.z,point.z)),y=mul(sub(p1.y,point.y),sub(p2.y,point.y));
 const bool accepted=add(add(x,z),y)<0;
 if(stageTraceEnabled)stageTraceRecords.push_back({v1,p1,p2,dir,p3,p4,plane.n,v2,before,point,plane.d,coefficient,d3,denominator,ratio,distance,i,j,accepted});
 return accepted;
}
}

// Helper function to detect contact between two edges
PX_FORCE_INLINE bool EdgeEdgeContactSpecial(const PxVec3& v1, const PxPlane& plane, 
	const PxVec3& p1, const PxVec3& p2, const PxVec3& dir, const PxVec3& p3, const PxVec3& p4,
	PxReal& dist, PxVec3& ip, unsigned int i, unsigned int j, float coeff, bool nativeEdge=false)
{
 if(nativeEdge)return nativeedge::intersect(v1,plane,p1,p2,dir,p3,p4,dist,ip,i,j,coeff);
	const PxReal d3 = plane.distance(p3);
	PxReal temp = d3 * plane.distance(p4);
	if(temp > 0.0f)
		return false;

	// if colliding edge (p3,p4) and plane are parallel return no collision
	const PxVec3 v2 = (p4-p3);
	temp = plane.n.dot(v2);
	if(temp == 0.0f) // ### epsilon would be better
		return false;

	// compute intersection point of plane and colliding edge (p3,p4)
	ip = p3-v2*(d3/temp);

	// compute distance of intersection from line (ip, -dir) to line (p1,p2)
	dist =	(v1[i]*(ip[j]-p1[j])-v1[j]*(ip[i]-p1[i])) * coeff;
	if(dist < 0.0f)
		return false;

	// compute intersection point on edge (p1,p2) line
	ip -= dist*dir;

	// check if intersection point (ip) is between edge (p1,p2) vertices
	temp = (p1.x-ip.x)*(p2.x-ip.x)+(p1.y-ip.y)*(p2.y-ip.y)+(p1.z-ip.z)*(p2.z-ip.z);
	if(temp<0.0f)	
		return true;	// collision found

	return false; //no collision
}

//This one can also handle 2 vertex 'polygons' (useful for capsule surface segments) and can shift the results before contact generation.
inline bool contactPolygonPolygonExt(	PxU32 numVerts0, const PxVec3* vertices0, const PxU8* indices0,	//polygon 0
									const Cm::Matrix34& world0, const PxPlane& localPlane0,	//xform of polygon 0, plane of polygon
									const PxMat33& rotT0,
									//
									PxU32 numVerts1, const PxVec3* PX_RESTRICT vertices1, const PxU8* PX_RESTRICT indices1,	//polygon 1
									const Cm::Matrix34& world1, const PxPlane& localPlane1,	//xform of polygon 1, plane of polygon
									const PxMat33& rotT1,
								//
									const PxVec3& worldSepAxis,	//world normal of separating plane - this is the world space normal of polygon0!!
									const Cm::Matrix34& transform0to1, const Cm::Matrix34& transform1to0,	//transforms between polygons
									PxU32 /*polyIndex0*/, PxU32 polyIndex1,	//feature indices for contact callback
									ContactBuffer& contactBuffer,
									bool flipNormal, const PxVec3& posShift, PxReal sepShift, bool nativeEdge=false,bool vertexPlane=false)	// shape order, result shift
{
	const PxVec3 n = flipNormal ? -worldSepAxis : worldSepAxis;

	PX_ASSERT(indices0 != NULL && indices1 != NULL);

	// - optimize "from to" computation
	// - do the raycast case && EE tests in same space as 2D case...
	// - project all edges at the same time ?
	PxU32 NumIn = 0;
	bool status = false;

	void* PX_RESTRICT stackMemory;
	{
		const PxU32 maxNumVert = PxMax(numVerts0, numVerts1);
		stackMemory = PxAlloca(maxNumVert * sizeof(PxVec3));
	}

	const PxU32 size0 = numVerts0 * sizeof(bool);
	bool* PX_RESTRICT flags0 = reinterpret_cast<bool*>(PxAlloca(size0));
	PxU8* PX_RESTRICT outCodes0 = reinterpret_cast<PxU8*>(PxAlloca(size0));
//	Ps::memZero(flags0, size0);
//	Ps::memZero(outCodes0, size0);

	const PxU32 size1 = numVerts1 * sizeof(bool);
	bool* PX_RESTRICT flags1 = reinterpret_cast<bool*>(PxAlloca(size1));
	PxU8* PX_RESTRICT outCodes1 = reinterpret_cast<PxU8*>(PxAlloca(size1));
//	Ps::memZero(flags1, size1);
//	Ps::memZero(outCodes1, size1);

#ifdef CONTACT_REDUCTION
	// We want to do contact reduction on newly created contacts, not on all the already existing ones...
	PxU32 nbExistingContacts = contactBuffer.count;
	PxU32 nbCurrentContacts=0;
	PxU8 indices[ContactBuffer::MAX_CONTACTS];
#endif

	{
		//polygon 1
		float* PX_RESTRICT verts2D = NULL;
		float minX=0, minY=0;
		float maxX=0, maxY=0;

		const PxVec3 localDir = -world1.rotateTranspose(worldSepAxis);		//contactNormal in hull1 space
																		//that's redundant, its equal to -localPlane1.d
		const Cm::Matrix34 t0to2D = transformTranspose(rotT1, transform0to1);	//transform from hull0 to RotT

		PxReal dn = localDir.dot(localPlane1.n);					//if the contactNormal == +-(normal of poly0) is NOT orthogonal to poly1 ...this is just to protect the division below.

		// PT: TODO: if "numVerts1>2" we may skip more
		if (numVerts1 > 2											//no need to test whether we're 'inside' ignore capsule segments and points
//		if(!(-1E-7 < dn && dn < 1E-7))
		&& dn >= 1E-7f)	// PT: it should never be negative so this unique test is enough
		{
			dn = 1.0f / dn;
			const float ld1 = -localPlane1.d;						// PT: unavoidable "int-to-float" LHS here, so we only want to read it once!

			// Lazy-transform vertices
			if(!verts2D)
			{
				verts2D = reinterpret_cast<float*>(stackMemory);
				//Project points
				transformVertices(
					minX, minY,
					maxX, maxY,
					verts2D, numVerts1, vertices1, indices1, rotT1);
			}

			for(PxU32 i=0; i < numVerts0; i++)							//for all vertices of poly0
			{
				const PxVec3& p = vertices0[indices0[i]];
				const float p0_z = transformZ(p, t0to2D);							//transform ith vertex of poly0 to RotT

				const PxVec3 pIn1 = transform0to1.transform(p);		//transform vertex to hull1 space, in which we have the poly1 vertices.

				const PxReal dd = (p0_z - ld1) * dn;			//(p0_z + localPlane1.d) is the depth of the vertex behind the triangle measured along the triangle's normal.
																	//we convert this to being measured along the 'contact normal' using the division.

//				if(dd < 0.0f)										//if the penetrating vertex will have a penetration along the contact normal: 
//				PX_ASSERT(dd <= 0.0f);		// PT: dn is always positive, so dd is always negative
				{
					float px, py;
					transform2DT(px, py, pIn1 - dd*localDir, rotT1);	//project vertex into poly1 plane along CONTACT NORMAL - not the polygon's normal. 

					const bool res = PointInConvexPolygon2D_OutCodes(verts2D, numVerts1, px-minX, py-minY, maxX, maxY, outCodes0[i]);
					flags0[i] = res;
					if(res)
					{
						NumIn++;

						if(p0_z < ld1)
						{
							status = true;	// PT: keep this first to avoid an LHS when leaving the function

							ContactPoint* PX_RESTRICT ctc = contactBuffer.contact();
							if(ctc)
							{
#ifdef CONTACT_REDUCTION
								indices[nbCurrentContacts++] = indices0[i];
#endif
								ctc->normal				= n;
								ctc->point				= world0.transform(p) + (flipNormal ? posShift : PxVec3(0.0f));
								ctc->separation			= dd + sepShift;
								ctc->internalFaceIndex1	= polyIndex1;
							}
						}
					}
				}
			}
		}
		else
		{
			PxMemZero(flags0, size0);
			PxMemZero(outCodes0, size0);
		}

		if(NumIn == numVerts0)
		{
			//All vertices0 are inside polygon 1
#ifdef CONTACT_REDUCTION
			ContactReductionAllIn(contactBuffer, nbExistingContacts, NumIn, rotT0, vertices0, indices);
#endif
			return status;
		}

#ifdef CONTACT_REDUCTION
		ContactReductionAllIn(contactBuffer, nbExistingContacts, NumIn, rotT0, vertices0, indices);
#endif

#ifdef CONTACT_REDUCTION
		nbExistingContacts = contactBuffer.count;
		nbCurrentContacts = 0;
#endif
		NumIn = 0;
		verts2D = NULL;

		//Polygon 0
		const Cm::Matrix34 t1to2D = transformTranspose(rotT0, transform1to0);

		if (numVerts0 > 2)											//no need to test whether we're 'inside' ignore capsule segments and points
		{
		const float ld0 = vertexPlane?nativeedge::dot(localPlane0.n,vertices0[indices0[0]]):-localPlane0.d;						// PT: unavoidable "int-to-float" LHS here, so we only want to read it once!

		// Lazy-transform vertices
		if(!verts2D)
		{
			verts2D = reinterpret_cast<float*>(stackMemory);
			//Project vertices
			transformVertices(
				minX, minY,
				maxX, maxY,
				verts2D, numVerts0, vertices0, indices0, rotT0);
		}

		for(PxU32 i=0; i < numVerts1; i++)
		{
			const PxVec3& p = vertices1[indices1[i]];

			float px, py;
			transform2D(px, py, p, t1to2D);

			const bool res = PointInConvexPolygon2D_OutCodes(verts2D, numVerts0, px-minX, py-minY, maxX, maxY, outCodes1[i]);
			flags1[i] = res;
			if(res)
			{
				NumIn++;

				const float pz = transformZ(p, t1to2D);
				if(pz < ld0)
				{
					status = true;	// PT: keep this first to avoid an LHS when leaving the function

					// PT: in theory, with this contact point we should use "worldSepAxis" as a contact normal.
					// However we want to output the same normal for all contact points not to break friction
					// patches!!! In theory again, it should be exactly the same since the contact point at
					// time of impact is supposed to be the same on both bodies. In practice however, and with
					// a depth-based engine, this is not the case. So the contact point here is not exactly
					// right, but preserving the friction patch seems more important.

					ContactPoint* PX_RESTRICT ctc = contactBuffer.contact();
					if(ctc)
					{
#ifdef CONTACT_REDUCTION
						indices[nbCurrentContacts++] = indices1[i];
#endif
						ctc->normal				= n;
						ctc->point				= world1.transform(p) + (flipNormal ? PxVec3(0.0f) : posShift);
						ctc->separation			= (pz - ld0) + sepShift;
						ctc->internalFaceIndex1	= polyIndex1;
					}
				}
			}
		}

		if(NumIn == numVerts1)
		{
			//all vertices 1 are inside polygon 0
#ifdef CONTACT_REDUCTION
			ContactReductionAllIn(contactBuffer, nbExistingContacts, NumIn, rotT1, vertices1, indices);
#endif
			return status;
		}
#ifdef CONTACT_REDUCTION
		ContactReductionAllIn(contactBuffer, nbExistingContacts, NumIn, rotT1, vertices1, indices);
#endif
		}
		else
		{
			PxMemZero(flags1, size1);
			PxMemZero(outCodes1, size1);
		}
	}

	//Edge/edge case
	//Calculation done in space 0
	PxVec3* PX_RESTRICT verts1in0 = reinterpret_cast<PxVec3*>(stackMemory);
	for(PxU32 i=0; i<numVerts1; i++)
	{
		verts1in0[i] = transform1to0.transform(vertices1[indices1[i]]);
	}

	if (numVerts0 >= 2 && numVerts1 >= 2)//useless if one of them is degenerate.
	for(PxU32 j=0; j<numVerts1; j++)
	{
		PxU32 j1 = j+1;
		if(j1 >= numVerts1) j1 = 0;

//		if(!(flags1[j] ^ flags1[j1]))
//			continue;
		if(flags1[j] && flags1[j1])
			continue;
		if(outCodes1[j]&outCodes1[j1])
			continue;

		const PxVec3& p0 = verts1in0[j];
		const PxVec3& p1 = verts1in0[j1];

//		gVisualizeLocalLine(vertices1[indices1[j]], vertices1[indices1[j1]], world1, callback.getManager());

		const PxVec3 v1 = p1-p0;
		PxVec3 planeNormal = v1.cross(localPlane0.n);
        if(nativeEdge)planeNormal=nativeedge::unit(planeNormal);
		const PxPlane plane(planeNormal, -(nativeEdge?nativeedge::dot(planeNormal,p0):planeNormal.dot(p0)));

		// find largest 2D plane projection
		PxU32 _i, _j;
		Ps::closestAxis(planeNormal, _i, _j);
        if(nativeEdge){_i=1;_j=2;if(std::abs(planeNormal.y)>std::abs(planeNormal.x)&&std::abs(planeNormal.y)>std::abs(planeNormal.z))_i=0;else if(std::abs(planeNormal.z)>std::abs(planeNormal.x)){_i=0;_j=1;}}

		const PxReal coeff = nativeEdge?nativeedge::divide(1.0f,nativeedge::sub(nativeedge::mul(v1[_i],localPlane0.n[_j]),nativeedge::mul(v1[_j],localPlane0.n[_i]))):1.0f / (v1[_i]*localPlane0.n[_j]-v1[_j]*localPlane0.n[_i]);

		for(PxU32 i=0; i<numVerts0; i++)
		{
			PxU32 i1 = i+1;
			if(i1 >= numVerts0) i1 = 0;

//			if(!(flags0[i] ^ flags0[i1]))
//				continue;
			if(flags0[i] && flags0[i1])
				continue;
			if(outCodes0[i]&outCodes0[i1])
				continue;

			const PxVec3& p0b = vertices0[indices0[i]];
			const PxVec3& p1b = vertices0[indices0[i1]];

//			gVisualizeLocalLine(p0b, p1b, world0, callback.getManager());

			PxReal dist;
			PxVec3 p;

			if(EdgeEdgeContactSpecial(v1, plane, p0, p1, localPlane0.n, p0b, p1b, dist, p, _i, _j, coeff, nativeEdge))
			{
				status = true;	// PT: keep this first to avoid an LHS when leaving the function
/*				p = world0.transform(p);

				//contacts are generated on the edges of polygon 1
				//we only have to shift the position of polygon 1 if flipNormal is false, because
				//in this case convex 0 gets passed as polygon 1, and it is convex 0 that was shifted.
				if (!flipNormal)
					p += posShift;

				contactBuffer.contact(p, n, -dist + sepShift, polyIndex0, polyIndex1, convexID);*/

				ContactPoint* PX_RESTRICT ctc = contactBuffer.contact();
				if(ctc)
				{
					ctc->normal				= n;
					ctc->point				= world0.transform(p) + (flipNormal ? PxVec3(0.0f) : posShift);
					ctc->separation			= -dist + sepShift;
					ctc->internalFaceIndex1	= polyIndex1;
				}
			}
		}
	}
	return status;
}

#undef PX_FORCE_INLINE
#undef PX_RESTRICT
#undef CONTACT_REDUCTION
#undef PX_MAX_F32
#undef PxAlloca
#undef PxMemZero
#undef PX_ASSERT

// v38: observed original triangle plane arithmetic (SSE stores, x87 normalization).
namespace nativeplane {
inline PxPlane triangle(PxVec3 p0,PxVec3 p1,PxVec3 p2) {
 using namespace nativeedge;
 const PxVec3 a={sub(p1.x,p0.x),sub(p1.y,p0.y),sub(p1.z,p0.z)};
 const PxVec3 b={sub(p2.x,p0.x),sub(p2.y,p0.y),sub(p2.z,p0.z)};
 const PxVec3 cross={sub(mul(a.y,b.z),mul(a.z,b.y)),sub(mul(a.z,b.x),mul(a.x,b.z)),sub(mul(a.x,b.y),mul(a.y,b.x))};
 const float length2=add(add(mul(cross.z,cross.z),mul(cross.y,cross.y)),mul(cross.x,cross.x));
 PxVec3 n(0);
 if(length2>0){const double inverse=1.0/std::sqrt(double(length2));n={store(double(cross.x)*inverse),store(double(cross.y)*inverse),store(double(cross.z)*inverse)};}
 const float distance=add(add(mul(n.z,p0.z),mul(n.y,p0.y)),mul(n.x,p0.x));
 return PxPlane(n,-distance);
}
}
inline PxPlane trianglePlane(const std::array<PxVec3,3>& vertices) {
 const char* flag=std::getenv("RRR3D_EXPERIMENT_NATIVE_TRIANGLE_PLANE");
 return flag && std::strcmp(flag,"1")==0?nativeplane::triangle(vertices[0],vertices[1],vertices[2]):PxPlane(vertices[0],vertices[1],vertices[2]);
}


// v60 investigation: finite triangle/box SAT, including hull and edge axes.
// A face-only triangle normal misses the source solver's hull-face contacts.
// Native PhysXCore box polygon table at RVA 0x312774. These are geometric
// corner indices for all six faces, independent of fixture, frame or pose.
namespace nativeboxfacesv97 {
inline bool enabled() { const char* flag=std::getenv("RRR3D_EXPERIMENT_NATIVE_BOX_FACE_VERTICES_V97");return flag && std::strcmp(flag,"1")==0; }
inline std::array<PxVec3,4> vertices(PxVec3 half,int axis,float sign) {
 const PxU8 faces[6][4]={{0,1,2,3},{1,5,6,2},{5,4,7,6},{4,0,3,7},{3,2,6,7},{4,5,1,0}};
 const int corners[8][3]={{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1}};
 const int face=axis==0?(sign>0?1:3):axis==1?(sign>0?4:5):(sign>0?2:0);
 std::array<PxVec3,4> result;
 for(unsigned i=0;i<4;++i)for(unsigned j=0;j<3;++j)result[i][j]=corners[faces[face][i]][j]<0?-half[j]:half[j];
 return result;
}
}

namespace boxtrianglesatv60 {
struct Choice { bool separated=false, triangleFace=true; PxVec3 axis; float depth=FLT_MAX; };
inline bool enabled() { const char* f=std::getenv("RRR3D_EXPERIMENT_NATIVE_BOX_TRIANGLE_SAT_V60"); return f && std::strcmp(f,"1")==0; }
inline bool interval(PxVec3 axis,PxVec3 half,const Cm::Matrix34& box,
                     const std::array<PxVec3,3>& triangle,float& depth) {
 const auto local=box.rotateTranspose(axis);
 const float radius=std::abs(local.x)*half.x+std::abs(local.y)*half.y+std::abs(local.z)*half.z;
 const float center=axis.dot(box.p);
 float lo=FLT_MAX,hi=-FLT_MAX;
 for(auto v:triangle){const float d=axis.dot(v);lo=std::min(lo,d);hi=std::max(hi,d);}
 if(center+radius<lo || hi<center-radius)return false;
 depth=std::min(center+radius-lo,hi-(center-radius));return true;
}
inline Choice select(PxVec3 half,const Cm::Matrix34& box,const std::array<PxVec3,3>& triangle,unsigned convexEdges=7) {
 Choice result;const PxPlane plane=trianglePlane(triangle);
 if(plane.distance(box.p)<0){result.separated=true;return result;}
 const auto center=(triangle[0]+triangle[1]+triangle[2])/3.0f;
 const auto witness=box.getInverseRT().transform(center);
 const std::array<PxVec3,3> columns{{box.m.column0,box.m.column1,box.m.column2}};
 bool found=false;float hullDepth=FLT_MAX;PxVec3 hullAxis;
 for(int backup=0;backup<2 && !found;++backup)for(unsigned axis=0;axis<3;++axis)for(int sign:{-1,1}) {
  if(!backup && sign*witness[axis]<half[axis])continue;
  float depth;const auto normal=float(sign)*columns[axis];
  if(!interval(normal,half,box,triangle,depth)){result.separated=true;return result;}
  found=true;if(depth<hullDepth){hullDepth=depth;hullAxis=normal;}
 }
 float triangleDepth;
 if(!interval(plane.n,half,box,triangle,triangleDepth)){result.separated=true;return result;}
 if(triangleDepth<hullDepth+0.0001f){result.depth=triangleDepth;result.axis=plane.n;}
 else {result.depth=hullDepth;result.axis=hullAxis;result.triangleFace=false;}
 // All nine edge cross axes are also separating tests; no face-only fallback.
 for(unsigned edge=0;edge<3;++edge)for(auto boxAxis:columns) {
  if(!(convexEdges&(1u<<edge)))continue;
  auto normal=boxAxis.cross(triangle[(edge+1)%3]-triangle[edge]);
  const float length2=normal.magnitudeSquared();if(length2<1.0e-12f)continue;
  normal=nativeedge::unit(normal);float depth;
  if(!interval(normal,half,box,triangle,depth)){result.separated=true;return result;}
  if(depth<result.depth-0.0001f){result.depth=depth;result.axis=normal;result.triangleFace=false;}
 }
 if((box.p-center).dot(result.axis)<0)result.axis=-result.axis;
 return result;
}
inline void hullContacts(ContactBuffer& out,PxVec3 half,const Cm::Matrix34& box,
                         const std::array<PxVec3,3>& triangle,unsigned index,bool nativeEdge,const Choice& choice,const Cm::Matrix34* nativeInverse=nullptr) {
 const auto local=box.rotateTranspose(-choice.axis);
 int axis=0;if(std::abs(local.y)>std::abs(local.x))axis=1;if(std::abs(local.z)>std::abs(local[axis]))axis=2;
 const int sign=local[axis]<0?-1:1;int a=axis==0?1:0,b=axis==2?1:2;
 const int signs[4][2]={{-1,-1},{1,-1},{1,1},{-1,1}};
 bool reverse=axis==0||(axis!=1)==(sign>0);
 const char* upper=std::getenv("RRR3D_EXPERIMENT_UPPER_FACE_WINDING");
 if(upper && std::strcmp(upper,"1")==0 && axis==2 && sign>0)reverse=!reverse;
 const char* nx=std::getenv("RRR3D_EXPERIMENT_NEGATIVE_X_FACE_ORDER");
 const bool negativeX=nx && std::strcmp(nx,"1")==0 && axis==0 && sign<0;
 const char* pz=std::getenv("RRR3D_EXPERIMENT_NATIVE_POSITIVE_Z_FACE_ORDER");
 const bool positiveZ=pz && std::strcmp(pz,"1")==0 && axis==2 && sign>0;
 std::array<PxVec3,4> verts;
 for(int i=0;i<4;++i){verts[i][axis]=sign*half[axis];const int j=positiveZ?(5-i)%4:negativeX?(3+i)%4:reverse?(4-i)%4:i;verts[i][a]=signs[j][0]*half[a];verts[i][b]=signs[j][1]*half[b];}
 if(nativeboxfacesv97::enabled())verts=nativeboxfacesv97::vertices(half,axis,sign);
 PxVec3 normal(0);normal[axis]=float(sign);const PxPlane face(normal,-half[axis]),plane=trianglePlane(triangle);
 const Cm::Matrix34 world;const PxU8 bi[4]={0,1,2,3},ti[3]={0,1,2};
 const unsigned before=out.count;
 contactPolygonPolygonExt(4,verts.data(),bi,box,face,findRotationMatrixFromZ(face.n),
                         3,triangle.data(),ti,world,plane,findRotationMatrixFromZ(plane.n),
                         -box.rotate(face.n),box,nativeInverse?*nativeInverse:box.getInverseRT(),0,index,out,true,PxVec3(0),0,nativeEdge);
 for(unsigned i=before;i<out.count;++i)out.contacts[i].nativeHullReference=true;
}
}

// Experimental face-contact wrapper. SAT/backface validation is supplied by
// the caller; edge-axis contacts require a separate generic convex wrapper.
inline void faceContacts(ContactBuffer& out,PxVec3 half,const Cm::Matrix34& box,
                         const std::array<PxVec3,3>& triangle,unsigned index=0,bool nativeEdge=false,unsigned convexEdges=7,const Cm::Matrix34* nativeInverse=nullptr,const PxPlane* patchPlane=nullptr)
{
 if(nativeEdge && boxtrianglesatv60::enabled()) {
  const auto choice=boxtrianglesatv60::select(half,box,triangle,convexEdges);
  if(choice.separated)return;
  if(!choice.triangleFace) {
   // The winning SAT axis and the polygon used as reference are separate
   // choices. A near-parallel edge axis may still require triangle reference.
   const auto local=box.rotateTranspose(-choice.axis);
   int axis=0;if(std::abs(local.y)>std::abs(local.x))axis=1;if(std::abs(local.z)>std::abs(local[axis]))axis=2;
   PxVec3 faceNormal(0);faceNormal[axis]=local[axis]<0?-1.0f:1.0f;
   const float hullAlignment=std::abs(box.rotate(faceNormal).dot(choice.axis));
   const float triangleAlignment=std::abs(trianglePlane(triangle).n.dot(choice.axis));
   if(hullAlignment>triangleAlignment){boxtrianglesatv60::hullContacts(out,half,box,triangle,index,nativeEdge,choice,nativeInverse);return;}
  }
 }
 const PxPlane plane=patchPlane?*patchPlane:trianglePlane(triangle);
 if(plane.distance(box.p)<0.0f)return;
 const PxVec3 local=box.rotateTranspose(-plane.n);
 const float support=std::abs(local.x)*half.x+std::abs(local.y)*half.y+std::abs(local.z)*half.z;
 if(plane.distance(box.p)>support)return;
 int axis=0;if(std::abs(local.y)>std::abs(local.x))axis=1;if(std::abs(local.z)>std::abs(local[axis]))axis=2;
 const float sign=local[axis]<0?-1.0f:1.0f;
 int a=(axis==0?1:0),b=(axis==2?1:2);
 const int signs[4][2]={{-1,-1},{1,-1},{1,1},{-1,1}};
 bool reverseWinding=axis==0 || (axis!=1)==(sign>0);
 const char* upperFaceFlag=std::getenv("RRR3D_EXPERIMENT_UPPER_FACE_WINDING");
 if(upperFaceFlag && std::strcmp(upperFaceFlag,"1")==0 && axis==2 && sign>0)
  reverseWinding=!reverseWinding;
 const char* negativeXFlag=std::getenv("RRR3D_EXPERIMENT_NEGATIVE_X_FACE_ORDER");
 const bool negativeXOrder=negativeXFlag && std::strcmp(negativeXFlag,"1")==0 && axis==0 && sign<0;
 if(negativeXOrder)reverseWinding=!reverseWinding;
 const char* positiveZFlag=std::getenv("RRR3D_EXPERIMENT_NATIVE_POSITIVE_Z_FACE_ORDER");
 const bool positiveZOrder=positiveZFlag && std::strcmp(positiveZFlag,"1")==0 && axis==2 && sign>0;
 std::array<PxVec3,4> verts;
 for(int i=0;i<4;++i){verts[i][axis]=sign*half[axis];const int j=positiveZOrder?(5-i)%4:negativeXOrder?(3+i)%4:(reverseWinding?(4-i)%4:i);verts[i][a]=signs[j][0]*half[a];verts[i][b]=signs[j][1]*half[b];}
 if(nativeboxfacesv97::enabled())verts=nativeboxfacesv97::vertices(half,axis,sign);
 PxVec3 faceNormal(0);faceNormal[axis]=sign;const PxPlane face(faceNormal,-half[axis]);
 float minimum=FLT_MAX;
 for(PxVec3 v:verts)minimum=std::min(minimum,plane.distance(box.transform(v)));
 if(minimum>0.0f)return;
 const Cm::Matrix34 world;const auto toTriangle=box;const auto toBox=patchPlane&&nativeInverse?*nativeInverse:box.getInverseRT();
 const PxU8 boxIndices[4]={0,1,2,3},triIndices[3]={0,1,2};
 const auto r0=findRotationMatrixFromZ(face.n),r1=findRotationMatrixFromZ(plane.n);
 const auto worldFaceNormal=box.rotate(face.n);
 if(std::abs(worldFaceNormal.dot(plane.n))>1.0f)
  contactPolygonPolygonExt(4,verts.data(),boxIndices,box,face,r0,3,triangle.data(),triIndices,world,plane,r1,worldFaceNormal,toTriangle,toBox,0,index,out,true,PxVec3(0),0,nativeEdge,patchPlane!=nullptr);
 else
  contactPolygonPolygonExt(3,triangle.data(),triIndices,world,plane,r1,4,verts.data(),boxIndices,box,face,r0,plane.n,toBox,toTriangle,0,index,out,false,PxVec3(0),0,nativeEdge,patchPlane!=nullptr);
}
}
