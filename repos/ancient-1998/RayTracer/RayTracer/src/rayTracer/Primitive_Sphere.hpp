#ifndef _RayTracer_Primitive_Sphere_hpp
#define _RayTracer_Primitive_Sphere_hpp

#include "Primitive.hpp"

namespace rayTracer {

	class Primitive_Sphere : public Primitive {
	public:

		Primitive_Sphere();
		Primitive_Sphere(const float& x, const float& y, const float& z, const float& r) {

			mSphere.center.set(x, y, z);
			mSphere.radius = r;
			mRadiusSq = r * r;
		}

		Primitive_Sphere(const Vector3& pos, const float& r) {

			mSphere.center = pos;
			mSphere.radius = r;
			mRadiusSq = r * r;
		}

		inline const float& getRadius() const { return mSphere.radius; }
		inline const float& getRadiusSq() const { return mRadiusSq; }
		inline Vector3& position() { return mSphere.center; }

		virtual const Vector3& getPosition() { return mSphere.center; }

		virtual bool intersects(const RayShot& rayShot, RayIntersectionT& intersection) {

			Vector3 dst;
			
			rayShot.ray->origin.subtract(position(), dst);
			float B = dst.dot(rayShot.ray->direction);
			float C = dst.dot(dst) - getRadiusSq();
			float D = B*B - C;

			if (D > 0.0f) {

				float sqrtD = sqrtf(D);

				return PrimitiveUtil::twoPointIntersectHelper(-B, sqrtD, rayShot.pOriginPrimitive == this, intersection);
			}

			return false;
		}

		virtual void resolveIntersectionInfo(const RayShot& rayShot, RayShotHit& hit) { 
												
			hit.intersInfo.point.subtract(position(), hit.intersInfo.normal);
			
			//if (WE::distanceSq(ray.origin, mPosition) < mRadiusSq) {
			//	intersection.normal.negate();
			//}

			hit.intersInfo.normal.normalize();
		}

		virtual void appendToVolume(AAB& volume) {

			volume.add(mSphere);
		}

		virtual PlanePartitionResult classify(AAPlane& plane, float planeThickness) {

			return plane.classify(mSphere);
		}

	protected:

		Sphere mSphere;
		float mRadiusSq;
	};

}

#endif