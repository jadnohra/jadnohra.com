#ifndef _THRASHIT3DZBUFFER_H__
#define _THRASHIT3DZBUFFER_H__

#include "..\..\General\_INCLUDES.h"

/*
comment this to make the indicator use a byte instead 
of 1 bit for each pixel
this will slow down clearing the buffer dramatically
but make checking and setting the buffer relativley 
slower, you might wanna try both configurations
*/
#define BITINDICATOR

#define TIZBUFFER				FLOAT**
#define TIZBUFFERINDICATOR		BYTE**

#define TIZBUFFER_RESET(BUFFER)			(BUFFER).Reset()
#define TIZBUFFER_GETBUFFER(BUFFER)		(BUFFER).Buffer
#define TIZBUFFER_GETINDICATOR(BUFFER)	(BUFFER).BufferFillIndicator

#ifdef BITINDICATOR
	#define TIZBUFFER_SETZVALUE(ZVAL,ZBUFFER,INDICATOR,X,Y)			{ZBUFFER[Y][X]=ZVAL;INDICATOR[Y][X>>3]|=((0x01) << (X%8));}
	#define TIZBUFFER_SHOULDDRAWPIXEL(ZVAL,ZBUFFER,INDICATOR,X,Y)	( ( (INDICATOR[Y][X>>3]==0)?TRUE:( ((INDICATOR[Y][X>>3]) & (0x01<<(X%8)) ) == 0?TRUE:ZVAL>ZBUFFER[Y][X]) ) )
#endif


#ifndef BITINDICATOR
	#define TIZBUFFER_SETZVALUE(ZVAL,ZBUFFER,INDICATOR,X,Y)			{ZBUFFER[Y][X]=ZVAL;INDICATOR[Y][X]=1;}
	#define TIZBUFFER_SHOULDDRAWPIXEL(ZVAL,ZBUFFER,INDICATOR,X,Y)	(INDICATOR[Y][X]==0?TRUE:(ZVAL>ZBUFFER[Y][X]))
#endif

struct TIZBuffer
{
	FLOAT** Buffer; //Buffer[y][x] (better cache hits ?)
	BYTE**	BufferFillIndicator;//0 no pixel 1 pixel filled	

	int Width;
	int Height;

#ifdef THRASHIT3D_COUNTZOVERDRAW
	LONG Overdraw;
#endif
	int IndicatorWidthInBytes;
	
	TIZBuffer();
	~TIZBuffer();
	
	BOOL Connect(int width,int height);
	VOID Disconnect();

	VOID Reset();

	inline VOID memset16Bit(LPWORD dst);

	//static VOID _SETZVALUE(float ZVAL,FLOAT** ZBUFFER,BYTE** INDICATOR,int X,int Y);			
	//static BOOL _SHOULDDRAWPIXEL(float ZVAL,FLOAT** ZBUFFER,BYTE** INDICATOR,int X,int Y);	

};


#endif