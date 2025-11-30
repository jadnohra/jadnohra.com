#ifndef _TISTAGERASTERGOURAUD_H__
#define _TISTAGERASTERGOURAUD_H__

#include "TIStageIncludes.h"
#include "TIStageRasterization.h"

class TIStageRasterizationGouraud : public TIStageRasterization
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
	inline VOID _RasterizeUp(float XUp,float YUp,CDXColor colorUp,float ZUp,float XDown1,float YDown1,CDXColor colorDown1,float ZDown1,float XDown2,float YDown2,CDXColor colorDown2,float ZDown2);
	inline VOID _RasterizeDown(float XDown,float YDown,CDXColor colorDown,float ZDown,float XUp1,float YUp1,CDXColor colorUp1,float ZUp1,float XUp2,float YUp2,CDXColor colorUp2,float ZUp2);
	
	CGraphic* _lpDestGraphic;
	TIZBUFFER _ZBuffer;
	TIZBUFFERINDICATOR _ZIndicator;
	
	
};

#endif