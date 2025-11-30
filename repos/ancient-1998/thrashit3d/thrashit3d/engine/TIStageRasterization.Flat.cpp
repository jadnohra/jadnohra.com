#include "TIStageRasterizationFlat.h"
#include "..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"

VOID TIStageRasterizationFlat::ExecuteStage(CGraphic* lpGraphic,TIZBuffer* lpZBuffer,TIZBUFFER ZBuffer,TIZBUFFERINDICATOR ZIndicator,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles,CFastPrimitiveArrayKeepTogether<TITriangle>& TempTriangles)
{
	lpZBuffer->Reset();	
	for(int RunIndex = 0;RunIndex < Triangles.UsedCount;RunIndex++)
	{
		//Triangles.Array[RunIndex].
	}
	for(RunIndex = 0;RunIndex < TempTriangles.UsedCount;RunIndex++)
	{
		//TempTriangles.Array[RunIndex].
	}
}
