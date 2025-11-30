#ifndef _RayTracer_Primitive_hpp
#define _RayTracer_Primitive_hpp

#include "RayTracerTypes.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "../WE3/math/WEDistance.h"
#include "../WE3/math/WEAAB.h"
#include "../WE3/math/WEPlane.h"
#include "../WE3/math/WESphere.h"

namespace rayTracer {

	class PrimitiveUtil {
	public:

		static bool twoPointIntersectHelper_Naive(const float& offset, const float& det, const bool& isOwnSurfaceOrigin, RayIntersectionT& intersection) {

			float tMin = offset - det;

			if (tMin > 0.0f) {

				intersection.t = tMin;
				return true;
			}

			//reuse tMin for tMax
			tMin = offset + det;

			if (tMin > 0.0f) {

				intersection.t = tMin;
				return true;
			}

			return false;
		}

		static bool twoPointIntersectHelper_Naive(const float& offset, const float& det, const float& div, const bool& isOwnSurfaceOrigin, RayIntersectionT& intersection) {

			float tMin = (offset - det) / div;

			if (tMin > 0.0f) {

				intersection.t = tMin;
				return true;
			}

			//reuse tMin for tMax
			tMin = (offset + det) / div;

			if (tMin > 0.0f) {

				intersection.t = tMin;
				return true;
			}

			return false;
		}

		static bool twoPointIntersectHelper(const float& offset, const float& det, const bool& isOwnSurfaceOrigin, RayIntersectionT& intersection) {

			float tMin = offset - det;

			if (isOwnSurfaceOrigin) {

				float tMax = offset + det;

				bool isTMinPos = tMin > 0.0f;
				bool isTMaxPos = tMax > 0.0f;

				bool isOutside = (isTMinPos == isTMaxPos);

				if (isOutside) {

					if (isTMaxPos) {
				
						intersection.t = tMax;
						return true;
					} 

				} else {

					if (fabs(tMax) > fabs(tMin)) {

						intersection.t = tMax;
						return true;
					}
				}

			} else {

				if (tMin > 0.0f) {

					intersection.t = tMin;
					return true;
				}

				//reuse tMin for tMax
				tMin = offset + det;

				if (tMin > 0.0f) {

					intersection.t = tMin;
					return true;
				}
			}

			return false;
		}

		static bool twoPointIntersectHelper(const float& offset, const float& det, const float& div, const bool& isOwnSurfaceOrigin, RayIntersectionT& intersection) {

			float tMin = (offset - det) / div;

			if (isOwnSurfaceOrigin) {

				float tMax = (offset + det) / div;

				bool isTMinPos = tMin > 0.0f;
				bool isTMaxPos = tMax > 0.0f;

				bool isOutside = (isTMinPos == isTMaxPos);

				if (isOutside) {

					if (isTMaxPos) {
				
						intersection.t = tMax;
						return true;
					} 

				} else {

					if (fabs(tMax) > fabs(tMin)) {

						intersection.t = tMax;
						return true;
					}
				}

			} else {

				if (tMin > 0.0f) {

					intersection.t = tMin;
					return true;
				}

				//reuse tMin for tMax
				tMin = (offset + det) / div;

				if (tMin > 0.0f) {

					intersection.t = tMin;
					return true;
				}
			}

			return false;
		}
	};
	
}

#include "Primitive_Plane.hpp"
#include "Primitive_Sphere.hpp"
#include "Primitive_Quadric.hpp"
#include "Primitive_Cone.hpp"
#include "Primitive_Triangle.hpp"

#include "../WE3/WETL/WETLArray.h"

namespace rayTracer {

	typedef WETL::CountedPtrArrayEx<Primitive, size_t> Primitives;
	typedef WETL::CountedArray<Primitive*, false, size_t> SoftPrimitives;
}

#endif