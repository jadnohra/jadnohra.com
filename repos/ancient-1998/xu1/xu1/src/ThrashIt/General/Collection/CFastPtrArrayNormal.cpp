#ifndef _CFASTPTRARRAYNORMAL_CPP__
#define _CFASTPTRARRAYNORMAL_CPP__

#include "CFastPtrArrayNormal.h"
#include "CFastPrimitiveArrayNormal.cpp"

TEMPLATEFUNCTION
CFastPtrArrayNormal<TEMPLATETYPE>::CFastPtrArrayNormal()
{
	bAutoDelete=TRUE;
}

TEMPLATEFUNCTION
CFastPtrArrayNormal<TEMPLATETYPE>::~CFastPtrArrayNormal()
{
	Destroy();
}


TEMPLATEFUNCTION
VOID CFastPtrArrayNormal<TEMPLATETYPE>::AutoDelete(BOOL Auto)
{
	bAutoDelete=Auto;
}

TEMPLATEFUNCTION
BOOL CFastPtrArrayNormal<TEMPLATETYPE>::Autodelete()
{
	return(bAutoDelete);
}


TEMPLATEFUNCTION
BOOL CFastPtrArrayNormal<TEMPLATETYPE>::DeletePtr(TEMPLATETYPE* ptr)
{
	WORD Index;
/*
	if(ArrayKeepTogether)
	{
		Index=0;
		while(Index<UsedCount)
		{
			if(ArrayKeepTogether[Index++]==ptr)
			{
				if(bAutoDelete)
					delete ptr;
				Delete(Index);
				
				return(TRUE);
			}
		}
		
		return(FALSE);
	}
	*/
	Index=First;
	while(Index!=CFASTPRIMITIVEARRAYNULLINDEX)
	{
		if(Array[Index]._data==ptr)
		{
			if(bAutoDelete)
				delete ptr;
			Delete(Index);
			
			return(TRUE);
		}
		
		Index=Array[Index]._nextIndex;
	}
	

	return(FALSE);
}

TEMPLATEFUNCTION
WORD CFastPtrArrayNormal<TEMPLATETYPE>::PtrIndex(TEMPLATETYPE* ptr)
{
	WORD Index;

	if(Array)
	{
		Index=First;
		while(Index!=CFASTPRIMITIVEARRAYNULLINDEX)
		{
			if(ArrayNormal[Index]._data==ptr)
			{
				return(Index);
			}

			Index=ArrayNormal[Index]._nextIndex;
		}
		
	}


	return(CFASTPRIMITIVEARRAYNULLINDEX);
}


TEMPLATEFUNCTION
VOID CFastPtrArrayNormal<TEMPLATETYPE>::DeletePtrAll()
{
	WORD Index;
	
	if(UsedCount!=0)
	{
		Index=First;

		While(Index!=CFASTPRIMITIVEARRAYNULLINDEX)
		{
			delete Array[Index]._data;
			Index=Array[Index]._nextIndex;
		}

	}


	FreeCount=AllocCount;
	UsedCount=0;
}


TEMPLATEFUNCTION
VOID CFastPtrArrayNormal<TEMPLATETYPE>::Destroy()
{
	if(bAutoDelete && Array)
	{
		RunIndex=First;
		
		while(RunIndex!=CFASTPRIMITIVEARRAYNULLINDEX)
		{
			delete ArrayNormal[RunIndex]._data;
			ArrayNormal[RunIndex]._data=NULL;
			RunIndex=ArrayNormal[RunIndex]._nextIndex;
		}
		
		
		/*if(ArrayKeepTogether)
		{
			RunIndex=0;
			if(RunIndex<UsedCount)
			{
				ArrayKeepTogether[RunIndex]=NULL;
				delete ArrayKeepTogether[RunIndex++];
			}

		}*/


	
	}

}


TEMPLATEFUNCTION
TEMPLATETYPE* CFastPtrArrayNormal<TEMPLATETYPE>::StartPtrForwardRun()
{
	if((RunIndex=First)==CFASTPRIMITIVEARRAYNULLINDEX)
		return(NULL);

	return(Array[RunIndex]._data);
}

TEMPLATEFUNCTION
TEMPLATETYPE* CFastPtrArrayNormal<TEMPLATETYPE>::NextPtrForwardRun()
{
	if((RunIndex=Array[RunIndex]._nextIndex)==CFASTPRIMITIVEARRAYNULLINDEX)
		return(NULL);

	return(Array[RunIndex]._data);
}

TEMPLATEFUNCTION
TEMPLATETYPE* CFastPtrArrayNormal<TEMPLATETYPE>::StartPtrBackwardRun()
{
	if((RunIndex=Last)==CFASTPRIMITIVEARRAYNULLINDEX)
		return(NULL);

	return(Array[RunIndex]._data);
}

TEMPLATEFUNCTION
TEMPLATETYPE* CFastPtrArrayNormal<TEMPLATETYPE>::NextPtrBackwardRun()
{
	if((RunIndex=Array[RunIndex]._prevIndex)==CFASTPRIMITIVEARRAYNULLINDEX)
		return(NULL);

	return(Array[RunIndex]._data);
}



TEMPLATEFUNCTION
TEMPLATETYPE* CFastPtrArrayNormal<TEMPLATETYPE>::DeletePtrForwardRun()
{
	if(!Delete(RunIndex))
		return(NULL);

	if((RunIndex=Array[RunIndex]._nextIndex)!=CFASTPRIMITIVEARRAYNULLINDEX)
		return(Array[RunIndex]._data);
	return(NULL);
}
	



TEMPLATEFUNCTION
TEMPLATETYPE* CFastPtrArrayNormal<TEMPLATETYPE>::DeletePtrBackwardRun()
{
	if(!Delete(RunIndex))
		return(NULL);

	if((RunIndex=Array[RunIndex]._prevIndex)!=CFASTPRIMITIVEARRAYNULLINDEX)
		return(Array[RunIndex]._data);
	return(NULL);
}


TEMPLATEFUNCTION
TEMPLATETYPE* CFastPtrArrayNormal<TEMPLATETYPE>::FirstPtrElement()
{
	
	if(First!=NULLINDEX)
		return((Array[First]._data));
	
	return(NULL);
}


TEMPLATEFUNCTION
TEMPLATETYPE* CFastPtrArrayNormal<TEMPLATETYPE>::LastPtrElement()
{
	
	if(Last!=NULLINDEX)
		return((Array[Last]._data));
	
	return(NULL);
}

#endif