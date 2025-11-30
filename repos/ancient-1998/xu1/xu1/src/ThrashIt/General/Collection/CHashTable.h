// CHashTable.h: interface for the CHashTable class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CHASHTABLE_H__
#define _CHASHTABLE_H__

#include "..\_INCLUDES.h"

#define TCLASS template<class TClass>


/*
find a better hash than % e.g( | !size ) & size
*/

TCLASS class CHashTable
{

//the class for the hash table elements containing the data
struct CHashElement
{
	DWORD	KEY;
	TClass*	DATAPTR;
};


protected:

CHashElement*	HashArray;
LONG		lFreeCount;
LONG		lHashSize;
LONG		lSizeIncrement;
BOOL		bAutoResize;
BOOL		bAutoDelete;	//should we auto delete the objects when we die?	

public:
	DWORD		NULLKEY;
	TClass*		NULLDATA;

	CHashTable()
	{
		NULLKEY=0xffffffff;
		NULLDATA=NULL;
		lFreeCount=0;
		lHashSize=0;
		lSizeIncrement=0;
		bAutoResize=TRUE;
		bAutoDelete=TRUE;
		HashArray=NULL;
	}


	
	virtual ~CHashTable()
	{
		Free();
	}

	VOID FC Free()
	{
		if(HashArray)
		{
			
			if(bAutoDelete)
			{
				ULONG ulCount=lHashSize-lFreeCount;
	
				if(ulCount!=0)
				{					
				
					LONG dwHashPosition=0;
						
					while(dwHashPosition<lHashSize)
					{
						if(HashArray[dwHashPosition].KEY!=NULLKEY)
							delete HashArray[dwHashPosition].DATAPTR;

						dwHashPosition++;
					}
				}
	
			}

				
			delete[] HashArray;
			HashArray=NULL;
		}

		lFreeCount=0;
		lHashSize=0;
		lSizeIncrement=0;
	}



	DWORD FC Init(ULONG ulSize)
	{
		return(Init(ulSize,ulSize/2));
	}

	DWORD FC Init(ULONG ulSize,ULONG ulSizeIncrement)
	{
		LONG dwHashPosition;

		if(HashArray)
			delete[]HashArray;
		
		HashArray=new CHashElement[ulSize];
			
		
		lFreeCount=ulSize;
		lHashSize=lFreeCount;		//size of hash table
		this->lSizeIncrement=ulSizeIncrement;
		if(this->lSizeIncrement==0)
			this->lSizeIncrement=1;
			
		dwHashPosition=0;
		while(dwHashPosition<lFreeCount)
		{
			HashArray[dwHashPosition].KEY=NULLKEY;
			HashArray[dwHashPosition].DATAPTR=NULLDATA;
			dwHashPosition++;
		}

		return(S_SUCCESS);
	}



	DWORD FC Init(ULONG ulSize,DWORD dwNullKey,TClass* lpNullData)
	{
		
		NULLKEY=dwNullKey;
		NULLDATA=lpNullData;
		return(Init(ulSize));
	}


	DWORD FC Init(ULONG ulSize,ULONG ulSizeIncrement,DWORD dwNullKey,TClass* lpNullData)
	{
		
		NULLKEY=dwNullKey;
		NULLDATA=lpNullData;
		return(Init(ulSize,ulSizeIncrement));
	}


	VOID FC AutoReSize(BOOL bAuto)
	{
		bAutoResize=bAuto;
	}

	BOOL FC AutoReSize()	 
	{
		return(bAutoResize);
	}

	VOID FC AutoDelete(BOOL bAuto)
	{
		bAutoDelete=bAuto;
	}

	BOOL FC AutoDelete()	 
	{
		return(bAutoDelete);
	}

	DWORD FC Insert(DWORD dwKey,TClass* lpData)
	{
		if(lFreeCount==0)
		{
			if(bAutoResize)
				ReSize(lHashSize+lSizeIncrement);	//and continue
			else
				return(S_FAILED);
		}

		//we have room => surely no loops
		DWORD dwHashPostition=dwKey%lHashSize;
		

		while(HashArray[dwHashPostition].KEY!=NULLKEY)
		{
			++dwHashPostition%=lHashSize;	//increase the pos. and modulo size
		}


		HashArray[dwHashPostition].KEY=dwKey;
		HashArray[dwHashPostition].DATAPTR=lpData;
		lFreeCount--;
		
		return(S_SUCCESS);
	}


	TClass* FC Get(DWORD dwKey)
	{
		if(!lHashSize)
			return(NULL);

		DWORD dwHashPostition=dwKey%lHashSize;
		DWORD dwFirstTestHashPosition=dwHashPostition;	

		
		while(HashArray[dwHashPostition].KEY!=dwKey)
		{
			++dwHashPostition%=lHashSize;	//increase the pos. and modulo size
			if(dwHashPostition==dwFirstTestHashPosition)
				return(NULLDATA);	//we looped and didnt find it
		}

		return(HashArray[dwHashPostition].DATAPTR);

	}

	
	BOOL FC Set(DWORD dwKey,TClass* lpData)
	{
		if(!lHashSize)
			return(FALSE);

		DWORD dwHashPostition=dwKey%lHashSize;
		DWORD dwFirstTestHashPosition=dwHashPostition;	

		
		while(HashArray[dwHashPostition].KEY!=dwKey)
		{
			++dwHashPostition%=lHashSize;	//increase the pos. and modulo size
			if(dwHashPostition==dwFirstTestHashPosition)
				return(FALSE);	//we looped and didnt find it
		}

		if(bAutoDelete)
			delete HashArray[dwHashPostition].DATAPTR;

		HashArray[dwHashPostition].DATAPTR=lpData;
		return(TRUE);

	}


	DWORD FC Delete(DWORD dwKey)
	{
		DWORD dwHashPostition=dwKey%lHashSize;
		DWORD dwFirstTestHashPosition=dwHashPostition;	

		
		while(HashArray[dwHashPostition].KEY!=dwKey)
		{
			++dwHashPostition%=lHashSize;	//increase the pos. and modulo size
			if(dwHashPostition==dwFirstTestHashPosition)
				return(S_FAILED);	//we looped and didnt find it
		}

		HashArray[dwHashPostition].KEY=NULLKEY;
		
		if(bAutoDelete)
			delete HashArray[dwHashPostition].DATAPTR;

		HashArray[dwHashPostition].DATAPTR=NULLDATA;
		lFreeCount++;
		
		return(S_SUCCESS);
	}



	TClass** FC GetAllData(DWORD& dwRetSize)
	{
		
		ULONG ulCount=lHashSize-lFreeCount;
		

		if(ulCount==0)
		{
			dwRetSize=0;
			return(NULL);
		}
		
		TClass** lpRet=new TClass*[ulCount];
		dwRetSize=ulCount;

		LONG dwHashPosition=0;
		DWORD dwRetIndex=0;

		while(dwHashPosition<lHashSize)
		{
			if(HashArray[dwHashPosition].KEY!=NULLKEY)
				lpRet[dwRetIndex++]=HashArray[dwHashPosition].DATAPTR;


			dwHashPosition++;
		}
		
		return(lpRet);
	}


	LPDWORD FC GetAllKeys(DWORD& dwRetSize)
	{
		
		ULONG ulCount=lHashSize-lFreeCount;
		
		if(ulCount==0)
		{
			dwRetSize=0;
			return(NULL);
		}
		
		LPDWORD lpRet=new DWORD[ulCount];
		dwRetSize=ulCount;

		LONG dwHashPosition=0;
		DWORD dwRetIndex=0;

		while(dwHashPosition<lHashSize)
		{
			if(HashArray[dwHashPosition].KEY!=NULLKEY)
				lpRet[dwRetIndex++]=HashArray[dwHashPosition].KEY;

			dwHashPosition++;
		}
		
		return(lpRet);
	}



		
	DWORD FC ReSize(ULONG ulSize)
	{
		ULONG ulCurrSize=(lHashSize);
		DWORD dwHashPosition=0;

		if(ulSize<(ulCurrSize-lFreeCount))
			return(S_FAILED);	//data will be lost

		CHashTable Temp;
		Temp.AutoDelete(FALSE);
		

		Temp.Init(ulSize,lSizeIncrement,NULLKEY,NULLDATA);
		
		//rehash everything
		while(dwHashPosition<ulCurrSize)
		{
			if(HashArray[dwHashPosition].KEY!=NULLKEY)
				Temp.Insert(HashArray[dwHashPosition].KEY,HashArray[dwHashPosition].DATAPTR);
			
			dwHashPosition++;
		}
		
		if(HashArray)
			delete[] HashArray;

		lHashSize=Temp.lHashSize;
		HashArray=new CHashElement[Temp.lHashSize];
		memcpy((LPVOID) HashArray,(LPVOID) Temp.HashArray,lHashSize*sizeof(CHashElement));

		lFreeCount+=ulSize-ulCurrSize;

		return(S_SUCCESS);	
	}



	ULONG FC Filled()
	{
		return(lHashSize-lFreeCount);
	}


	DWORD FC FirstEmptyKey(DWORD& dwResult)
	{
		if(!lFreeCount)
		{
			if(bAutoResize)
				ReSize(lHashSize+lSizeIncrement);	//and continue
			else
				return(S_FAILED);
		}
			
		dwResult=0;
		
		while(HashArray[dwResult].KEY!=NULLKEY)
		{
			++dwResult%=lHashSize;	//increase the pos. and modulo size
		}

		return(S_SUCCESS);
		
	}


};

#endif // !defined _CHASHTABLE_H__
