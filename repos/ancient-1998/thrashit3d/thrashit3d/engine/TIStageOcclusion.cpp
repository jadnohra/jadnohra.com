#include "TIStageOcclusion.h"
#include "..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"

#include "ThrashIt3D.h"


BOOL TIStageOcclusion::Connect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogger = lpLogfile->NewPerformanceLogger("Occlusion");
	
#endif

	return(TRUE);
}


VOID TIStageOcclusion::DisConnect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogfile->DeletePerformanceLogger(lpLogger);
#endif

}


VOID TIStageOcclusion::ExecuteStage(TICamera* lpCamera,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles,CFastPrimitiveArrayKeepTogether<TITriangle>& TempTriangles)
{
	for(int RunIndex = 0;RunIndex < Triangles.UsedCount;RunIndex++)
	{
		//Triangles.Array[RunIndex].
	}
	for(RunIndex = 0;RunIndex < TempTriangles.UsedCount;RunIndex++)
	{
		//TempTriangles.Array[RunIndex].
	}
}
