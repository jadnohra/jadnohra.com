#include "TIStageProjection.h"
#include "..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"

#include "ThrashIt3D.h"
#include "..\geom\TITransform.cpp"

BOOL TIStageProjection::Connect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogger = lpLogfile->NewPerformanceLogger("Projection");
	
#endif

	return(TRUE);
}


VOID TIStageProjection::DisConnect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogfile->DeletePerformanceLogger(lpLogger);
#endif

}


VOID TIStageProjection::ExecuteStage(int& Focal,short& ViewportCenterX,short& ViewportCenterY,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles,CFastPrimitiveArrayKeepTogether<TITriangle>& TempTriangles)
{
#ifdef THRASHIT3D_LOGFILE
	STARTTIMER(lpLogger);
#endif

	for(int RunIndex = 0;RunIndex < Triangles.UsedCount;RunIndex++)
	{
		TIInlineProject(Focal,ViewportCenterX,ViewportCenterY,Triangles.Array[RunIndex].v1.Aligned,Triangles.Array[RunIndex].v1.Screen);
		TIInlineProject(Focal,ViewportCenterX,ViewportCenterY,Triangles.Array[RunIndex].v2.Aligned,Triangles.Array[RunIndex].v2.Screen);
		TIInlineProject(Focal,ViewportCenterX,ViewportCenterY,Triangles.Array[RunIndex].v3.Aligned,Triangles.Array[RunIndex].v3.Screen);
	}
	for(RunIndex = 0;RunIndex < TempTriangles.UsedCount;RunIndex++)
	{
		TIInlineProject(Focal,ViewportCenterX,ViewportCenterY,TempTriangles.Array[RunIndex].v1.Aligned,TempTriangles.Array[RunIndex].v1.Screen);
		TIInlineProject(Focal,ViewportCenterX,ViewportCenterY,TempTriangles.Array[RunIndex].v2.Aligned,TempTriangles.Array[RunIndex].v2.Screen);
		TIInlineProject(Focal,ViewportCenterX,ViewportCenterY,TempTriangles.Array[RunIndex].v3.Aligned,TempTriangles.Array[RunIndex].v3.Screen);
	}
#ifdef THRASHIT3D_LOGFILE
	STOPTIMER(lpLogger);
#endif

}
