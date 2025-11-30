#include "ZOrderArray.h"

#include "CFastPtrArrayKeepTogether.cpp"
#include "CFastPrimitiveArrayKeepTogether.cpp"


TEMPLATEFUNCTION 
BOOL ZOrderArray<TEMPLATETYPE>::Create(WORD initialElementCount)
{
	ZArray.Create(initialElementCount);
	ZActions.Create(initialElementCount);

	ZArray.AutoDelete(FALSE);
	
	return(TRUE);
}

TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::Destroy()
{
	ZArray.Destroy();
	ZActions.Destroy();
}


TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::Lock()
{
	Locked=TRUE;
}

TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::UnLock()
{
	Locked=FALSE;
	ExecuteActions();
}

	


TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::Delete(TEMPLATETYPE* lpElement)
{
	
	
	if(Locked)
	{
		ZOrderAction* lpAction=ZActions.New();
		lpAction->action=_Delete;
		lpAction->lpObject=lpElement;
	}
	else
	{		
		WORD Index=ZArray.PtrIndex(lpElement);
		if(Index==CFASTPRIMITIVEARRAYNULLINDEX)
			return;
		
		DeleteNow(Index);
	}


}
	

TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::Change(TEMPLATETYPE* lpElement,int NewZOrder)
{
	
	
	if(Locked)
	{
		ZOrderAction* lpAction=ZActions.New();
		lpAction->action=_Change;
		lpAction->index=Index;
		lpAction->wcNewZOrder=NewZOrder;
	}
	else
	{
		WORD Index=ZArray.PtrIndex(lpElement);
	
		if(Index==CFASTPRIMITIVEARRAYNULLINDEX)
			return;
	
		ChangeNow(Index,NewZOrder);
	}


}


TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::Delete(WORD index)
{
	
	if(index>=ZArray.UsedCount)
			return;

	if(Locked)
	{
		ZOrderAction* lpAction=ZActions.New();
		lpAction->action=_Delete;
		lpAction->lpObject=ZArray.Array[index];
	}
	else
	{		
		DeleteNow(index);
	}


}
	

TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::Change(WORD index,int NewZOrder)
{
	
	if(index>=ZArray.UsedCount)
		return;
	
	if(Locked)
	{
		ZOrderAction* lpAction=ZActions.New();
		lpAction->action=_Change;
		lpAction->lpObject=ZArray.Array[index];
		lpAction->wcNewZOrder=NewZOrder;
	}
	else
	{
		ChangeNow(index,NewZOrder);
	}


}



TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::Add(TEMPLATETYPE* lpElement)
{
	
	if(Locked)
	{
		ZOrderAction* lpAction=ZActions.New();
		lpAction->action=_Add;
		lpAction->lpObject=lpElement;
	}
	else
	{
		AddNow(lpElement);
	}

}


	
TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::DeleteNow(int index)
{
	memcpy(&ZArray.Array[index],&ZArray.Array[index+1],((ZArray.UsedCount-index)-1)*sizeof(TEMPLATETYPE*));
	ZArray.UsedCount--;
	ZArray.FreeCount++;
}
	






TEMPLATEFUNCTION 
int ZOrderArray<TEMPLATETYPE>::SearchForPlace(int currZ,int newZ,int currIndex)
{
	if(currZ==newZ)
		return(currIndex);
	
	int index=currIndex;
	
	
	if(currZ>newZ)
	{
		index++;
		while(index<ZArray.UsedCount)
		{
			if(ZArray.Array[index]->wcZOrder<=newZ)
				return(index-1);
			index++;
			
		}
		return(index);
	}
	else
	{
		index--;
		while(index>=0)
		{
			if(ZArray.Array[index]->wcZOrder>=newZ)
				return(index+1);
			index--;
			
		}
		return(index);
	}


}


TEMPLATEFUNCTION 
int ZOrderArray<TEMPLATETYPE>::SearchForNewPlace(int newZ,int currIndex)
{
	int index=currIndex;
	
	
	index--;
	while(index>=0)
	{
		if(ZArray.Array[index]->wcZOrder<=newZ)
			return(index+1);
		index--;
			
	}
	return(0);
	

}




TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::ChangeNow(int index,int NewZOrder)
{
	int newPlace=SearchForPlace(ZArray.Array[index]->wcZOrder,NewZOrder,index);

	if(newPlace==index)
		return;

	if(newPlace>index)
	{
		TEMPLATETYPE* lpMove=ZArray.Array[index];
		memcpy(&ZArray.Array[index],&ZArray.Array[index],((newPlace-index)-1)*sizeof(TEMPLATETYPE*));
		ZArray.Array[newPlace]=lpMove;
	}
	else
	{
		TEMPLATETYPE* lpMove=ZArray.Array[index];
		memcpy(&ZArray.Array[newPlace+1],&ZArray.Array[newPlace],((index-newPlace)-1)*sizeof(TEMPLATETYPE*));
		ZArray.Array[newPlace]=lpMove;
	}


}

TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::AddNow(TEMPLATETYPE* lpElement)
{
	*(ZArray.New())=lpElement;

	int index=ZArray.UsedCount-1;
	int newPlace=SearchForNewPlace(ZArray.Array[index]->wcZOrder,index);

	if(newPlace==index)
		return;

	TEMPLATETYPE* lpMove=ZArray.Array[index];
	memcpy(&(ZArray.Array[newPlace+1]),&(ZArray.Array[newPlace]),(index-newPlace)*sizeof(TEMPLATETYPE*));
	ZArray.Array[newPlace]=lpMove;

}
	
	

TEMPLATEFUNCTION 
VOID ZOrderArray<TEMPLATETYPE>::ExecuteActions()
{
	int index;

	for(int i=0;i<ZActions.UsedCount;i++)
	{
		switch(ZActions.Array[i].action)
		{
		case _Delete:
			index=ZArray.PtrIndex(ZActions.Array[i].lpObject);
			if(index!=CFASTPRIMITIVEARRAYNULLINDEX)
				DeleteNow(index);
			break;
		case _Change:
			index=ZArray.PtrIndex(ZActions.Array[i].lpObject);
			if(index!=CFASTPRIMITIVEARRAYNULLINDEX)
				ChangeNow(index,ZActions.Array[i].wcNewZOrder);
			break;
		case _Add:
			AddNow(ZActions.Array[i].lpObject);
			break;

		}

	}

	ZActions.UsedCount=0;
	ZActions.FreeCount=ZActions.AllocCount;
}

