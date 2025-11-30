#ifndef _CFASTPRIMITIVEARRAYKEEPTOGETHER_CPP__
#define _CFASTPRIMITIVEARRAYKEEPTOGETHER_CPP__


#include "CFastPrimitiveArrayKeepTogether.h"


#define INDEXBYPTRKEEPTOGETHER(PTR) (WORD) (((TEMPLATETYPE*) (((DWORD) PTR)))-Array);


TEMPLATEFUNCTION
CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::CFastPrimitiveArrayKeepTogether()
{
	Array=NULL;
	
	FreeCount=0;
	AllocCount=0;
	UsedCount=0;
	RunIndex=0;
	bAutoSize=TRUE;
}

TEMPLATEFUNCTION
CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::~CFastPrimitiveArrayKeepTogether()
{
	Destroy();
}


TEMPLATEFUNCTION
VOID CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::DeleteAll()
{
	FreeCount=AllocCount;
	UsedCount=0;
}



TEMPLATEFUNCTION
VOID CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::Destroy()
{
	
	if(Array)
	{
		delete[] Array;
		Array=NULL;
		
	}
	
		
	FreeCount=0;
	AllocCount=0;
	UsedCount=0;
	RunIndex=0;
	
	
}


TEMPLATEFUNCTION
VOID CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::AutoSize(BOOL Auto)
{
	bAutoSize=Auto;
}

TEMPLATEFUNCTION
BOOL CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::AutoSize()
{
	return(	bAutoSize);
}



TEMPLATEFUNCTION 
BOOL FC CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::GrowBy(WORD Grow)
{
	TEMPLATETYPE* NewArrayKeepTogether=new TEMPLATETYPE[AllocCount+Grow];
	
	
	memcpy(NewArrayKeepTogether,Array,AllocCount*sizeof(TEMPLATETYPE));
	
	delete[] Array;
	Array=NewArrayKeepTogether;

	AllocCount+=Grow;
	FreeCount+=Grow;

	
	return(TRUE);
}

TEMPLATEFUNCTION 
BOOL CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::Create(WORD ElementCount,BOOL AutoSize)
{
	if(Array)
		Destroy();

	Array=new TEMPLATETYPE[ElementCount];
		
	AllocCount=ElementCount;
	bAutoSize=AutoSize;

	UsedCount=0;
	FreeCount=ElementCount;

	
	return(TRUE);
}


TEMPLATEFUNCTION 	
BOOL FC CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::Delete(WORD index)
{
	if(index>=UsedCount)
		return(FALSE);

	memcpy(&Array[index],&Array[index+1],((UsedCount-index)-1)*sizeof(TEMPLATETYPE));

		
	FreeCount++;
	UsedCount--;
	
	
	return(TRUE);
}


TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::DeleteForwardRun()
{
	if(!Delete(RunIndex))
		return(NULL);

	if(RunIndex==UsedCount)
		return(NULL);
	return(&Array[RunIndex]);
}



TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::DeleteBackwardRun()
{
	if(!Delete(RunIndex))
		return(NULL);

	if(RunIndex==0)
		return(NULL);
	return(&Array[--RunIndex]);
}


TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::New(WORD InsertIndex)
{
	if(!FreeCount && (bAutoSize && !GrowBy(AllocCount)) )
		return(NULL);

	if(InsertIndex>=UsedCount)
		return(New());

	memmove(&Array[InsertIndex+1],&Array[InsertIndex],(UsedCount-InsertIndex)*sizeof(TEMPLATETYPE));
	
	UsedCount++;
	FreeCount--;

	return(&Array[InsertIndex]);
	
}



TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::New()
{
	WORD Index;

	if(!FreeCount && (bAutoSize && !GrowBy(AllocCount)) )
		return(NULL);
	

	
	Index=UsedCount;
	UsedCount++;
	FreeCount--;

	return(&Array[Index]);
}


TEMPLATEFUNCTION 
TEMPLATETYPE* FC CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::Element(WORD ElementIndex)
{
		
	if(ElementIndex>UsedCount)
		return(NULL);

	
	return(&Array[ElementIndex]);	
}



TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::StartForwardRun()
{
	if(!UsedCount)
		return(NULL);

	return(&Array[(RunIndex=0)]);
}

TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::NextForwardRun()
{
	if(++RunIndex==UsedCount)
		return(NULL);

	return(&Array[RunIndex]);
}

TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::StartBackwardRun()
{
	if(!UsedCount)
		return(NULL);

	return(&Array[(RunIndex=(UsedCount-1))]);
}

TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::NextBackwardRun()
{
	if(RunIndex==0)
		return(NULL);

	return(&Array[--RunIndex]);
}

TEMPLATEFUNCTION 
BOOL FC CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::Delete(TEMPLATETYPE* lpElement)
{
	WORD Index=INDEXBYPTRKEEPTOGETHER(lpElement);
	
	return(Delete(Index));
}

TEMPLATEFUNCTION 
TEMPLATETYPE* FC CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::Next(TEMPLATETYPE* lpElement)
{
	WORD Index=INDEXBYPTRKEEPTOGETHER(lpElement);
	if(Index+1==UsedCount)
		return(NULL);

	return(Array[Index+1]);
}


TEMPLATEFUNCTION 
TEMPLATETYPE* FC CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::Previous(TEMPLATETYPE* lpElement)
{	
	WORD Index=INDEXBYPTRKEEPTOGETHER(lpElement);
	if(Index==0)
		return(NULL);

	return(Array[Index-1]);
}


TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::FirstElement()
{
	
	if(UsedCount)
		return(&Array[0]);
	
	return(NULL);
}


TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPrimitiveArrayKeepTogether<TEMPLATETYPE>::LastElement()
{
	
	if(UsedCount)
		return(&Array[UsedCount-1]);
	
	return(NULL);
}

#endif