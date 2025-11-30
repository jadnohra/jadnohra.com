#ifndef _RayTracer_Primitive_Plane_hpp
#define _RayTracer_Primitive_Plane_hpp

#include "Primitive.hpp"

namespace rayTracer {

	class Primitive_Plane : public Primitive {
	public:

		Primitive_Plane();
		Primitive_Plane(const float& x, const float& y, const float& z, 
						const float& nx, const float& ny, const float& nz) {

			point().set(x, y, z);
			normal().set(nx, ny, nz);
		}

		Primitive_Plane(const Vector3& pt, const Vector3& n) {

			point() = pt;
			normal() = n;
		}

		inline Vector3& point() { return mPoint; }
		inline Vector3& normal() { return mNormal; }

		virtual const Vector3& getPosition() { return point(); }

		virtual bool intersects(const RayShot& rayShot, RayIntersectionT& intersection) {

			if (rayShot.pOriginPrimitive == this)
				return false;

			float denom = mNormal.dot(rayShot.ray->direction);

			if (denom == 0.0f) {

				//parallel or contained (if numerator is zero)
				return false;
			}

			Vector3 diff;
			rayShot.ray->origin.subtract(mPoint, diff);
			float num = -mNormal.dot(diff);

			float t = num / denom;

			if (t > rayShot.rayTolerance->t) {

				intersection.t = t;
				return true;

			} else {

				//ray starts from plane
			}

			return false;
		}

		virtual void resolveIntersectionInfo(const RayShot& rayShot, RayShotHit& hit) { 
												
			hit.intersInfo.normal = mNormal;
		}

		virtual void appendToVolume(AAB& volume) {

			volume.add(mPoint);
		}

		virtual PlanePartitionResult classify(AAPlane& plane, float planeThickness) {

			return PP_BackAndFront;
		}

	protected:

		Vector3 mNormal;
		Vector3 mPoint;
	};
}

#endif