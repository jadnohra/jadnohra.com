#include "TIZBuffer.h"


#define FLOATPTR			FLOAT*
#define BYTEP				BYTE*



TIZBuffer::TIZBuffer()
{
	Buffer = NULL;
	BufferFillIndicator = NULL;

}

TIZBuffer::~TIZBuffer()
{
	Disconnect();
}


VOID TIZBuffer::Reset()
{
	for(int i = 0; i < Height; i++)
	{
		memset(BufferFillIndicator[i],0,IndicatorWidthInBytes);
	}
}



BOOL TIZBuffer::Connect(int width,int height)
{

	int LineSizeInBytes;

	if(width <= 0 || height <= 0) 
	{
		return(FALSE);
	}

	Width = width + 1;
	Height = height + 1;

	Buffer = new FLOATPTR[Height];
	BufferFillIndicator = new BYTEP[Height];

	if(Buffer == NULL || BufferFillIndicator == NULL)
	{
		Disconnect();
		return(FALSE);
	}

	
	memset(Buffer,0,sizeof(FLOATPTR)*Height);
	memset(BufferFillIndicator,0,sizeof(BYTEP)*Height);

	LineSizeInBytes = sizeof(FLOAT)*Width;

	IndicatorWidthInBytes = Width;
#ifdef BITINDICATOR
	IndicatorWidthInBytes /= 8; 
#endif

	

	for(int i = 0; i < Height; i++)
	{
		Buffer[i] = new FLOAT[Width];
		BufferFillIndicator[i] = new BYTE[IndicatorWidthInBytes];		

		if(Buffer[i] == NULL || BufferFillIndicator[i] == NULL)
		{
			Disconnect();
			return(FALSE);
		}
		
		
	}

	Reset();

	
	return(TRUE);
}

VOID TIZBuffer::Disconnect()
{
	if(Buffer != NULL)
	{
		for(int i = 0; i < Height; i++)
		{
			if(Buffer[i] != NULL)
			{
				delete[] Buffer[i];
				Buffer[i] = NULL;
			}
		}

		delete[] Buffer;
		Buffer = NULL;
	}

	if(BufferFillIndicator != NULL)
	{
		for(int i = 0; i < Height; i++)
		{
			if(BufferFillIndicator[i] != NULL)
			{
				delete[] BufferFillIndicator[i];
				BufferFillIndicator[i] = NULL;
			}
		}

		delete[] BufferFillIndicator;
		BufferFillIndicator = NULL;
	}
}

/*
VOID TIZBuffer::_SETZVALUE(float ZVAL,FLOAT** ZBUFFER,BYTE** INDICATOR,int X,int Y)
{
	

	BYTE b = 1;
	int shift = (X%8);

	b = b << shift;


	ZBUFFER[Y][X]=ZVAL;

		
	INDICATOR[Y][X>>3]|=b;
	
	//{ZBUFFER[Y][X]=ZVAL;INDICATOR[Y][X>>3]|=((0x01) << (X%8));}
}

BOOL TIZBuffer::_SHOULDDRAWPIXEL(float ZVAL,FLOAT** ZBUFFER,BYTE** INDICATOR,int X,int Y)
{
	BYTE mask;
	BYTE temp;

	
	if(INDICATOR[Y][X>>3]==0) 
	{
		return(TRUE);
	}
	
	mask = TIZBuffer::BitMask[X%8];
	
	
	
	temp = (INDICATOR[Y][X>>3]) & mask;

	if(temp == 0) 
	{
		return(TRUE);
	}

	int z = ZBUFFER[Y][X];

	return(	ZVAL>z);
	

	//return ( ( (INDICATOR[Y][X>>3]==0)?TRUE:( ((INDICATOR[Y][X>>3]) & (0x01<<(X%8)) ) == 0?TRUE:ZVAL>ZBUFFER[Y][X]) ) );
	
	
}
*/