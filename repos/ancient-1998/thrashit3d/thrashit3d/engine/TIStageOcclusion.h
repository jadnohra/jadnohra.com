#ifndef _TISTAGEOCCLUSION_H__
#define _TISTAGEOCCLUSION_H__

#include "TIStageIncludes.h"

	
class TIStageOcclusion
{

public:

#ifdef THRASHIT3D_LOGFILE
	PerformanceLogger*	lpLogger;
	LogFile*			lpLogfile;
#endif

	virtual BOOL Connect();
	virtual VOID DisConnect();
	
	virtual VOID FC ExecuteStage(TICamera* lpCamera,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles,CFastPrimitiveArrayKeepTogether<TITriangle>& TempTriangles);

};

#endif