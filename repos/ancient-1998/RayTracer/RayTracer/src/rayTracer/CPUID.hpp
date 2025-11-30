#ifndef _rayTracer_CPUID_hpp
#define _rayTracer_CPUID_hpp

namespace rayTracer {

	struct CPUID {
	
		char vendorString[(4 * 3) + 1];

		unsigned __int32 MMX : 1;
		unsigned __int32 SSE : 1;
		unsigned __int32 SSE2 : 1;
		unsigned __int32 SSE3 : 1;

		unsigned __int32 AMDMMX : 1;
		unsigned __int32 _3DNow : 1;
		unsigned __int32 _3DNow2 : 1;

		unsigned __int32 AltiVec : 1;

		unsigned __int32 CyrixEMMX : 1;

		CPUID();
	};

	const CPUID& getCPUID();
}

#endif