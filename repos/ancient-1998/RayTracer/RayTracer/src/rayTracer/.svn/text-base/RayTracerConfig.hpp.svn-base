#ifndef _rayTracer_RayTracerConfig_hpp
#define _rayTracer_RayTracerConfig_hpp

//#define RayTracerConfig_SpatialAccel_Debug
#define RayTracerConfig_SpatialAccel_Debug_Color Vector3(1.0f, 1.0f, 0.0f)
#define RayTracerConfig_SpatialAccel_Debug_Alpha 0.01f

//#define RayTracerConfig_BIHNode
#define RayTracerConfig_BVH2
#define RayTracerConfig_SpatialAccel_Iter
#define RayTracerConfig_UseSpatialAccel
//#define RayTracerConfig_EnablePackets
#define RayTracerConfig_EnablePacketTraversalOcclusion
//#define RayTracerConfig_SSE

#define RayTracerConfig_TryAlignCahe

#ifdef RayTracerConfig_PP_OpenMP

	#define RayTracerConfig_EnableParallelProcessing 1
	#define RayTracerConfig_ParallelProcessor_OpenMP 1

#else 

	//#define RayTracerConfig_EnableParallelProcessing 1

	#if RayTracerConfig_EnableParallelProcessing

		#define RayTracerConfig_ParallelProcessor_Debug 1

		//#define RayTracerConfig_ParallelProcessor_Simple 1
		#define RayTracerConfig_ParallelProcessor_OpenMP 1

	#endif
#endif

//#define RayTracerConfig_Trace_SpatialAccel_Traversal

#endif