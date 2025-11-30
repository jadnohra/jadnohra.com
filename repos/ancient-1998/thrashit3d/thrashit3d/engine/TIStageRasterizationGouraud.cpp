#include "TIStageRasterizationGouraud.h"
#include "..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"


#include "ThrashIt3D.h"


BOOL TIStageRasterizationGouraud::Connect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogger = lpLogfile->NewPerformanceLogger("RasterizationGouraud");
#endif

	return(TRUE);
}


VOID TIStageRasterizationGouraud::DisConnect()
{
#ifdef THRASHIT3D_LOGFILE
	lpLogfile->DeletePerformanceLogger(lpLogger);
#endif

}




VOID TIStageRasterizationGouraud::ExecuteStage(CGraphic* lpGraphic,TIZBuffer* lpZBuffer,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles,CFastPrimitiveArrayKeepTogether<TITriangle>& TempTriangles)
{
#ifdef THRASHIT3D_LOGFILE
	STARTTIMER(lpLogger);
#endif
	
	lpZBuffer->Reset();

	_ZBuffer = TIZBUFFER_GETBUFFER(*lpZBuffer);
	_ZIndicator = TIZBUFFER_GETINDICATOR(*lpZBuffer);
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


VOID TIStageRasterizationGouraud::_Rasterize(TITriangle& tr)
{
	TIVertex *sortedY1,*sortedY2,*sortedY3;
	float interX;	
	float interZ;
	CDXColor interColor;
	
	
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
			_RasterizeUp(sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z,sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z);
		else
			_RasterizeDown(sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z,sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z);
		return;
	}		
	
	
	if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY3->Screen.y))
	{
		if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY2->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY2->Screen.y)<INTCOORD(sortedY1->Screen.y))
			_RasterizeUp(sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z);
		else
			_RasterizeDown(sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z);
		return;
	}		
	
	
	
	if(INTCOORD(sortedY2->Screen.y)==INTCOORD(sortedY3->Screen.y))
	{
		if(INTCOORD(sortedY2->Screen.y)==INTCOORD(sortedY1->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY1->Screen.y)<INTCOORD(sortedY2->Screen.y))
			_RasterizeUp(sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z);
		else
			_RasterizeDown(sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z);
		return;
	}		
	
	
	
	interX = _LINEINTERSECTWITHHORIZNOCHECK(sortedY1->Screen,sortedY3->Screen,sortedY2->Screen.y);
	
	
	interColor.r = LINEINTERSECTWITHHORIZNOCHECK(sortedY1->color.r,sortedY1->Screen.y,sortedY3->color.r,sortedY3->Screen.y,sortedY2->Screen.y);
	interColor.g = LINEINTERSECTWITHHORIZNOCHECK(sortedY1->color.g,sortedY1->Screen.y,sortedY3->color.g,sortedY3->Screen.y,sortedY2->Screen.y);
	interColor.b = LINEINTERSECTWITHHORIZNOCHECK(sortedY1->color.b,sortedY1->Screen.y,sortedY3->color.b,sortedY3->Screen.y,sortedY2->Screen.y);
	interColor.color = interColor.Color16Bit(interColor.r,interColor.g,interColor.b);
	
	
	interZ = LINEINTERSECTWITHHORIZNOCHECK(sortedY1->World.z,sortedY1->Screen.y,sortedY3->World.z,sortedY3->Screen.y,sortedY2->Screen.y);
	
	
	_RasterizeUp(sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,interX,sortedY2->Screen.y,interColor,interZ);
	_RasterizeDown(sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,interX,sortedY2->Screen.y,interColor,interZ);
	
	
	
}




VOID TIStageRasterizationGouraud::_RasterizeUp(float XUp,float YUp,CDXColor colorUp,float ZUp,float XDown1,float YDown1,CDXColor colorDown1,float ZDown1,float XDown2,float YDown2,CDXColor colorDown2,float ZDown2)
{
	//sort XDown1 XDown2
	if(XDown1 > XDown2)
	{
		float temp;
		CDXColor tempColor;
		
		temp = XDown2;
		XDown2 = XDown1;
		XDown1 = temp;
		
		temp = YDown2;
		YDown2 = YDown1;
		YDown1 = temp;
		
		tempColor = colorDown2;
		colorDown2 = colorDown1;
		colorDown1 = tempColor;
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
	
	CDXFloatColorComponents ScanColor;
	
	CDXFloatColorComponents lColor;
	lColor.Set(colorUp);
	lColor.rInc = (float)(colorDown1.r - colorUp.r) / (float)(ScanYEnd - ScanY);
	lColor.gInc = (float)(colorDown1.g - colorUp.g) / (float)(ScanYEnd - ScanY);
	lColor.bInc = (float)(colorDown1.b - colorUp.b) / (float)(ScanYEnd - ScanY);
	
	CDXFloatColorComponents rColor;
	rColor.Set(colorUp);
	rColor.rInc = (float)(colorDown2.r - colorUp.r) / (float)(ScanYEnd - ScanY);
	rColor.gInc = (float)(colorDown2.g - colorUp.g) / (float)(ScanYEnd - ScanY);
	rColor.bInc = (float)(colorDown2.b - colorUp.b) / (float)(ScanYEnd - ScanY);
	
	float ScanZ;
	float ScanZInc;
	float leftZInc  = (float)(ZDown1 - ZUp) / (float)(ScanYEnd - ScanY);
	float rightZInc = (float)(ZDown2 - ZUp) / (float)(ScanYEnd - ScanY);
	float leftZ = ZUp;
	float rightZ = ZUp;
	
	
	
	
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
		
		
		lColor.r += lColor.rInc;
		lColor.g += lColor.gInc;
		lColor.b += lColor.bInc;
		rColor.r += rColor.rInc;
		rColor.g += rColor.gInc;
		rColor.b += rColor.bInc;
		ScanColor = lColor;
		ScanColor.rInc = ((float) (rColor.r - lColor.r)) / ((float)(EdgeEndX - EdgeStartX));
		ScanColor.gInc = ((float) (rColor.g - lColor.g)) / ((float)(EdgeEndX - EdgeStartX));
		ScanColor.bInc = ((float) (rColor.b - lColor.b)) / ((float)(EdgeEndX - EdgeStartX));
		
		leftZ += leftZInc;
		rightZ += rightZInc;
		ScanZ = leftZ;
		ScanZInc = ((float) (rightZ - leftZ)) / ((float)(EdgeEndX - EdgeStartX));
		
		
		X = EdgeStartX;
		while(X<=EdgeEndX) 
		{
			
			//if(ScanColor.r > 255 || ScanColor.r < 0 ) 
			//{
			//	ScanColor.r = ScanColor.r;
			//}
			//if(ScanColor.g > 255 || ScanColor.g < 0) 
			//{
			//	ScanColor.g = ScanColor.g;
			//}
			//if(ScanColor.b > 255 || ScanColor.b < 0) 
			//{
			//	ScanColor.b = ScanColor.b;
			//}
			
			
			if(TIZBUFFER_SHOULDDRAWPIXEL(ScanZ,_ZBuffer,_ZIndicator,X,ScanY))
			{
				
				_lpDestGraphic->ClippedPoint(X,ScanY,ScanColor.Color16Bit());
				TIZBUFFER_SETZVALUE(ScanZ,_ZBuffer,_ZIndicator,X,ScanY);
			}
			
			
			X++;
			ScanColor.r += ScanColor.rInc;
			ScanColor.g += ScanColor.gInc;
			ScanColor.b += ScanColor.bInc;
			ScanZ +=ScanZInc;
		}
		
		ScanY++;
		
	}
}



VOID TIStageRasterizationGouraud::_RasterizeDown(float XDown,float YDown,CDXColor colorDown,float ZDown,float XUp1,float YUp1,CDXColor colorUp1,float ZUp1,float XUp2,float YUp2,CDXColor colorUp2,float ZUp2)
{
	
	
	//sort XUp1 XUp2
	if(XUp1 > XUp2)
	{
		float temp;
		CDXColor tempColor;
		
		temp = XUp2;
		XUp2 = XUp1;
		XUp1 = temp;
		
		temp = YUp2;
		YUp2 = YUp1;
		YUp1 = temp;
		
		tempColor = colorUp2;
		colorUp2 = colorUp1;
		colorUp1 = tempColor;
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
	
	CDXFloatColorComponents ScanColor;
	
	CDXFloatColorComponents lColor;
	lColor.Set(colorUp1);
	lColor.rInc = (float)(colorDown.r - colorUp1.r) / (float)(ScanYEnd - ScanY);
	lColor.gInc = (float)(colorDown.g - colorUp1.g) / (float)(ScanYEnd - ScanY);
	lColor.bInc = (float)(colorDown.b - colorUp1.b) / (float)(ScanYEnd - ScanY);
	
	CDXFloatColorComponents rColor;
	rColor.Set(colorUp2);
	rColor.rInc = (float)(colorDown.r - colorUp2.r) / (float)(ScanYEnd - ScanY);
	rColor.gInc = (float)(colorDown.g - colorUp2.g) / (float)(ScanYEnd - ScanY);
	rColor.bInc = (float)(colorDown.b - colorUp2.b) / (float)(ScanYEnd - ScanY);
	
	
	float ScanZ;
	float ScanZInc;
	float leftZInc  = (float)(ZDown - ZUp1) / (float)(ScanYEnd - ScanY);
	float rightZInc = (float)(ZDown - ZUp2) / (float)(ScanYEnd - ScanY);
	float leftZ = ZUp1;
	float rightZ = ZUp2;
	
	
	
	
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
		
		
		lColor.r += lColor.rInc;
		lColor.g += lColor.gInc;
		lColor.b += lColor.bInc;
		rColor.r += rColor.rInc;
		rColor.g += rColor.gInc;
		rColor.b += rColor.bInc;
		ScanColor = lColor;
		ScanColor.rInc = ((float) (rColor.r - lColor.r)) / ((float)(EdgeEndX - EdgeStartX));
		ScanColor.gInc = ((float) (rColor.g - lColor.g)) / ((float)(EdgeEndX - EdgeStartX));
		ScanColor.bInc = ((float) (rColor.b - lColor.b)) / ((float)(EdgeEndX - EdgeStartX));
		
		leftZ += leftZInc;
		rightZ += rightZInc;
		ScanZ = leftZ;
		ScanZInc = ((float) (rightZ - leftZ)) / ((float)(EdgeEndX - EdgeStartX));
		
		
		
		X = EdgeStartX;
		while(X<=EdgeEndX) 
		{
			
			if(ScanColor.r > 255 || ScanColor.r < 0) 
			{
				ScanColor.r = ScanColor.r;
			}
			if(ScanColor.g > 255 || ScanColor.g < 0) 
			{
				ScanColor.g = ScanColor.g;
			}
			if(ScanColor.b > 255 || ScanColor.b < 0) 
			{
				ScanColor.b = ScanColor.b;
			}
			
			if(TIZBUFFER_SHOULDDRAWPIXEL(ScanZ,_ZBuffer,_ZIndicator,X,ScanY))
			{
				
				_lpDestGraphic->ClippedPoint(X,ScanY,ScanColor.Color16Bit());
				TIZBUFFER_SETZVALUE(ScanZ,_ZBuffer,_ZIndicator,X,ScanY);
			}
			
			X++;
			ScanColor.r += ScanColor.rInc;
			ScanColor.g += ScanColor.gInc;
			ScanColor.b += ScanColor.bInc;
			ScanZ+=ScanZInc;
		}
		
		ScanY++;
		
	}
	
}
