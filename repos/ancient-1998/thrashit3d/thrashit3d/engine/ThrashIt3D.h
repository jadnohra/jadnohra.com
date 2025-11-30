#ifndef _THRASHIT3D_H__
#define _THRASHIT3D_H__

#include "TIStageIncludes.h"
#include "TIStages.h"

/*
in reality Viewer and Camera Settings are the inverse of the values!
to speed up things!
*/

class ThrashIt3D
{

protected:


	BOOL				Connected;
	
	CFastPrimitiveArrayKeepTogether<TITriangle> Triangles;
	CFastPrimitiveArrayKeepTogether<TITriangle> TempTriangles;

	/*
	inline VOID _Translate(D3Vector<float>& point,D3Vector<float>& trans,D3Vector<float>& result);
	inline VOID _Rotate(D3Vector<float>& point,D3Vector<float>& trans,D3Vector<float>& result);
	inline VOID _Project(Viewport* lpView,D3Vector<float>& point,D2Vector<float>& result);
	inline VOID _Project(D3Vector<float>& point,D2Vector<float>& result);

	inline VOID _RasterizeWireframe(Triangle& tr);
	inline VOID _RenderWireframe(Viewport* lpView);

	inline VOID _RasterizeWireframeUp(float XUp,float YUp,float XDown1,float YDown1,float XDown2,float YDown2,DWORD color);
	inline VOID _RasterizeWireframeDown(float XDown,float YDown,float XUp1,float YUp1,float XUp2,float YUp2,DWORD color);

	
	inline VOID _RasterizeFlat(Triangle& tr);
	inline VOID _RenderFlat(Viewport* lpView);

	inline VOID _RasterizeFlatUp(float XUp,float YUp,float XDown1,float YDown1,float XDown2,float YDown2,DWORD color);
	inline VOID _RasterizeFlatDown(float XDown,float YDown,float XUp1,float YUp1,float XUp2,float YUp2,DWORD color);

	
	inline VOID _RasterizeGouraud(Triangle& tr);
	inline VOID _RenderGouraud(Viewport* lpView);

	inline VOID _RasterizeGouraudUp(float XUp,float YUp,CDXColor colorUp,float ZUp,float XDown1,float YDown1,CDXColor colorDown1,float ZDown1,float XDown2,float YDown2,CDXColor colorDown2,float ZDown2);
	inline VOID _RasterizeGouraudDown(float XDown,float YDown,CDXColor colorDown,float ZDown,float XUp1,float YUp1,CDXColor colorUp1,float ZUp1,float XUp2,float YUp2,CDXColor colorUp2,float ZUp2);

	
	inline FLOAT _LineIntersectWithHorizNoCheck(D2Vector<float> LinePt1,D2Vector<float> LinePt2,float HorizLineY);
	inline FLOAT _LineIntersectWithHorizNoCheck(float LinePt1X,float LinePt1Y,float LinePt2X,float LinePt2Y,float HorizLineY);
	*/

	TIStageTransformation			StageTransformation;
	TIStageOcclusion				StageOcclusion;
	TIStageProjection				StageProjection;
	TIStageRasterization			StageRasterization;
	TIStageRasterizationWireframe	StageRasterizationWireframe;
	TIStageRasterizationFlat		StageRasterizationFlat;
	TIStageRasterizationGouraud		StageRasterizationGouraud;

#ifdef THRASHIT3D_LOGFILE
	PerformanceLogger* lpLogger;
#endif
	

public:

	static DWORD RENDERMODE_WIREFRAME;
	static DWORD RENDERMODE_FLAT;
	static DWORD RENDERMODE_GOURAUD;
	static DWORD RENDERMODE_NONE;

	static DWORD RENDERMODE_WIREFRAME_COLOR;

#ifdef THRASHIT3D_LOGFILE
	LogFile	Logfile;
#endif


	
	BOOL Connect(LONG TriangleCount=THRASHIT3D_DEFAULT_TRIANGLE_COUNT);
	VOID Disconnect();
	
	ThrashIt3D();
	~ThrashIt3D();

	/*
	VOID FC Translate(D3Vector<float>& point,D3Vector<float>& trans,D3Vector<float>& result);
	VOID FC Rotate(D3Vector<float>& point,D3Vector<float>& trans,D3Vector<float>& result);
	VOID FC Project(Viewport* lpView,D3Vector<float>& point,D2Vector<float>&result);
	*/
	

	TITriangle* NewTriangle();
	VOID Render(TIViewport *lpView);


	VOID LogPerformance();


};



#endif