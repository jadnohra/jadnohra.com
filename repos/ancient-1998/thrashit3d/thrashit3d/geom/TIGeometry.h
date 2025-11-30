#ifndef _TIGEOMETRY_H__
#define _TIGEOMETRY_H__

#include "TIBasicVertex.h"
#include "TIVertex.h"
#include "TITriangle.h"
#include "TITransform.h"


#ifndef THRASHIT3D_LOOKUPTABLES	
	#define TIGEOMETRY_COS(FANGLE)	cos(FANGLE)
	#define TIGEOMETRY_SIN(FANGLE)	sin(FANGLE) 
#endif
#ifdef THRASHIT3D_LOOKUPTABLES	
	#define TIGEOMETRY_COS(FANGLE)	TIGeometry::LOOKUP_COS[(int) (THRASHIT3D_LOOKUPTABLEOFFSET + FANGLE * THRASHIT3D_LOOKUPRESOLUTION)]
	#define TIGEOMETRY_SIN(FANGLE)	TIGeometry::LOOKUP_SIN[(int) (THRASHIT3D_LOOKUPTABLEOFFSET + FANGLE * THRASHIT3D_LOOKUPRESOLUTION)] 

#endif

class TIGeometry
{
private:
	static TIGeometry	object;
	static BOOL			connected;

public:

	#ifdef THRASHIT3D_LOOKUPTABLES	
		static float LOOKUP_COS[THRASHIT3D_LOOKUPTABLESIZE];
		static float LOOKUP_SIN[THRASHIT3D_LOOKUPTABLESIZE];
	#endif
	
	static BOOL Connect();
	static VOID Disconnect();

	static VOID GetLogInfo(String& info);

	~TIGeometry();
};


#endif

