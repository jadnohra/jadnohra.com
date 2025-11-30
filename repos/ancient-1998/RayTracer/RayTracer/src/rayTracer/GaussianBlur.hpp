#ifndef _RayTracer_GaussianBlur_hpp
#define _RayTracer_GaussianBlur_hpp

#include "RayTracerTypes.hpp"
#include "RenderTarget.hpp"
#include "Clock.hpp"

namespace rayTracer {

	class GaussianBlur {
	public:

		static void transform(Clock& clock, RenderTarget& src, RenderTarget& dest);
	};
}

#endif