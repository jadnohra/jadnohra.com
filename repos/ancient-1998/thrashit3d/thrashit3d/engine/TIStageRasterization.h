#ifndef _TISTAGERASTERIZATION_H__
#define _TISTAGERASTERIZATION_H__

#include "TIStageIncludes.h"

#define INTCOORD(FLOAT)		((int) (FLOAT+0.5))
#define SLOPE(X1,Y1,X2,Y2)	((Y1 - Y2) / (X1 - X2))
#define B(X,Y,A)			(Y - A * X)
#define X(A,B,Y)			((Y - B) / A)
#define Y(A,B,X)			(A*X + B)
#define LINEINTERSECTWITHHORIZNOCHECK(LinePt1X,LinePt1Y,LinePt2X,LinePt2Y,HorizLineY) ( ((HorizLineY - LinePt1Y) / SLOPE(LinePt1X,LinePt1Y,LinePt2X,LinePt2Y)) + LinePt1X) 
#define _LINEINTERSECTWITHHORIZNOCHECK(LinePt1,LinePt2,HorizLineY) ( ((HorizLineY - LinePt1.y) / SLOPE(LinePt1.x,LinePt1.y,LinePt2.x,LinePt2.y)) + LinePt1.x) 




class TIStageRasterization
{
public:	

#ifdef THRASHIT3D_LOGFILE
	PerformanceLogger*	lpLogger;
	LogFile*			lpLogfile;
#endif

	virtual BOOL Connect();
	virtual VOID DisConnect();
	

	virtual VOID FC ExecuteStage(CGraphic* lpGraphic,TIZBuffer* lpZBuffer,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles,CFastPrimitiveArrayKeepTogether<TITriangle>& TempTriangles);
};


#endif