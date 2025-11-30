#include "TIStageRasterization.h"
#include "..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"

#include "ThrashIt3D.h"


BOOL TIStageRasterization::Connect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogger = lpLogfile->NewPerformanceLogger("Rasterization");
#endif

	return(TRUE);
}


VOID TIStageRasterization::DisConnect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogfile->DeletePerformanceLogger(lpLogger);
#endif

}


VOID TIStageRasterization::ExecuteStage(CGraphic* lpGraphic,TIZBuffer* lpZBuffer,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles,CFastPrimitiveArrayKeepTogether<TITriangle>& TempTriangles)
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


