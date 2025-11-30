#ifndef _RayTracer_Primitive_Triangle_hpp
#define _RayTracer_Primitive_Triangle_hpp

#include "Primitive.hpp"
#include "../WE3/math/WETriangle.h"

#define RayTracerConfig_Tri_Proj
//#define RayTracerConfig_Tri_Proj_TryAlignCahe

namespace rayTracer {

	class Primitive_Triangle : public Primitive, public TriangleEx1 {
	protected:

		struct TriAccel;

	public:

		Primitive_Triangle();
		Primitive_Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3* n, bool rotLeftHanded = true) {

			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;

			if (n){

				normal = *n;

			} else {

				this->initNormal();

				if (!rotLeftHanded)
					normal.negate();
			}

			#ifdef RayTracerConfig_Tri_Proj
			update(mAccel, v0, v1, v2);
			#endif
		}
		
		virtual const Vector3& getPosition() { return V(0); }

		virtual bool intersects(const RayShot& rayShot, RayIntersectionT& intersection) {

			if (rayShot.pOriginPrimitive == this)
				return false;

			#ifdef RayTracerConfig_Tri_Proj
			//Optimized proj.
			{

				float u, v;

				return (intersectRayTri_impl_project(mAccel, rayShot.ray->origin, rayShot.ray->direction,
														&intersection.t, &u, &v) == 1);
			}
			#else
			//Barycentric
			{
				float u, v;

				return (intersectRayTri_impl2_v4(rayShot.ray->origin.el, rayShot.ray->direction.el,
												this->v(0).el, this->v(1).el, this->v(2).el,
												&intersection.t, &u, &v) == 1) && (intersection.t >= 0.0f);
			}
			#endif

			/*
			{
				return (intersectRayTri_impl1(ray, dref(this), intersection.t) == 1);
			}
			*/
		}

		virtual void resolveIntersectionInfo(const RayShot& rayShot, RayShotHit& hit) { 
												
			hit.intersInfo.normal = normal;
		}

		virtual void appendToVolume(AAB& volume) {

			volume.add(V(0));
			volume.add(V(1));
			volume.add(V(2));
		}

		virtual PlanePartitionResult classify(AAPlane& plane, float planeThickness) {

			return plane.classify(vertices, 3);
		}

		virtual PlanePartitionResult chooseClassify(AAPlane& plane, float planeThickness) {

			PlanePartitionResult ret = plane.classify(vertices, 3);

			if (ret == PP_BackAndFront) {

				Vector3 centroid;
				V(0).mul(0.333f, centroid);
				V(1).mulAndAdd(0.333f, centroid);
				V(2).mulAndAdd(0.333f, centroid);

				ret = plane.classify(centroid);

				if (ret == PP_BackAndFront)
					ret = PP_Back;
			}

			return ret;
		}

		#ifdef RayTracerConfig_EnablePackets

			int intersectRayTri_impl_project(const Constants& cts, const PacketRayShot& rayPacket, PacketRayHit& t, const PacketRayMask& rayPacketMask, const unsigned int& primID/*, float *u, float *v*/);

		#endif


	protected:

		//Return: -1 = triangle is degenerate (a segment or point)
		//0 = disjoint (no intersect)
		//1 = intersect in unique point I1
		//2 = are in the same plane
		static int intersectRayTri_impl1(const Ray& ray, const Primitive_Triangle& T, float& t);

		//1 = intersect
		static int intersectRayTri_impl2_v1(const float orig[3], const float dir[3],
		       const float vert0[3], const float vert1[3], const float vert2[3],
		       float *t, float *u, float *v);

		static int intersectRayTri_impl2_v2(const float orig[3], const float dir[3],
		       const float vert0[3], const float vert1[3], const float vert2[3],
		       float *t, float *u, float *v);

		static int intersectRayTri_impl2_v3(const float orig[3], const float dir[3],
		       const float vert0[3], const float vert1[3], const float vert2[3],
		       float *t, float *u, float *v);

		static int intersectRayTri_impl2_v4(const float orig[3], const float dir[3],
		       const float vert0[3], const float vert1[3], const float vert2[3],
		       float *t, float *u, float *v);

		static int intersectRayTri_impl_project(const TriAccel& acc, const Vector3& orig, const Vector3& dir,
				float *t, float *u, float *v);

	protected:

		struct TriAccel
		{
			// first 16 byte half cache line
			// plane:
			float n_u; //!< == normal.u / normal.k
			float n_v; //!< == normal.v / normal.k
			float n_d; //!< constant of plane equation
			int k; // projection dimension
			// second 16 byte half cache line
			// line equation for line ac
			float b_nu;
			float b_nv;
			float b_d;
			
			#ifdef RayTracerConfig_Tri_Proj_TryAlignCahe
			int pad1; // pad to next cache line
			#endif

			// third 16 byte half cache line
			// line equation for line ab
			float c_nu;
			float c_nv;
			float c_d;
			
			#ifdef RayTracerConfig_Tri_Proj_TryAlignCahe
			int pad2; // pad to 48 bytes for cache alignment purposes
			#endif
		};

		#ifdef RayTracerConfig_Tri_Proj
		
		TriAccel mAccel;

		void update(TriAccel& accel, const Vector3& vert0, const Vector3& vert1, const Vector3& vert2);

		#endif
	};
}

#endif