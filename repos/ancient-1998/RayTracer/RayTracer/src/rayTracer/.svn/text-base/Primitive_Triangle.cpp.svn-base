#include "Primitive_Triangle.hpp"

namespace rayTracer {

//Modified by jn
// Copyright 2001, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

// Assume that classes are already given for the objects:
//    Point and Vector with
//        coordinates {float x, y, z;}
//        operators for:
//            == to test equality
//            != to test inequality
//            (Vector)0 = (0,0,0)         (null vector)
//            Point  = Point ± Vector
//            Vector = Point - Point
//            Vector = Scalar * Vector    (scalar product)
//            Vector = Vector * Vector    (cross product)
//    Line and Ray and Segment with defining points {Point P0, P1;}
//        (a Line is infinite, Rays and Segments start at P0)
//        (a Ray extends beyond P1, but a Segment ends at P1)
//    Plane with a point and a normal {Point V0; Vector n;}
//    Triangle with defining vertices {Point V0, V1, V2;}
//    Polyline and Polygon with n vertices {int n; Point *V;}
//        (a Polygon has V[n]=V[0])
//===================================================================

//This solution yields a straightforward ray/segment-triangle intersection algorithm 
//(see our implementation: intersect_RayTriangle() ).  
//Based on a count of the operations done up to the first reject test, 
//this algorithm is not as efficient as the [Moller-Trumbore, 1997] (MT) algorithm, 
//although we have not yet done any runtime performance tests.  
//However, the MT algorithm uses two cross products whereas our algorithm uses only one, 
//and the one we use computes the normal vector of the triangle's plane.  
//Thus, when the normal vectors have been precomputed and stored for all triangles in a scene 
//(which is often the case), our algorithm would not compute a cross product at all, 
//making it even more efficient.  
//On the other hand, in this case, the MT algorithm would still compute two cross products, 
//and be less efficient than our algorithm.

inline float dot(const Vector3& u, const Vector3& v) {

	return u.dot(v);
}

// intersect_RayTriangle(): intersect a ray with a 3D triangle
//    Input:  a ray R, and a triangle T
//    Output: *I = intersection point (when it exists)
//    Return: -1 = triangle is degenerate (a segment or point)
//             0 = disjoint (no intersect)
//             1 = intersect in unique point I1
//             2 = are in the same plane
int
Primitive_Triangle::intersectRayTri_impl1(const Ray& ray, const Primitive_Triangle& T, float& t)
{
	static const float SMALL_NUM = 0.00000001f; // anything that avoids division overflow

	Vector3    u, v;             // triangle vectors
	Vector3    w0, w;          // ray vectors
	float     r, a, b;             // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = T.V(1) - T.V(0);
	v = T.V(2) - T.V(0);
	
	//n = u * v;             // cross product
	//u.cross(v, n);
	const Vector3& n = T.normal;
	
	if (n == Vector3::kZero)            // triangle is degenerate
		return -1;                 // do not deal with this case

	const Vector3& dir = ray.direction;             // ray direction vector
	w0 = ray.origin - T.V(0);
	a = -dot(n,w0);
	b = dot(n,dir);
	if (fabs(b) < SMALL_NUM) {     // ray is parallel to triangle plane
		if (a == 0)                // ray lies in triangle plane
			return 2;
		else return 0;             // ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0)                   // ray goes away from triangle
		return 0;                  // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect

	Vector3 I;

	t = r;
	I = ray.origin + dir * r;           // intersect point of ray and plane

	// is I inside T?
	float    uu, uv, vv, wu, wv, D;
	uu = dot(u,u);
	uv = dot(u,v);
	vv = dot(v,v);
	w = I - T.V(0);
	wu = dot(w,u);
	wv = dot(w,v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float ps, pt;
	ps = (uv * wv - vv * wu) / D;
	if (ps < 0.0 || ps > 1.0)        // I is outside T
		return 0;
	pt = (uv * wu - uu * wv) / D;
	if (pt < 0.0 || (ps + pt) > 1.0)  // I is outside T
		return 0;

	return 1;                      // I is in T
}

//Modified by jn
/* Ray-Triangle Intersection Test Routines          */
/* Different optimizations of my and Ben Trumbore's */
/* code from journals of graphics tools (JGT)       */
/* http://www.acm.org/jgt/                          */
/* by Tomas Moller, May 2000                        */
/* P.S: Modified for the Witch Engine */

#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

/* the original jgt code */
int Primitive_Triangle::intersectRayTri_impl2_v1(const float orig[3], const float dir[3],
		       const float vert0[3], const float vert1[3], const float vert2[3],
		       float *t, float *u, float *v)
{
   
	const float EPSILON = 0.000001f;

   float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   float det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   if (det > -EPSILON && det < EPSILON)
     return 0;
   inv_det = 1.0f / det;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   *u = DOT(tvec, pvec) * inv_det;
   if (*u < 0.0 || *u > 1.0)
     return 0;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, edge1);

   /* calculate V parameter and test bounds */
   *v = DOT(dir, qvec) * inv_det;
   if (*v < 0.0 || *u + *v > 1.0)
     return 0;

   /* calculate t, ray intersects triangle */
   *t = DOT(edge2, qvec) * inv_det;

   return 1;
}


/* code rewritten to do tests on the sign of the determinant */
/* the division is at the end in the code                    */
int Primitive_Triangle::intersectRayTri_impl2_v2(const float orig[3], const float dir[3],
			const float vert0[3], const float vert1[3], const float vert2[3],
			float *t, float *u, float *v)
{

	const float EPSILON = 0.000001f;

   float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   float det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   if (det > EPSILON)
   {
      /* calculate distance from vert0 to ray origin */
      SUB(tvec, orig, vert0);
      
      /* calculate U parameter and test bounds */
      *u = DOT(tvec, pvec);
      if (*u < 0.0 || *u > det)
	 return 0;
      
      /* prepare to test V parameter */
      CROSS(qvec, tvec, edge1);
      
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec);
      if (*v < 0.0 || *u + *v > det)
	 return 0;
      
   }
   else if(det < -EPSILON)
   {
      /* calculate distance from vert0 to ray origin */
      SUB(tvec, orig, vert0);
      
      /* calculate U parameter and test bounds */
      *u = DOT(tvec, pvec);
/*      printf("*u=%f\n",(float)*u); */
/*      printf("det=%f\n",det); */
      if (*u > 0.0 || *u < det)
	 return 0;
      
      /* prepare to test V parameter */
      CROSS(qvec, tvec, edge1);
      
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec) ;
      if (*v > 0.0 || *u + *v < det)
	 return 0;
   }
   else return 0;  /* ray is parallell to the plane of the triangle */


   inv_det = 1.0f / det;

   /* calculate t, ray intersects triangle */
   *t = DOT(edge2, qvec) * inv_det;
   (*u) *= inv_det;
   (*v) *= inv_det;

   return 1;
}

/* code rewritten to do tests on the sign of the determinant */
/* the division is before the test of the sign of the det    */
int Primitive_Triangle::intersectRayTri_impl2_v3(const float orig[3], const float dir[3],
			const float vert0[3], const float vert1[3], const float vert2[3],
			float *t, float *u, float *v)
{
   
	const float EPSILON = 0.000001f;

	float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   float det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);
   inv_det = 1.0f / det;
   
   if (det > EPSILON)
   {
      /* calculate U parameter and test bounds */
      *u = DOT(tvec, pvec);
      if (*u < 0.0 || *u > det)
	 return 0;
      
      /* prepare to test V parameter */
      CROSS(qvec, tvec, edge1);
      
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec);
      if (*v < 0.0 || *u + *v > det)
	 return 0;
      
   }
   else if(det < -EPSILON)
   {
      /* calculate U parameter and test bounds */
      *u = DOT(tvec, pvec);
      if (*u > 0.0 || *u < det)
	 return 0;
      
      /* prepare to test V parameter */
      CROSS(qvec, tvec, edge1);
      
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec) ;
      if (*v > 0.0 || *u + *v < det)
	 return 0;
   }
   else return 0;  /* ray is parallell to the plane of the triangle */

   /* calculate t, ray intersects triangle */
   *t = DOT(edge2, qvec) * inv_det;
   (*u) *= inv_det;
   (*v) *= inv_det;

   return 1;
}

/* code rewritten to do tests on the sign of the determinant */
/* the division is before the test of the sign of the det    */
/* and one CROSS has been moved out from the if-else if-else */
int Primitive_Triangle::intersectRayTri_impl2_v4(const float orig[3], const float dir[3],
			const float vert0[3], const float vert1[3], const float vert2[3],
			float *t, float *u, float *v)
{
   
	const float EPSILON = 0.000001f;
	//const float EPSILON = 0.0f;

	float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   float det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);
   inv_det = 1.0f / det;
   
   CROSS(qvec, tvec, edge1);
      
   if (det > EPSILON)
   {
      *u = DOT(tvec, pvec);
      if (*u < 0.0 || *u > det)
	 return 0;
            
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec);
      if (*v < 0.0 || *u + *v > det)
	 return 0;
      
   }
   else if(det < -EPSILON)
   {
      /* calculate U parameter and test bounds */
      *u = DOT(tvec, pvec);
      if (*u > 0.0 || *u < det)
	 return 0;
      
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec) ;
      if (*v > 0.0 || *u + *v < det)
	 return 0;
   }
   else return 0;  /* ray is parallell to the plane of the triangle */

   *t = DOT(edge2, qvec) * inv_det;
   (*u) *= inv_det;
   (*v) *= inv_det;

   return 1;
}

//ALIGN(ALIGN_CACHELINE) 
static const unsigned int modulo_table[] = {0,1,2,0,1};

void Primitive_Triangle::update(TriAccel& acc, const Vector3& vert0, const Vector3& vert1, const Vector3& vert2) {

	{
		Vector3 b, c, N;
		int k;

		b = vert1 - vert0;
		c = vert2 - vert0;
		
		c.cross(b, N);

		if (fabs(N.x) > fabs(N.y))
			if (fabs(N.x) > fabs(N.z)) k = 0; /* X */ else k=2; /* Z */
		else
			if (fabs(N.y) > fabs(N.z)) k = 1; /* Y */ else k=2; /* Z */

		int u = modulo_table[k+1]; 
		int v = modulo_table[k+2];

		N.div(N.el[k]);
		acc.k = k;
		acc.n_u = N.el[u];
		acc.n_v = N.el[v];
		acc.n_d = vert0.dot(N);

		float div_b = 1.0f / (b.el[u]*c.el[v] - b.el[v]*c.el[u]);
		acc.b_nv = b.el[u] * div_b;
		acc.b_nu = -b.el[v] * div_b;
		acc.b_d = (b.el[v]*vert0.el[u] - b.el[u]*vert0.el[v]) * div_b;

		float div_c = 1.0f / (c.el[u]*b.el[v] - c.el[v]*b.el[u]);
		acc.c_nv = c.el[u] * div_c;
		acc.c_nu = -c.el[v] * div_c;
		acc.c_d = (c.el[v]*vert0.el[u] - c[u]*vert0.el[v]) * div_c;
	}
}

int Primitive_Triangle::intersectRayTri_impl_project(const TriAccel& acc, const Vector3& orig, const Vector3& dir,
			float *t, float *u, float *v)
{
	const float EPSILON = 0.000001f;

	#define ku modulo_table[acc.k+1]
	#define kv modulo_table[acc.k+2]
	
	// don’t prefetch here, assume data has already been prefetched
	// start high-latency division as early as possible
	const float nd_rec = (dir[acc.k] + acc.n_u * dir[ku] + acc.n_v * dir[kv]);
	const float nd = 1.0f/nd_rec;
	const float f = (acc.n_d - orig[acc.k] - acc.n_u * orig[ku] - acc.n_v * orig[kv]) * nd;
	
	// check for valid distance.
	if (!(*t > f && f > EPSILON )) return 0;
	
	// compute hitpoint positions on uv plane
	const float hu = (orig[ku] + f * dir[ku]);
	const float hv = (orig[kv] + f * dir[kv]);
	
	// check first barycentric coordinate
	const float lambda = (hu * acc.b_nu + hv * acc.b_nv + acc.b_d);
	if (lambda < 0.0f) return 0;
	
	// check second barycentric coordinate
	const float mue = (hu * acc.c_nu + hv * acc.c_nv + acc.c_d);
	if (mue < 0.0f) return 0;
	
	// check third barycentric coordinate
	if (lambda+mue > 1.0f) return 0;
	
	// have a valid hitpoint here. store it.
	*t = f;
	//hit.tri = triNum;
	*u = lambda;
	*v = mue;

	return 1;
}

#ifdef RayTracerConfig_EnablePackets

int Primitive_Triangle::intersectRayTri_impl_project(const Constants& cts, const PacketRayShot& rayPacket, PacketRayHit& t, const PacketRayMask& rayPacketMask, const unsigned int& primID_)
{
	const float EPSILON = 0.000001f;

	#define ku modulo_table[acc.k+1]
	#define kv modulo_table[acc.k+2]
	
	const TriAccel& acc = mAccel;

	static const __m128 eps = _mm_set_ps1(EPSILON);

	const __m128 acc_nu = _mm_set_ps1(acc.n_u);
	const __m128 acc_nv = _mm_set_ps1(acc.n_v);
	
	const __m128& dir_k = rayPacket.ray4_direction[acc.k];
	const __m128& dir_ku = rayPacket.ray4_direction[ku];
	const __m128& dir_kv = rayPacket.ray4_direction[kv];

	// don’t prefetch here, assume data has already been prefetched
	// start high-latency division as early as possible
	//const float nd = 1.0f/(dir[acc.k] + acc.n_u * dir[ku] + acc.n_v * dir[kv]);
	const __m128 nd_rec = (
							_mm_add_ps(
								dir_k
								,
								_mm_add_ps(
									_mm_mul_ps(acc_nu, dir_ku)
									,
									_mm_mul_ps(acc_nv, dir_kv)
								)
							)
						);
	
	//unprecise rec.
	//const __m128 nd = _mm_rcp_ps(nd_rec);
	
	//rec. with newton-raphon iter.
	//const __m128 nd = _mm_safercp(nd_rec);

	//this might be ok with new CPU's (E6850, radix-16 divider)
	const __m128 nd = _mm_div_ps(cts.one4, nd_rec);
						
		
	const __m128 acc_nd = _mm_set_ps1(acc.n_d);
	const __m128& orig_k = rayPacket.ray4_origin[acc.k];
	const __m128& orig_ku = rayPacket.ray4_origin[ku];
	const __m128& orig_kv = rayPacket.ray4_origin[kv];
		
	//const float f = (acc.n_d - orig[acc.k] - acc.n_u * orig[ku] - acc.n_v * orig[kv]) * nd;
	const __m128 f = _mm_mul_ps(
						_mm_sub_ps(
							_mm_sub_ps(
								_mm_sub_ps(acc_nd, orig_k), _mm_mul_ps(acc_nu, orig_ku)
							), _mm_mul_ps(acc_nv, orig_kv)
						), nd);


	// check for valid distance.
	//if (!(*t > f && f > EPSILON )) return 0;
	
	union { __m128 active_mask4; __m128i active_mask4i; };

	active_mask4 = _mm_and_ps(_mm_cmpgt_ps(f, eps), _mm_cmpgt_ps(t.t.el4, f));
	active_mask4 = _mm_and_ps(active_mask4, rayPacketMask.active.el4);

	int active_mask_bits = _mm_movemask_ps(active_mask4);
	
	if (active_mask_bits == 0) return 0;
	
	// compute hitpoint positions on uv plane
	//const float hu = (orig[ku] + f * dir[ku]);
	//const float hv = (orig[kv] + f * dir[kv]);

	const __m128 hu = _mm_add_ps(orig_ku, _mm_mul_ps(f, dir_ku));
	const __m128 hv = _mm_add_ps(orig_kv, _mm_mul_ps(f, dir_kv));

	// check first barycentric coordinate
	//const float lambda = (hu * acc.b_nu + hv * acc.b_nv + acc.b_d);
	
	const __m128 acc_b_nu = _mm_set_ps1(acc.b_nu);
	const __m128 acc_b_nv = _mm_set_ps1(acc.b_nv);
	const __m128 acc_b_d = _mm_set_ps1(acc.b_d);

	const __m128 alpha = _mm_add_ps(
								_mm_add_ps(_mm_mul_ps(hu, acc_b_nu), _mm_mul_ps(hv, acc_b_nv))
								, acc_b_d);
	
	//if (lambda < 0.0f) return 0;
	active_mask4 = _mm_and_ps(active_mask4, _mm_cmpge_ps(alpha, cts.zero4));
	active_mask_bits = _mm_movemask_ps(active_mask4);

	if (active_mask_bits == 0) return 0;
	
	// check second barycentric coordinate
	//const float mue = (hu * acc.c_nu + hv * acc.c_nv + acc.c_d);
	const __m128 acc_c_nu = _mm_set_ps1(acc.c_nu);
	const __m128 acc_c_nv = _mm_set_ps1(acc.c_nv);
	const __m128 acc_c_d = _mm_set_ps1(acc.c_d);

	const __m128 beta = _mm_add_ps(
								_mm_add_ps(_mm_mul_ps(hu, acc_c_nu), _mm_mul_ps(hv, acc_c_nv))
								, acc_c_d);
	
	//if (mue < 0.0f) return 0;
	active_mask4 = _mm_and_ps(active_mask4, _mm_cmpge_ps(beta, cts.zero4));
	active_mask_bits = _mm_movemask_ps(active_mask4);

	if (active_mask_bits == 0) return 0;

	// check third barycentric coordinate
	//if (lambda+mue > 1.0f) return 0;
	const __m128 gamma = _mm_add_ps(alpha, beta);

	active_mask4 = _mm_and_ps(active_mask4, _mm_cmple_ps(gamma, cts.one4));
	active_mask_bits = _mm_movemask_ps(active_mask4);

	if (active_mask_bits == 0) return 0;

	// have a valid hitpoint here. store it.
	//*t = f;
	t.t.el4 = _mm_or_ps(_mm_and_ps(active_mask4, f), _mm_andnot_ps(active_mask4, t.t.el4));

	//hit.tri = triNum;
	const __m128i primID = _mm_set1_epi32((int) primID_);
	t.pid.eli4 = _mm_or_si128(_mm_and_si128(active_mask4i, primID), _mm_andnot_si128(active_mask4i, t.pid.eli4));

	/*
	*u = lambda;
	*v = mue;
	*/

	return 1;
}

#endif

}