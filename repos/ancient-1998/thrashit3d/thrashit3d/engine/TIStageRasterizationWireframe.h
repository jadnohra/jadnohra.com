#ifndef _TISTAGERASTERWIREFRAME_H__
#define _TISTAGERASTERWIREFRAME_H__

#include "TIStageIncludes.h"
#include "TIStageRasterization.h"

class TIStageRasterizationWireframe : public TIStageRasterization 
{
public:	

#ifdef THRASHIT3D_LOGFILE
	PerformanceLogger*	lpLogger;
	LogFile*			lpLogfile;
#endif


	virtual BOOL Connect();
	virtual VOID DisConnect();
	

	virtual VOID FC ExecuteStage(CGraphic* lpGraphic,TIZBuffer* lpZBuffer,CFastPrimitiveArrayKeepTogether<TITriangle>& Triangles,CFastPrimitiveArrayKeepTogether<TITriangle>& TempTriangles);

protected:
	inline VOID _Rasterize(TITriangle& tr);
	inline VOID _RasterizeUp(float XUp,float YUp,float XDown1,float YDown1,float XDown2,float YDown2,DWORD color);
	inline VOID _RasterizeDown(float XDown,float YDown,float XUp1,float YUp1,float XUp2,float YUp2,DWORD color);

	CGraphic* _lpDestGraphic;
};


#endif