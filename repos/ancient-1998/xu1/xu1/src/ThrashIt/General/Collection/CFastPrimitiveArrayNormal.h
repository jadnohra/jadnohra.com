#ifndef _CFASTPRIMITIVEARRAYNORMAL_H__
#define _CFASTPRIMITIVEARRAYNORMAL_H__

#include "..\_INCLUDES.h"

#define TEMPLATECLASS template<class TEMPLATETYPE> class
#define TEMPLATEFUNCTION template<class TEMPLATETYPE> 

#define CFASTPRIMITIVEARRAYNULLINDEX	0xFFFF

TEMPLATECLASS CFastPrimitiveArrayNormal
{
	struct CFastPrimitiveArrayElementNormal 
	{
	public:	
		WORD	_prevIndex;
		WORD	_nextIndex;
		BYTE	_valid;
		TEMPLATETYPE _data;
	};

	
public:
	CFastPrimitiveArrayNormal();
	virtual ~CFastPrimitiveArrayNormal();
	

	virtual VOID Destroy();
	
	VOID AutoSize(BOOL Auto);
	BOOL AutoSize();
		
	BOOL FC GrowBy(WORD Grow);
	
	BOOL Create(WORD ElementCount,BOOL AutoSize=TRUE);
	
	VOID DeleteAll();
	BOOL FC Delete(WORD Index);
	
	TEMPLATETYPE* FirstElement();
	TEMPLATETYPE* LastElement();
	BOOL FC Delete(TEMPLATETYPE* lpElement);
	TEMPLATETYPE* FC Next(TEMPLATETYPE* lpElement);
	TEMPLATETYPE* FC Previous(TEMPLATETYPE* lpElement);	

	TEMPLATETYPE* FC Element(WORD ElementIndex);

	TEMPLATETYPE* New();

	TEMPLATETYPE* StartForwardRun();
	TEMPLATETYPE* NextForwardRun();
	TEMPLATETYPE* DeleteForwardRun();
	TEMPLATETYPE* StartBackwardRun();
	TEMPLATETYPE* NextBackwardRun();
	TEMPLATETYPE* DeleteBackwardRun();

	

	CFastPrimitiveArrayElementNormal* Array;
			
	BOOL bAutoSize;
	WORD RunIndex;

	WORD* FreeIndexes;

	WORD First;
	WORD Last;

	WORD AllocCount;
	WORD FreeCount;
	WORD UsedCount;

};



#endif
