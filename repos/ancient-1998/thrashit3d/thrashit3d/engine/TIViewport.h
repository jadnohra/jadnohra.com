#ifndef _THRASHIT3DVIEWPORT_H__
#define _THRASHIT3DVIEWPORT_H__

#include "TIStructs.h"
#include "TIZBuffer.h"


struct TIViewport
{
	TICamera		Camera;
	TIZBuffer		ZBuffer;
	CGraphic*		lpDestGraphic;
	BOOL			AutoReleaseBuffer;

	int				Focal;
	D2Vector<short>	ViewportCenter;
	
	WORD			RenderMode;	

	
	
	
	
	TIViewport(int focal=THRASHIT3D_DEFAULT_FOCAL);
	~TIViewport();

	BOOL Connect(CGraphic* lpGraphic);
	BOOL Connect(CGraphic* lpGraphic,D2Vector<short> center);
	VOID Disconnect();
};


#endif