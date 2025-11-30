#ifndef _TISTAGEPROJECTION_H__
#define _TISTAGEPROJECTION_H__

#include "TIStageIncludes.h"


class TIStageProjection
{

public:

#ifdef THRASHIT3D_LOGFILE
	PerformanceLogger*	lpLogger;
	LogFile*			lpLogfile;
#endif

	virtual BOOL Connect();
	virtual VOID DisConnect();
	

	virtual VOID FC ExecuteStage(int& Focal,short& ViewportCenterX,short& ViewportCenterY,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles,CFastPrimitiveArrayKeepTogether<TITriangle>& TempTriangles);

};


#endif