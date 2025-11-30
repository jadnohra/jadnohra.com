#include "Math.hpp"

namespace rayTracer {

#ifdef RayTracerConfig_EnablePackets

void _mm_transform(const Matrix4x4& m, const Packet4_3& src, Packet4_3& dest, Packet4& wInOut) {

	dest.x4 = 
		_mm_add_ps(
			_mm_add_ps(_mm_mul_ps(src.x4, _mm_set1_ps(m.m11)), _mm_mul_ps(src.y4, _mm_set1_ps(m.m21)))
			, _mm_add_ps(_mm_mul_ps(src.z4, _mm_set1_ps(m.m31)), _mm_mul_ps(wInOut, _mm_set1_ps(m.m41)))
			);

	dest.y4 = 
		_mm_add_ps(
			_mm_add_ps(_mm_mul_ps(src.x4, _mm_set1_ps(m.m12)), _mm_mul_ps(src.y4, _mm_set1_ps(m.m22)))
			, _mm_add_ps(_mm_mul_ps(src.z4, _mm_set1_ps(m.m32)), _mm_mul_ps(wInOut, _mm_set1_ps(m.m42)))
			);

	dest.z4 = 
		_mm_add_ps(
			_mm_add_ps(_mm_mul_ps(src.x4, _mm_set1_ps(m.m13)), _mm_mul_ps(src.y4, _mm_set1_ps(m.m23)))
			, _mm_add_ps(_mm_mul_ps(src.z4, _mm_set1_ps(m.m33)), _mm_mul_ps(wInOut, _mm_set1_ps(m.m43)))
			);

	wInOut.el4 = 
		_mm_add_ps(
			_mm_add_ps(_mm_mul_ps(src.x4, _mm_set1_ps(m.m14)), _mm_mul_ps(src.y4, _mm_set1_ps(m.m24)))
			, _mm_add_ps(_mm_mul_ps(src.z4, _mm_set1_ps(m.m34)), _mm_mul_ps(wInOut, _mm_set1_ps(m.m44)))
			);
	
}

void _mm_transform(const Matrix4x3Base& m, const Packet4_3& src, Packet4_3& dest) {

	dest.x4 = 
		_mm_add_ps(
			_mm_add_ps(_mm_mul_ps(src.x4, _mm_set1_ps(m.m11)), _mm_mul_ps(src.y4, _mm_set1_ps(m.m21)))
			, _mm_add_ps(_mm_mul_ps(src.z4, _mm_set1_ps(m.m31)), _mm_set1_ps(m.tx))
			);

	dest.y4 = 
		_mm_add_ps(
			_mm_add_ps(_mm_mul_ps(src.x4, _mm_set1_ps(m.m12)), _mm_mul_ps(src.y4, _mm_set1_ps(m.m22)))
			, _mm_add_ps(_mm_mul_ps(src.z4, _mm_set1_ps(m.m32)), _mm_set1_ps(m.ty))
			);

	dest.z4 = 
		_mm_add_ps(
			_mm_add_ps(_mm_mul_ps(src.x4, _mm_set1_ps(m.m13)), _mm_mul_ps(src.y4, _mm_set1_ps(m.m23)))
			, _mm_add_ps(_mm_mul_ps(src.z4, _mm_set1_ps(m.m33)), _mm_set1_ps(m.tz))
			);
}

#endif

}