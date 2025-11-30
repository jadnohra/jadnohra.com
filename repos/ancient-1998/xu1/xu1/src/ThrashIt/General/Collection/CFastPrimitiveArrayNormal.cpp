#ifndef _CFASTPRIMITIVEARRAYNORMAL_CPP__
#define _CFASTPRIMITIVEARRAYNORMAL_CPP__

#include "CFastPrimitiveArrayNormal.h"


#define INDEXBYPTRNORMAL(PTR) (WORD) (((CFastPrimitiveArrayElementNormal*) (((DWORD) PTR)-(sizeof(CFastPrimitiveArrayElementNormal)-sizeof(TEMPLATETYPE))))-Array);


TEMPLATEFUNCTION
CFastPrimitiveArrayNormal<TEMPLATETYPE>::CFastPrimitiveArrayNormal()
{
	Array=NULL;
	FreeIndexes=NULL;
	
	FreeCount=0;
	AllocCount=0;
	UsedCount=0;
	RunIndex=0;
	
	First=CFASTPRIMITIVEARRAYNULLINDEX;
	Last=CFASTPRIMITIVEARRAYNULLINDEX;
}

TEMPLATEFUNCTION
CFastPrimitiveArrayNormal<TEMPLATETYPE>::~CFastPrimitiveArrayNormal()
{
	Destroy();
}


TEMPLATEFUNCTION
VOID CFastPrimitiveArrayNormal<TEMPLATETYPE>::DeleteAll()
{
	WORD Index;
	
	if(UsedCount!=0)
	{
		Index=First;

		While(Index!=CFASTPRIMITIVEARRAYNULLINDEX)
		{
			Array[Index]._valid=FALSE;
			Index=Array[Index]._nextIndex;
		}

	}


	FreeCount=AllocCount;
	UsedCount=0;
}


TEMPLATEFUNCTION
VOID CFastPrimitiveArrayNormal<TEMPLATETYPE>::Destroy()
{
	
	if(Array)
	{
		delete[] Array;
		Array=NULL;
		
	}
	
	if(FreeIndexes)
	{
		delete[] FreeIndexes;
		FreeIndexes=NULL;
		
	}
	
	
	FreeCount=0;
	AllocCount=0;
	UsedCount=0;
	RunIndex=0;
	
	First=CFASTPRIMITIVEARRAYNULLINDEX;
	Last=CFASTPRIMITIVEARRAYNULLINDEX;
	
}


TEMPLATEFUNCTION
VOID CFastPrimitiveArrayNormal<TEMPLATETYPE>::AutoSize(BOOL Auto)
{
	bAutoSize=Auto;
}

TEMPLATEFUNCTION
BOOL CFastPrimitiveArrayNormal<TEMPLATETYPE>::AutoSize()
{
	return(	bAutoSize);
}



TEMPLATEFUNCTION
BOOL CFastPrimitiveArrayNormal<TEMPLATETYPE>::Create(WORD ElementCount,BOOL AutoSize)
{
	if(Array)
		Destroy();

	Array=new CFastPrimitiveArrayElementNormal[ElementCount];
	FreeIndexes=new WORD[ElementCount];
	
	AllocCount=ElementCount;
	bAutoSize=AutoSize;

	UsedCount=0;
	FreeCount=ElementCount;

	for(int i=0;i<ElementCount;i++)
	{
		FreeIndexes[i]=(ElementCount-i)-1;
	}

		
	First=CFASTPRIMITIVEARRAYNULLINDEX;
	Last=CFASTPRIMITIVEARRAYNULLINDEX;

	return(TRUE);
}

TEMPLATEFUNCTION
BOOL FC CFastPrimitiveArrayNormal<TEMPLATETYPE>::GrowBy(WORD Grow)
{
	CFastPrimitiveArrayElementNormal* NewArrayNormal=new CFastPrimitiveArrayElementNormal[AllocCount+Grow];
	WORD* NewFreeIndexes=new WORD[AllocCount+Grow];

	
	memcpy(NewArrayNormal,Array,AllocCount*sizeof(CFastPrimitiveArrayElementNormal));
	memcpy(&(NewFreeIndexes[Grow]),NewFreeIndexes,FreeCount*sizeof(WORD));

	delete[] FreeIndexes;
	delete[] Array;

	FreeIndexes=NewFreeIndexes;
	Array=NewArrayNormal;

	
	AllocCount+=Grow;
	FreeCount+=Grow;

	for(int i=0;i<Grow;i++)
	{
		FreeIndexes[i]=(AllocCount-i)-1;		
	}

	
	return(TRUE);
}



TEMPLATEFUNCTION	
BOOL FC CFastPrimitiveArrayNormal<TEMPLATETYPE>::Delete(WORD Index)
{
	if(Index>AllocCount || !Array[Index]._valid)
		return(FALSE);

	
	Array[Index]._valid=FALSE;
	FreeIndexes[FreeCount++]=Index;
	UsedCount--;
	

	if(First==Index)
		First=Array[Index]._nextIndex;
	if(Last==Index)
		Last=Array[Index]._prevIndex;

	if(Array[Index]._prevIndex!=CFASTPRIMITIVEARRAYNULLINDEX)
		Array[Array[Index]._prevIndex]._nextIndex=Array[Index]._nextIndex;
	if(Array[Index]._nextIndex!=CFASTPRIMITIVEARRAYNULLINDEX)
		Array[Array[Index]._nextIndex]._prevIndex=Array[Index]._prevIndex;

	
	return(TRUE);
}








TEMPLATEFUNCTION
TEMPLATETYPE* CFastPrimitiveArrayNormal<TEMPLATETYPE>::DeleteForwardRun()
{
	if(!Delete(RunIndex))
		return(NULL);

	if((RunIndex=Array[RunIndex]._nextIndex)!=CFASTPRIMITIVEARRAYNULLINDEX)
		return(&Array[RunIndex]._data);
	return(NULL);
}
	






TEMPLATEFUNCTION
TEMPLATETYPE* CFastPrimitiveArrayNormal<TEMPLATETYPE>::DeleteBackwardRun()
{
	if(!Delete(RunIndex))
		return(NULL);

	if((RunIndex=Array[RunIndex]._prevIndex)!=CFASTPRIMITIVEARRAYNULLINDEX)
		return(&Array[RunIndex]._data);
	return(NULL);
}





TEMPLATEFUNCTION
TEMPLATETYPE* CFastPrimitiveArrayNormal<TEMPLATETYPE>::New()
{
	WORD Index;

	if(!FreeCount && (bAutoSize && !GrowBy(AllocCount)) )
		return(NULL);
	

	
	Index=FreeIndexes[--FreeCount];
	Array[Index]._valid=TRUE;
	UsedCount++;
	

	if(Last!=CFASTPRIMITIVEARRAYNULLINDEX)
	{
		Array[Last]._nextIndex=Index;	
		Array[Index]._prevIndex=Last;
		Array[Index]._nextIndex=CFASTPRIMITIVEARRAYNULLINDEX;
	}
	else
	{
		Array[Index]._prevIndex=CFASTPRIMITIVEARRAYNULLINDEX;
		Array[Index]._nextIndex=CFASTPRIMITIVEARRAYNULLINDEX;
		First=Index;
	}

	Last=Index;
	
	return(&Array[Index]._data);
}



TEMPLATEFUNCTION
TEMPLATETYPE* FC CFastPrimitiveArrayNormal<TEMPLATETYPE>::Element(WORD ElementIndex)
{
	WORD RunIndex;
	WORD RunCounter=0;
	
	
	if(ElementIndex>UsedCount)
		return(NULL);

	if((RunIndex=First)==CFASTPRIMITIVEARRAYNULLINDEX)
		return(NULL);


	while(RunCounter!=ElementIndex)
	{
		RunIndex=Array[RunIndex]._nextIndex;
		RunCounter++;
	}

	return(&Array[RunIndex]._data);	
	
}





TEMPLATEFUNCTION
TEMPLATETYPE* CFastPrimitiveArrayNormal<TEMPLATETYPE>::StartForwardRun()
{
	if((RunIndex=First)==CFASTPRIMITIVEARRAYNULLINDEX)
		return(NULL);

	return(&Array[RunIndex]._data);
}

TEMPLATEFUNCTION
TEMPLATETYPE* CFastPrimitiveArrayNormal<TEMPLATETYPE>::NextForwardRun()
{
	if((RunIndex=Array[RunIndex]._nextIndex)==CFASTPRIMITIVEARRAYNULLINDEX)
		return(NULL);

	return(&Array[RunIndex]._data);
}

TEMPLATEFUNCTION
TEMPLATETYPE* CFastPrimitiveArrayNormal<TEMPLATETYPE>::StartBackwardRun()
{
	if((RunIndex=Last)==CFASTPRIMITIVEARRAYNULLINDEX)
		return(NULL);

	return(&Array[RunIndex]._data);
}

TEMPLATEFUNCTION
TEMPLATETYPE* CFastPrimitiveArrayNormal<TEMPLATETYPE>::NextBackwardRun()
{
	if((RunIndex=Array[RunIndex]._prevIndex)==CFASTPRIMITIVEARRAYNULLINDEX)
		return(NULL);

	return(&Array[RunIndex]._data);
}

TEMPLATEFUNCTION
BOOL FC CFastPrimitiveArrayNormal<TEMPLATETYPE>::Delete(TEMPLATETYPE* lpElement)
{
	WORD Index=INDEXBYPTRNORMAL(lpElement);
	
	return(Delete(Index));
}

TEMPLATEFUNCTION
TEMPLATETYPE* FC CFastPrimitiveArrayNormal<TEMPLATETYPE>::Next(TEMPLATETYPE* lpElement)
{
	WORD Index=INDEXBYPTRNORMAL(lpElement);
	WORD NextIndex;

	if(Index>AllocCount)
		return(NULL);
	NextIndex=Array[Index]._nextIndex;

	if(NextIndex!=NULLINDEX)
		return(&(Array[NextIndex]._data));
	
	return(NULL);
}


TEMPLATEFUNCTION
TEMPLATETYPE* FC CFastPrimitiveArrayNormal<TEMPLATETYPE>::Previous(TEMPLATETYPE* lpElement)
{	
	WORD Index=INDEXBYPTRNORMAL(lpElement);
	WORD PrevIndex;

	if(Index>AllocCount)
		return(NULL);
	PrevIndex=Array[Index]._prevIndex;

	if(PrevIndex!=NULLINDEX)
		return(&(Array[PrevIndex]._data));
	
	return(NULL);
}


TEMPLATEFUNCTION
TEMPLATETYPE* CFastPrimitiveArrayNormal<TEMPLATETYPE>::FirstElement()
{
	
	if(First!=NULLINDEX)
		return(&(Array[First]._data));
	
	return(NULL);
}


TEMPLATEFUNCTION
TEMPLATETYPE* CFastPrimitiveArrayNormal<TEMPLATETYPE>::LastElement()
{
	
	if(Last!=NULLINDEX)
		return(&(Array[Last]._data));
	
	return(NULL);
}




#endif