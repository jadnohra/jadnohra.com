#ifndef _RayTracer_Primitive_Cone_hpp
#define _RayTracer_Primitive_Cone_hpp

#include "Primitive.hpp"

namespace rayTracer {

	class Primitive_Cone : public Primitive {
	public:

		Primitive_Cone();
		
	protected:

		float zMin;
		float zMax;
	};

}

#endif