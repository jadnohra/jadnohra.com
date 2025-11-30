#include "CPUID.hpp"
#include <string>

namespace rayTracer {

const CPUID& getCPUID() {

	static const CPUID CpuID;

	return CpuID;
}

struct cpuid_args {

	unsigned __int32 eax;
	unsigned __int32 ebx; 
	unsigned __int32 ecx; 
	unsigned __int32 edx;
};

void cpuid(unsigned __int32 func, cpuid_args* args) {

	__asm {

		mov	edi, args
		mov eax, func
		cpuid
		mov [edi].eax, eax
		mov [edi].ebx, ebx
		mov [edi].ecx, ecx
		mov [edi].edx, edx
	}
}

inline bool isBitSet(unsigned __int32 val, int bit) {

	return (val & (1 << (bit))) != 0;
}

CPUID::CPUID() {

	std::memset(this, 0, sizeof(CPUID));

	cpuid_args result;

	cpuid(0, &result);

	{
		((unsigned __int32*) vendorString)[0] = result.ebx;
		((unsigned __int32*) vendorString)[1] = result.edx;
		((unsigned __int32*) vendorString)[2] = result.ecx;

		vendorString[sizeof(vendorString) - 1] = 0;
	}

	if (result.eax >= 1) {

		cpuid(1, &result);
		this->MMX = isBitSet(result.edx, 23) ? 1 : 0;
		this->SSE = isBitSet(result.edx, 25) ? 1 : 0;
		this->SSE2 = isBitSet(result.edx, 26) ? 1 : 0;
		this->SSE3 = isBitSet(result.ecx, 0) ? 1 : 0;
	}

	cpuid(0x80000000, &result);

	if (result.eax >= 0x80000001) {

		cpuid(0x80000001, &result);
		this->AMDMMX = isBitSet(result.edx, 22) ? 1 : 0;
		this->_3DNow2 = isBitSet(result.edx, 30) ? 1 : 0;
		this->_3DNow = isBitSet(result.edx, 31) ? 1 : 0;
	}

	if (this->SSE) {

		//TODO
		//does OS support SSE?

		/*
		Please note that SSE extends the way the cpu operates, so not only does the CPU have to support it, but the OS also needs to support it. The way to detect this varies from system to system.
		Windows98 and up support SSE
		Linux 2.4 supports SSE (patches for 2.2, if it's not native by now)
		Various BSD flavors have support for SSE as well.
		*/
	}
}



}