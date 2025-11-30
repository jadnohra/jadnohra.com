#ifndef _TIVERTEX_H__
#define _TIVERTEX_H__

struct TIVertex
{
	TIBasicVertex World;
	TIBasicVertex Aligned;

	D2Vector<float> Screen;
	
	CDXColor color;	//gouraud
};

#endif