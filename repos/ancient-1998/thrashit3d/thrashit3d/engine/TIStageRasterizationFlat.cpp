#include "TIStageRasterizationFlat.h"
#include "..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"


#include "ThrashIt3D.h"

BOOL TIStageRasterizationFlat::Connect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogger = lpLogfile->NewPerformanceLogger("RasterizationFlat");
#endif

	return(TRUE);
}


VOID TIStageRasterizationFlat::DisConnect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogfile->DeletePerformanceLogger(lpLogger);
#endif

}




VOID TIStageRasterizationFlat::ExecuteStage(CGraphic* lpGraphic,TIZBuffer* lpZBuffer,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles,CFastPrimitiveArrayKeepTogether<TITriangle>& TempTriangles)
{

#ifdef THRASHIT3D_LOGFILE
	STARTTIMER(lpLogger);
#endif
	
	lpZBuffer->Reset();	

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


VOID TIStageRasterizationFlat::_Rasterize(TITriangle& tr)
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


VOID TIStageRasterizationFlat::_RasterizeUp(float XUp,float YUp,float XDown1,float YDown1,float XDown2,float YDown2,DWORD color)
{
	
	//sort XDown1 XDown2
	if(XDown1 > XDown2)
	{
		float temp;
		
		temp = XDown2;
		XDown2 = XDown1;
		XDown1 = temp;
		
		temp = YDown2;
		YDown2 = YDown1;
		YDown1 = temp;
	}
	
	
	float aLeft = SLOPE(XUp,YUp,XDown1,YDown1);
	float aRight = SLOPE(XUp,YUp,XDown2,YDown2);	
	float bLeft = B(XUp,YUp,aLeft);
	float bRight = B(XUp,YUp,aRight);	
	int ScanY = INTCOORD(YUp);
	int EdgeStartX;
	int EdgeEndX;
	int X;
	int ScanYEnd = INTCOORD(YDown1);
	
	
	ScanY++; //dont draw top and right!!!
	
	while(ScanY <= ScanYEnd)
	{
		if(ScanY>YDown1) 
		{
			EdgeStartX = INTCOORD(X(aLeft,bLeft,YDown1));
			EdgeEndX = INTCOORD(X(aRight,bRight,YDown1)) - 1; //dont draw top and right!!!
		} 
		else
		{
			EdgeStartX = INTCOORD(X(aLeft,bLeft,ScanY));
			EdgeEndX = INTCOORD(X(aRight,bRight,ScanY)) - 1; //dont draw top and right!!!
		}
		X = EdgeStartX;
		while(X<=EdgeEndX) 
		{
			_lpDestGraphic->ClippedPoint(X,ScanY,color);
			X++;
		}
		
		ScanY++;
		
	}
	
}


VOID TIStageRasterizationFlat::_RasterizeDown(float XDown,float YDown,float XUp1,float YUp1,float XUp2,float YUp2,DWORD color)
{
	
	//sort XUp1 XUp2
	if(XUp1 > XUp2)
	{
		float temp;
		
		temp = XUp2;
		XUp2 = XUp1;
		XUp1 = temp;
		
		temp = YUp2;
		YUp2 = YUp1;
		YUp1 = temp;
	}
	
	
	float aLeft = SLOPE(XUp1,YUp1,XDown,YDown);
	float aRight = SLOPE(XUp2,YUp2,XDown,YDown);	
	float bLeft = B(XUp1,YUp1,aLeft);
	float bRight = B(XUp2,YUp2,aRight);	
	int ScanY = INTCOORD(YUp1);
	int EdgeStartX;
	int EdgeEndX;
	int X;
	int ScanYEnd = INTCOORD(YDown);
	
	
	//
	ScanY++; //dont draw top and right!!!
	
	while(ScanY <= ScanYEnd)
	{
		if(ScanY>YDown) 
		{
			EdgeStartX = INTCOORD(X(aLeft,bLeft,YDown));
			EdgeEndX = INTCOORD(X(aRight,bRight,YDown)) - 1; //dont draw top and right!!!
		} 
		else
		{
			EdgeStartX = INTCOORD(X(aLeft,bLeft,ScanY));
			EdgeEndX = INTCOORD(X(aRight,bRight,ScanY)) - 1; //dont draw top and right!!!
		}
		X = EdgeStartX;
		while(X<=EdgeEndX) 
		{
			_lpDestGraphic->ClippedPoint(X,ScanY,color);
			
			
			X++;
		}
		
		ScanY++;
		
	}
	
}
