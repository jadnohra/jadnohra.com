#include "RayTracerConfig.hpp"

#if !RayTracerConfig_EnableParallelProcessing

	#include "ParallelProcessor_Dummy.hpp"

#elif RayTracerConfig_ParallelProcessor_Simple

	#include "ParallelProcessor_Simple.hpp"

#elif RayTracerConfig_ParallelProcessor_OpenMP

	#include "omp.h"
	#include "ParallelProcessor_Dummy.hpp"

#endif

