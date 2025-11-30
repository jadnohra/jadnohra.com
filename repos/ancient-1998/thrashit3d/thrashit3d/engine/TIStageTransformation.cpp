#include "TIStageTransformation.h"
#include "..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"

#include "..\geom\TITransform.cpp"
#include "..\geom\TIAngle.cpp"
#include "ThrashIt3D.h"


BOOL TIStageTransformation::Connect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogger = lpLogfile->NewPerformanceLogger("Transformation");
#endif

	return(TRUE);
}


VOID TIStageTransformation::DisConnect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogfile->DeletePerformanceLogger(lpLogger);
#endif

}


VOID TIStageTransformation::ExecuteStage(TIBasicVertex& ViewportPosition,TIBasicVertex& ViewportRotation,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles)
{
#ifdef THRASHIT3D_LOGFILE
	STARTTIMER(lpLogger);
#endif


	TIAcceleratedRotation rot;
	TIInlineRadToTIRotation(-ViewportRotation.x,-ViewportRotation.y,-ViewportRotation.z,rot);
	for(int RunIndex = 0;RunIndex < Triangles.UsedCount;RunIndex++)
	{
		
		TIInlineNegativeTranslate(Triangles.Array[RunIndex].v1.World,ViewportPosition,Triangles.Array[RunIndex].v1.Aligned);
		TIInlineNegativeTranslate(Triangles.Array[RunIndex].v2.World,ViewportPosition,Triangles.Array[RunIndex].v2.Aligned);
		TIInlineNegativeTranslate(Triangles.Array[RunIndex].v3.World,ViewportPosition,Triangles.Array[RunIndex].v3.Aligned);
		

		TIInlineAcceleratedRotate(Triangles.Array[RunIndex].v1.Aligned,rot,Triangles.Array[RunIndex].v1.Aligned);
		TIInlineAcceleratedRotate(Triangles.Array[RunIndex].v2.Aligned,rot,Triangles.Array[RunIndex].v2.Aligned);
		TIInlineAcceleratedRotate(Triangles.Array[RunIndex].v3.Aligned,rot,Triangles.Array[RunIndex].v3.Aligned);

	}
#ifdef THRASHIT3D_LOGFILE
	STOPTIMER(lpLogger);
#endif

}


