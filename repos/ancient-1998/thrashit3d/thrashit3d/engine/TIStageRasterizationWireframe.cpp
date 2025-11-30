#include "TIStageRasterizationWireframe.h"
#include "..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"


#include "ThrashIt3D.h"


BOOL TIStageRasterizationWireframe::Connect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogger = lpLogfile->NewPerformanceLogger("RasterizationWireframe");
#endif

	return(TRUE);
}


VOID TIStageRasterizationWireframe::DisConnect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogfile->DeletePerformanceLogger(lpLogger);
#endif

}




VOID TIStageRasterizationWireframe::ExecuteStage(CGraphic* lpGraphic,TIZBuffer* lpZBuffer,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles,CFastPrimitiveArrayKeepTogether<TITriangle>& TempTriangles)
{

#ifdef THRASHIT3D_LOGFILE
	STARTTIMER(lpLogger);
#endif	

	//lpZBuffer->Reset();
	
	_lpDestGraphic = lpGraphic;
	for(int RunIndex = 0;RunIndex < Triangles.UsedCount;RunIndex++)
	{
		_Rasterize(Triangles.Array[RunIndex]);
	}
	for(RunIndex = 0;RunIndex < TempTriangles.UsedCount;RunIndex++)
	{
		_Rasterize(TempTriangles.Array[RunIndex]);
	}

#ifdef THRASHIT3D_LOGFILE
	STOPTIMER(lpLogger);
#endif
}

VOID TIStageRasterizationWireframe::_Rasterize(TITriangle& tr)
{
	TIVertex *sortedY1,*sortedY2,*sortedY3;
	float interX;	
	float interZ;
	
	
	if(tr.v1.Screen.y<tr.v2.Screen.y)
	{
		if(tr.v1.Screen.y<tr.v3.Screen.y)
		{
			sortedY1=&tr.v1;
			
			if(tr.v2.Screen.y<tr.v3.Screen.y)
			{
				sortedY2=&tr.v2;
				sortedY3=&tr.v3;
				
			}
			else
			{
				sortedY2=&tr.v3;
				sortedY3=&tr.v2;
			}
			
		}
		else
		{
			sortedY1=&tr.v3;
			sortedY2=&tr.v1;
			sortedY3=&tr.v2;
			
		}
		
	}
	else
	{
		if(tr.v2.Screen.y<tr.v3.Screen.y)
		{
			sortedY1=&tr.v2;
			
			if(tr.v1.Screen.y<tr.v3.Screen.y)
			{
				sortedY2=&tr.v1;
				sortedY3=&tr.v3;
			}
			else
			{
				sortedY2=&tr.v3;
				sortedY3=&tr.v1;
			}
		}
		else
		{
			sortedY1=&tr.v3;
			sortedY2=&tr.v2;
			sortedY3=&tr.v1;
			
		}
		
		
	}
	
	
	if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY2->Screen.y))
	{
		if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY3->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY3->Screen.y)<INTCOORD(sortedY1->Screen.y))
			_RasterizeUp(sortedY3->Screen.x,sortedY3->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,tr.color);
		else
			_RasterizeDown(sortedY3->Screen.x,sortedY3->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,tr.color);
		return;
	}		
	
	
	if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY3->Screen.y))
	{
		if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY2->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY2->Screen.y)<INTCOORD(sortedY1->Screen.y))
			_RasterizeUp(sortedY2->Screen.x,sortedY2->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color);
		else
			_RasterizeDown(sortedY2->Screen.x,sortedY2->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color);
		return;
	}		
	
	
	
	if(INTCOORD(sortedY2->Screen.y)==INTCOORD(sortedY3->Screen.y))
	{
		if(INTCOORD(sortedY2->Screen.y)==INTCOORD(sortedY1->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY1->Screen.y)<INTCOORD(sortedY2->Screen.y))
			_RasterizeUp(sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color);
		else
			_RasterizeDown(sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color);
		return;
	}		
	
	
	interX = _LINEINTERSECTWITHHORIZNOCHECK(sortedY1->Screen,sortedY3->Screen,sortedY2->Screen.y);
	_RasterizeUp(sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,interX,sortedY2->Screen.y,tr.color);
	_RasterizeDown(sortedY3->Screen.x,sortedY3->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,interX,sortedY2->Screen.y,tr.color);
	
	
	
}


VOID TIStageRasterizationWireframe::_RasterizeUp(float XUp,float YUp,float XDown1,float YDown1,float XDown2,float YDown2,DWORD color)
{
	
	iVector from,to;
	
	from.x = XDown1;
	from.y = YDown1;
	to.x = XDown2;
	//_lpDestGraphic->ClippedHorizLine(from,to.x,CDXDraw::Color16Bit(255,255,255));
	
	to.x = XUp;
	to.y = YUp;
	_lpDestGraphic->ClippedLine(from,to,ThrashIt3D::RENDERMODE_WIREFRAME_COLOR);
	
	from.x = XDown2;
	from.y = YDown2;
	_lpDestGraphic->ClippedLine(from,to,ThrashIt3D::RENDERMODE_WIREFRAME_COLOR);	
	
}


VOID TIStageRasterizationWireframe::_RasterizeDown(float XDown,float YDown,float XUp1,float YUp1,float XUp2,float YUp2,DWORD color)
{
	
	iVector from,to;
	
	from.x = XUp1;
	from.y = YUp1;
	to.x = XUp2;
	//_lpDestGraphic->ClippedHorizLine(from,to.x,CDXDraw::Color16Bit(180,180,180));
	
	to.x = XDown;
	to.y = YDown;
	_lpDestGraphic->ClippedLine(from,to,ThrashIt3D::RENDERMODE_WIREFRAME_COLOR);
	
	from.x = XUp2;
	from.y = YUp2;
	_lpDestGraphic->ClippedLine(from,to,ThrashIt3D::RENDERMODE_WIREFRAME_COLOR);
	
}

