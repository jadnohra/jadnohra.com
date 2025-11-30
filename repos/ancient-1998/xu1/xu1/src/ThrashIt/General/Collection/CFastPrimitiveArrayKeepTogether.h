#ifndef _CFASTPRIMITIVEARRAYKEEPTOGETHER_H__
#define _CFASTPRIMITIVEARRAYKEEPTOGETHER_H__

#include "..\_INCLUDES.h"



#define CFASTPRIMITIVEARRAYNULLINDEX	0xFFFF

TEMPLATECLASS CFastPrimitiveArrayKeepTogether
{

	
public:
	CFastPrimitiveArrayKeepTogether();
	virtual ~CFastPrimitiveArrayKeepTogether();
	

	virtual VOID Destroy();
	
	VOID AutoSize(BOOL Auto);
	BOOL AutoSize();
		
	BOOL FC GrowBy(WORD Grow);
	
	BOOL Create(WORD ElementCount,BOOL AutoSize=TRUE);
	
	
	BOOL FC Delete(WORD Index);
	VOID DeleteAll();
	
	TEMPLATETYPE* FirstElement();
	TEMPLATETYPE* LastElement();
	BOOL FC Delete(TEMPLATETYPE* lpElement);
	TEMPLATETYPE* FC Next(TEMPLATETYPE* lpElement);
	TEMPLATETYPE* FC Previous(TEMPLATETYPE* lpElement);	

	TEMPLATETYPE* FC Element(WORD ElementIndex);

	TEMPLATETYPE* New();
	TEMPLATETYPE* New(WORD InsertIndex);

	TEMPLATETYPE* StartForwardRun();
	TEMPLATETYPE* NextForwardRun();
	TEMPLATETYPE* DeleteForwardRun();
	TEMPLATETYPE* StartBackwardRun();
	TEMPLATETYPE* NextBackwardRun();
	TEMPLATETYPE* DeleteBackwardRun();

	

	TEMPLATETYPE* Array;
			
	BOOL bAutoSize;
	WORD RunIndex;


	WORD AllocCount;
	WORD FreeCount;
	WORD UsedCount;

};



#endif
