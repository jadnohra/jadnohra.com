#include "TIViewport.h"

TIViewport::TIViewport(int focal)
{
	lpDestGraphic = NULL;
	AutoReleaseBuffer = FALSE;
	
	Focal = focal;
	
	Camera.Position.Zero();
	Camera.Position.z=THRASHIT3D_DEFAULT_VIEWER_Z;
		
	RenderMode = THRASHIT3D_RENDER_DEFAULT;
}

BOOL TIViewport::Connect(CGraphic* lpGraphic)
{
	lpDestGraphic = lpGraphic;
	AutoReleaseBuffer = FALSE;
	
	ViewportCenter.x = lpDestGraphic->frame.right / 2;
	ViewportCenter.y = lpDestGraphic->frame.bottom / 2;
	
	return(ZBuffer.Connect(lpGraphic->frame.right,lpGraphic->frame.bottom));
}

BOOL TIViewport::Connect(CGraphic* lpGraphic,D2Vector<short> center)
{
	lpDestGraphic = lpGraphic;
	AutoReleaseBuffer = FALSE;
	ViewportCenter = center;

	return(ZBuffer.Connect(lpGraphic->frame.right,lpGraphic->frame.bottom));
}

VOID TIViewport::Disconnect()
{
	if(lpDestGraphic && AutoReleaseBuffer)
	{
		delete lpDestGraphic;
	}
	lpDestGraphic = NULL;

	ZBuffer.Disconnect();
}

TIViewport::~TIViewport()
{
	Disconnect();
}
