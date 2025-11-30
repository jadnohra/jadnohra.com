#ifndef _TISTAGETRANSFORMGEOMETRY_H__
#define _TISTAGETRANSFORMGEOMETRY_H__

#include "TIStageIncludes.h"


class TIStageTransformation
{
public:

#ifdef THRASHIT3D_LOGFILE
	PerformanceLogger*	lpLogger;
	LogFile*			lpLogfile;
#endif

	virtual BOOL Connect();
	virtual VOID DisConnect();
	

	virtual VOID FC ExecuteStage(TIBasicVertex& ViewportPosition,TIBasicVertex& ViewportRotation,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles);
};

#endif