#ifndef _RayTracer_Primitive_Quad_hpp
#define _RayTracer_Primitive_Quad_hpp

#include "Primitive.hpp"


namespace rayTracer {

	class Primitive_Quad : public Primitive {
	public:

		Primitive_Quad();
		Primitive_Quad(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3* n, bool rotLeftHanded = true) {

			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
			vertices[3] = v3;

			if (n){

				normal = *n;

			} else {

				this->initNormal();

				if (!rotLeftHanded)
					normal.negate();
			}
		}
		
		virtual const Vector3& getPosition() { return V(0); }

		virtual bool intersects(const Ray& ray, RayIntersectionT& intersection, const RayTolerance& tolerance, bool isOwnSurfaceOrigin = false) {

			if (isOwnSurfaceOrigin)
				return false;

			assert(false);
			return false;
		}

		virtual void resolveIntersectionInfo(const Ray& ray, const RayIntersectionT& intersectionT, 
											const Vector3& intersectionPoint, 
											RayIntersection& intersection) { 
												
			intersection.normal = normal;
		}

	protected:

		Vector3 normal;
	};
}

#endif