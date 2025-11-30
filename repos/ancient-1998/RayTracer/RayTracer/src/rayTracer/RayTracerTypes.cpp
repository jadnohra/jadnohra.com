#include "RayTracerTypes.hpp"

namespace rayTracer {

const Constants Constants::kConstants;
#ifdef RayTracerConfig_EnablePackets

	const Constants::MaskInfo Constants::MaskBitInfoTable[16] = 
	{
		/*0000*/{-1,0}, /*0001*/{0,1}, /*0010*/{1,1}, /*0011*/{0,2}, 
		/*0100*/{2,1}, /*0101*/{0,2}, /*0110*/{1,2}, /*0111*/{0,3}, 
		/*1000*/{3,1}, /*1001*/{0,2}, /*1010*/{1,2}, /*1011*/{0,3}, 
		/*1100*/{2,2}, /*1101*/{0,3}, /*1110*/{1,3}, /*1111*/{0,3}, 
	};

	const PacketRayMask PacketRayMask::kFullMask(Constants::kConstants);

#endif

void Triangle::autoGenNormal(const Vertex* vertexArray, bool rotLeftHanded) {

	Vector3 temp[2];

	const Vector3& v0 = vertexArray[vertices[0]];
	const Vector3& v1 = vertexArray[vertices[1]];
	const Vector3& v2 = vertexArray[vertices[2]];

	v1.subtract(v0, temp[0]);
	v2.subtract(v0, temp[1]);

	temp[0].cross(temp[1], normal);
	normal.normalize();

	if (!rotLeftHanded)
		normal.negate();
}

}