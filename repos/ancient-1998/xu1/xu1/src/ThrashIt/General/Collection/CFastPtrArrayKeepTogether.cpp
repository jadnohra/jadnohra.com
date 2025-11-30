#ifndef _CFASTPTRARRAYKEEPTOGETHER_CPP__
#define _CFASTPTRARRAYKEEPTOGETHER_CPP__

#include "CFastPtrArrayKeepTogether.h"
#include "CFastPrimitiveArrayKeepTogether.cpp"



TEMPLATEFUNCTION
CFastPtrArrayKeepTogether<TEMPLATETYPE>::CFastPtrArrayKeepTogether()
{
	bAutoDelete=TRUE;
}

TEMPLATEFUNCTION
CFastPtrArrayKeepTogether<TEMPLATETYPE>::~CFastPtrArrayKeepTogether()
{
	Destroy();
}


TEMPLATEFUNCTION
VOID CFastPtrArrayKeepTogether<TEMPLATETYPE>::AutoDelete(BOOL Auto)
{
	bAutoDelete=Auto;
}

TEMPLATEFUNCTION
BOOL CFastPtrArrayKeepTogether<TEMPLATETYPE>::Autodelete()
{
	return(bAutoDelete);
}


TEMPLATEFUNCTION
BOOL CFastPtrArrayKeepTogether<TEMPLATETYPE>::DeletePtr(TEMPLATETYPE* ptr)
{
	WORD Index;
	
	Index=0;
	while(Index<UsedCount)
	{
		if(Array[Index++]==ptr)
		{
			if(bAutoDelete)
				delete ptr;
			Delete(Index-1);
			
			return(TRUE);
		}
	}
	
		return(FALSE);

}

TEMPLATEFUNCTION
VOID CFastPtrArrayKeepTogether<TEMPLATETYPE>::DeletePtr(WORD Index)
{
			if(bAutoDelete)
				delete Array[Index];
			Delete(Index);
}

TEMPLATEFUNCTION
VOID CFastPtrArrayKeepTogether<TEMPLATETYPE>::DeletePtrAll()
{
	WORD Index;

	if(Array && bAutoDelete)
	{
		Index=0;
		while(Index<UsedCount)
		{
			delete Array[Index];
			Index++;
		}
		
	}

	DeleteAll();

}



TEMPLATEFUNCTION
WORD CFastPtrArrayKeepTogether<TEMPLATETYPE>::PtrIndex(TEMPLATETYPE* ptr)
{
	WORD Index;

	if(Array)
	{
		Index=0;
		while(Index<UsedCount)
		{
			if(Array[Index]==ptr)
			{
				return(Index);
			}

			Index++;
		}
		
	}


	return(CFASTPRIMITIVEARRAYNULLINDEX);
}



TEMPLATEFUNCTION
VOID CFastPtrArrayKeepTogether<TEMPLATETYPE>::Destroy()
{
	if(bAutoDelete && Array)
	{
		
		RunIndex=0;
		while(RunIndex<UsedCount)
		{
			delete Array[RunIndex];
			Array[RunIndex++]=NULL;
			
		}
		
		
		
	}

}


TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPtrArrayKeepTogether<TEMPLATETYPE>::StartPtrForwardRun()
{
	if(!UsedCount)
		return(NULL);

	return(Array[(RunIndex=0)]);
}

TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPtrArrayKeepTogether<TEMPLATETYPE>::NextPtrForwardRun()
{
	if(++RunIndex==UsedCount)
		return(NULL);

	return(Array[RunIndex]);
}

TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPtrArrayKeepTogether<TEMPLATETYPE>::StartPtrBackwardRun()
{
	if(!UsedCount)
		return(NULL);

	return(Array[(RunIndex=(UsedCount-1))]);
}

TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPtrArrayKeepTogether<TEMPLATETYPE>::NextPtrBackwardRun()
{
	if(RunIndex==0)
		return(NULL);

	return(Array[--RunIndex]);
}



TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPtrArrayKeepTogether<TEMPLATETYPE>::FirstPtrElement()
{
	
	if(UsedCount)
		return(Array[0]);
	
	return(NULL);
}


TEMPLATEFUNCTION 
TEMPLATETYPE* CFastPtrArrayKeepTogether<TEMPLATETYPE>::LastPtrElement()
{
	
	if(UsedCount)
		return(Array[UsedCount-1]);
	
	return(NULL);
}



#endif