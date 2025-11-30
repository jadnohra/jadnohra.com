#include "TIGeometry.h"

TIGeometry TIGeometry::object;
BOOL		TIGeometry::connected = FALSE;


#ifdef THRASHIT3D_LOOKUPTABLES	
	float TIGeometry::LOOKUP_COS[THRASHIT3D_LOOKUPTABLESIZE];
	float TIGeometry::LOOKUP_SIN[THRASHIT3D_LOOKUPTABLESIZE];
#endif


BOOL TIGeometry::Connect()
{
	if(connected)
	{
		return(TRUE);
	}

	#ifdef THRASHIT3D_LOOKUPTABLES
		int off = THRASHIT3D_LOOKUPTABLEOFFSET;
		int size = THRASHIT3D_LOOKUPTABLESIZE;

		for(float i = -2.0f * THRASHIT3D_LOOKUPRESOLUTION * THRASHIT3D_PI; (int) i + off < THRASHIT3D_LOOKUPTABLESIZE; i += 1.0f )
		{
			LOOKUP_COS[(int) i + off] = (float) cos(i/THRASHIT3D_LOOKUPRESOLUTION);
			LOOKUP_SIN[(int) i + off] = (float) sin(i/THRASHIT3D_LOOKUPRESOLUTION);
		}
		
	#endif
		
	return(connected = TRUE);
}


VOID TIGeometry::GetLogInfo(String& info)
{
	#ifndef THRASHIT3D_LOOKUPTABLES
		info.Set("Not Using Lookup Tables");
	#endif
	
	#ifdef THRASHIT3D_LOOKUPTABLES
		
		info.Set("Using Lookup Tables Resolution[");
		LONG temp = THRASHIT3D_LOOKUPRESOLUTION;
		info.Append(temp);
		
		temp = THRASHIT3D_LOOKUPTABLESIZE;
		info.Append("] Lookup Table Size[");
		info.Append(temp);

		 temp = THRASHIT3D_LOOKUPTABLESIZE;
		info.Append("] Memory Used[");
		info.Append((LONG) (temp * 2 * sizeof(FLOAT)));

		info.Append(" bytes]");

	#endif
	
}

VOID TIGeometry::Disconnect()
{
	connected = FALSE;
}


TIGeometry::~TIGeometry()
{
	TIGeometry::Disconnect();
}