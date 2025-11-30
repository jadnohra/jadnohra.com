// CArray.h: interface for the CArray class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CARRAY_H__
#define _CARRAY_H__

#include "..\_INCLUDES.h"
#include "memory.h"

#define NULLINDEX 0xffff
#define TCLASS template<class TClass>


enum ELEMENT_STATE{VALID,INVALID};

class CClassArrayElement 
{
public:	
	WORD	_prevIndex;
	WORD	_nextIndex;
	ELEMENT_STATE _state;

	virtual VOID Reset()=0; //override this and call destruction then construction code
	/*{
		1.release ptrs and resources
		2.init like a constructor
	  };*/
};

/*
	TClass should inherit from CClassArrayElement
*/
TCLASS class CClassArray
{
protected:
	TClass* Array;
	WORD FirstValid;
	WORD FirstInvalid;
	WORD LastInvalid;

	WORD	AllocatedElementCount;
	WORD	ValidElementCount;
	WORD	RunIndex;
	
public:
	CClassArray()
	{
		Array			=NULL;
		FirstValid		=NULLINDEX;
		FirstInvalid	=NULLINDEX;
		LastInvalid		=NULLINDEX;

		AllocatedElementCount	=0;
		ValidElementCount		=0;
	};

	
	BOOL Allocate(WORD ElementCount)
	{
		TClass fill;
		return(Allocate(ElementCount,&fill));
	}

	
	BOOL Allocate(WORD ElementCount,TClass* lpFill)
	{
		if(Array)
			Free();

		Array=new TClass[ElementCount];

		if(!Array)
			return(FALSE);

		AllocatedElementCount	=ElementCount;
		ValidElementCount		=0;

		lpFill->_state=INVALID;
		//fill the array and setup the links
		while(ElementCount)
		{
			lpFill->_prevIndex=ElementCount-2;
			lpFill->_nextIndex=ElementCount;
			memcpy((LPVOID) &Array[--ElementCount],lpFill,sizeof(TClass));
		}
		//fix the 1st and last elements
		Array[AllocatedElementCount-1]._nextIndex=NULLINDEX;
		Array[0]._prevIndex=NULLINDEX;


		FirstInvalid=0;
		LastInvalid=AllocatedElementCount-1;
		return(TRUE);
	}

	//TClass* New(WORD ElementCount);
	TClass* NewAutoSize()
	{
		if(FirstInvalid==NULLINDEX)
		{
			if(!Reallocate(ValidElementCount+ValidElementCount))
				return(NULL);
		}

		return(New());
	}

	
	/*
	TClass* NewAutoSize(TClass* lpCopy)
	{
		if(FirstInvalid==NULLINDEX)
		{
			if(!Reallocate(ValidElementCount+ValidElementCount))
				return(NULL);
		}

		return(New(lpCopy));
	}

	
	TClass* New(TClass* lpCopy)
	{
		TClass* New=this->New();
		memcpy(New,lpCopy,sizeof(TClass));
		we cant do this : we will overwrite the _prevIndex and stuff!!!!
		return(New);
	}
	*/

	TClass* New()
	{
		WORD NewIndex;
		if(FirstInvalid==NULLINDEX)
			return(NULL);
		
		ValidElementCount++;
		NewIndex=FirstInvalid;
		Array[NewIndex]._state=VALID;

		FirstInvalid=Array[FirstInvalid]._nextIndex;
		if(FirstInvalid!=NULLINDEX)
		{
			Array[FirstInvalid]._prevIndex=NULLINDEX;
		}
		else
		{
			LastInvalid=NULLINDEX;
		}
		
		if(NewIndex>FirstValid)
		{
			WORD Search=NewIndex-1;
			//make a Search backward to find a VALID element
			while(Array[Search--]._state==INVALID);
			Search++;
			Array[NewIndex]._nextIndex=Array[Search]._nextIndex;
			Array[NewIndex]._prevIndex=Search;
			Array[Search]._nextIndex=NewIndex;
			if(Array[NewIndex]._nextIndex!=NULLINDEX)
				Array[Array[NewIndex]._nextIndex]._prevIndex=NewIndex;
		}
		else
		{
			Array[NewIndex]._prevIndex=NULLINDEX;
			Array[NewIndex]._nextIndex=FirstValid;
			
			if(FirstValid!=NULLINDEX)
				Array[FirstValid]._prevIndex=NewIndex;

			FirstValid=NewIndex;
		}

		return(&Array[NewIndex]);
	}

	VOID FC Delete(TClass* lpDestroy)
	{
		WORD DelIndex=(lpDestroy-Array);
		_Delete(DelIndex);
	}

	VOID FC FastDelete(TClass* lpDestroy)
	{
		WORD DelIndex=(lpDestroy-Array);
		_FastDelete(DelIndex);
	}



	VOID FC Delete(WORD DelIndex)
	{
		_Delete(DelIndex);		
	}

	VOID FC FastDelete(WORD DelIndex)
	{
		_FastDelete(DelIndex);
	}


	TClass* First() 
	{
		if(FirstValid!=NULLINDEX)
			return(&Array[FirstValid]);
		return(NULL);
	}

	
	TClass* FC Next(TClass* lpElement)
	{
		WORD RetIndex=(lpElement-Array);

		if(Array[RetIndex]._nextIndex!=NULLINDEX)
			return(&Array[Array[RetIndex]._nextIndex]);
		
		return(NULL);
	}

	TClass* FC Previous(TClass* lpElement)
	{
		WORD RetIndex=(lpElement-Array);

		if(Array[RetIndex]._prevIndex!=NULLINDEX)
			return(&Array[Array[RetIndex]._prevIndex]);
		
		return(NULL);
	}

	
	
	/*these fcts are slightly faster than the Next(ptr)
		fctions but they should be used with care
		=> if initRun returns null => dont continue
		=> when Next() returns Null you have to call
		initRun b4 the next call to next()
	*/
	TClass* StartRun()
	{
		if(FirstValid==NULLINDEX)
			return(NULL);
		
		RunIndex=FirstValid;
		return(&Array[RunIndex]);
	}
	TClass* Next()
	{
		RunIndex=Array[RunIndex]._nextIndex;

		if(RunIndex!=NULLINDEX)
		{
			return(&Array[RunIndex]);
		}

		return(NULL);
	}



	TClass* FC Element(WORD wIndex)
	{
		if(Array[wIndex]._state==INVALID)
			return(NULL);

		return(&Array[wIndex]);
	}

	TClass* FC FastElement(WORD wIndex)//but unsafe
	{
		return(&Array[wIndex]);
	}

	
	WORD ElementCount()
	{
		return(ValidElementCount);
	}
	
	WORD AllocatedElements()
	{
		return(AllocatedElementCount);
	}
	
	
	BOOL Reallocate(WORD newElementCount)
	{
		TClass fill;
		return(Reallocate(newElementCount,&fill));
	}

	
	BOOL Reallocate(WORD newElementCount,TClass* lpFill)
	{
		if(newElementCount<=AllocatedElementCount)
			return(FALSE);

		
		TClass* New=new TClass[newElementCount];

		if(New==NULL)
			return(FALSE);

		memcpy(New,Array,sizeof(TClass)*AllocatedElementCount);

		delete[] Array;
		Array=New;

		lpFill->_state=INVALID;
		
		//fill the array and setup the links
		for(int i=AllocatedElementCount;i<newElementCount;i++)
		{
			lpFill->_prevIndex=i-1;
			lpFill->_nextIndex=i+1;
			memcpy((LPVOID) &Array[i],lpFill,sizeof(TClass));
		}
		
		if(FirstInvalid==NULLINDEX)
			FirstInvalid=AllocatedElementCount;
		

		Array[AllocatedElementCount]._prevIndex=LastInvalid;
		Array[newElementCount-1]._nextIndex=NULLINDEX;

		if(LastInvalid!=NULLINDEX)
			Array[LastInvalid]._nextIndex=AllocatedElementCount;

		LastInvalid=newElementCount;

		AllocatedElementCount=newElementCount;
		
		return(TRUE);
	}
	
	VOID Free()
	{
		WORD ResetRun=FirstValid;

		while(ResetRun!=NULLINDEX)
		{
			Array[ResetRun].Reset();
			ResetRun=Array[ResetRun]._nextIndex;
		}

		delete[] Array;
		
		Array			=NULL;
		FirstValid		=NULLINDEX;
		FirstInvalid	=NULLINDEX;
		LastInvalid		=NULLINDEX;

		AllocatedElementCount	=0;
		ValidElementCount		=0;
	}

	~CClassArray()
	{
		Free();
	}


private:
	inline VOID _Delete(WORD& DelIndex)
	{
		//already dead
		if(DelIndex>=AllocatedElementCount)
			return;
		if(Array[DelIndex]._state==INVALID)
			return;
	
		_FastDelete(DelIndex);
	}

	inline VOID _FastDelete(WORD& DelIndex)
	{
		
		ValidElementCount--;
		//call the destructor
		Array[DelIndex].Reset();
		Array[DelIndex]._state=INVALID;

		//rem. from valid list
		if(DelIndex==FirstValid)
			FirstValid=Array[DelIndex]._nextIndex;

		if(Array[DelIndex]._prevIndex!=NULLINDEX)
			Array[Array[DelIndex]._prevIndex]._nextIndex=Array[DelIndex]._nextIndex;
		if(Array[DelIndex]._nextIndex!=NULLINDEX)
			Array[Array[DelIndex]._nextIndex]._prevIndex=Array[DelIndex]._prevIndex;
	
		//add to invalid list
		
		//we had no deleted elements
		if(FirstInvalid==NULLINDEX)
		{
			FirstInvalid=LastInvalid=DelIndex;
			Array[DelIndex]._prevIndex=NULLINDEX;
			Array[DelIndex]._nextIndex=NULLINDEX;
			return; //thats it return
		}

		//we have to add it to the list

		//make it first valid
		if(DelIndex<FirstInvalid)
		{
			Array[DelIndex]._nextIndex=FirstInvalid;
			Array[DelIndex]._prevIndex=NULLINDEX;
			FirstInvalid=DelIndex;
		}
		else//make it last invalid
		{
			Array[DelIndex]._prevIndex=LastInvalid;
			Array[DelIndex]._nextIndex=NULLINDEX;

			Array[LastInvalid]._nextIndex=DelIndex;
			LastInvalid=DelIndex;
		}

				
	}


};



/*
	Array for primtive _data types and structs (e.g : DWORD , LPVOID , ...)
	no destructors are called , no inheritance needed
*/
TCLASS class CPrimitiveArray
{

//the class for the array elements containing the primitve
class CPrimitiveArrayElement
{
public:	//WARNING if this struct changes u have to modify the Del Fctions
	WORD	_prevIndex;
	WORD	_nextIndex;
	ELEMENT_STATE _state;
	TClass	_data;
};

protected:
	CPrimitiveArrayElement* Array;
	WORD FirstValid;
	WORD FirstInvalid;
	WORD LastInvalid;

	WORD	RunIndex;
	
public:
	WORD	AllocatedElementCount;
	WORD	ValidElementCount;
	

	CPrimitiveArray()
	{
		Array			=NULL;
		FirstValid		=NULLINDEX;
		FirstInvalid	=NULLINDEX;
		LastInvalid		=NULLINDEX;

		AllocatedElementCount	=0;
		ValidElementCount		=0;
	};

	BOOL Allocate(WORD ElementCount)
	{
		TClass fill;
		memset(&fill,0,sizeof(TClass));
		return(Allocate(ElementCount,&fill));
	}


	BOOL Allocate(WORD ElementCount,TClass* fill)
	{
		if(Array)
			Free();

		Array=new CPrimitiveArrayElement[ElementCount];

		if(!Array)
			return(FALSE);

		AllocatedElementCount	=ElementCount;
		ValidElementCount		=0;

		CPrimitiveArrayElement _fill;
		memcpy(&_fill,fill,sizeof(TClass));
		_fill._state=INVALID;
		//fill the array and setup the links
		while(ElementCount)
		{
			_fill._prevIndex=ElementCount-2;
			_fill._nextIndex=ElementCount;
			memcpy((LPVOID) &Array[--ElementCount],&_fill,sizeof(CPrimitiveArrayElement));
		}
		//fix the 1st and last elements
		Array[AllocatedElementCount-1]._nextIndex=NULLINDEX;
		Array[0]._prevIndex=NULLINDEX;


		FirstInvalid=0;
		LastInvalid=AllocatedElementCount-1;
		return(TRUE);
	}

	//TClass* New(WORD ElementCount);
	TClass* NewAutoSize(TClass* data)
	{
		if(FirstInvalid==NULLINDEX)
		{
			if(!Reallocate(ValidElementCount+ValidElementCount))
				return(NULL);

		}

		return(New(data));
		
	}



	TClass* New(TClass* data)
	{
		WORD NewIndex;
		if(FirstInvalid==NULLINDEX)
			return(NULL);
		
		ValidElementCount++;
		NewIndex=FirstInvalid;
		Array[NewIndex]._state=VALID;
		memcpy(&(Array[NewIndex]._data),data,sizeof(TClass));


		FirstInvalid=Array[FirstInvalid]._nextIndex;
		if(FirstInvalid!=NULLINDEX)
		{
			Array[FirstInvalid]._prevIndex=NULLINDEX;
		}
		else
		{
			LastInvalid=NULLINDEX;
		}
		
		if(NewIndex>FirstValid)
		{
			WORD Search=NewIndex-1;
			//make a Search backward to find a VALID element
			while(Array[Search--]._state==INVALID);
			Search++;
			Array[NewIndex]._nextIndex=Array[Search]._nextIndex;
			Array[NewIndex]._prevIndex=Search;
			Array[Search]._nextIndex=NewIndex;
			if(Array[NewIndex]._nextIndex!=NULLINDEX)
				Array[Array[NewIndex]._nextIndex]._prevIndex=NewIndex;
		}
		else
		{
			Array[NewIndex]._prevIndex=NULLINDEX;
			Array[NewIndex]._nextIndex=FirstValid;
			
			if(FirstValid!=NULLINDEX)
				Array[FirstValid]._prevIndex=NewIndex;

			FirstValid=NewIndex;
		}

		return(&(Array[NewIndex]._data));
	}
	
	VOID FC Delete(TClass* lpDestroy)
	{
		WORD DelIndex=(WORD) (((CPrimitiveArrayElement*) (((DWORD) lpDestroy)-(sizeof(CPrimitiveArrayElement)-sizeof(TClass))))-Array);
		_Delete(DelIndex);		
	}

	VOID FC FastDelete(TClass* lpDestroy)
	{
		WORD DelIndex=(WORD) (((CPrimitiveArrayElement*) (((DWORD) lpDestroy)-(sizeof(CPrimitiveArrayElement)-sizeof(TClass))))-Array);
		_FastDelete(DelIndex);
	}


	VOID FC Delete(WORD DelIndex)
	{
		_Delete(DelIndex);		
	}

	VOID FC FastDelete(WORD DelIndex)
	{
		_FastDelete(DelIndex);
	}

	TClass* First() 
	{
		if(FirstValid!=NULLINDEX)
			return(&(Array[FirstValid]._data));
		return(NULL);
	}

	
	TClass* FC Next(TClass* lpElement)
	{
		WORD RetIndex=(WORD) (((CPrimitiveArrayElement*) (((DWORD) lpElement)-(sizeof(CPrimitiveArrayElement)-sizeof(TClass))))-Array);

		if(Array[RetIndex]._nextIndex!=NULLINDEX)
			return(&(Array[Array[RetIndex]._nextIndex]._data));
		
		return(NULL);
	}

	TClass* FC Previous(TClass* lpElement)
	{
		WORD RetIndex=(WORD) (((CPrimitiveArrayElement*) (((DWORD) lpElement)-(sizeof(CPrimitiveArrayElement)-sizeof(TClass))))-Array);

		if(Array[RetIndex]._prevIndex!=NULLINDEX)
			return(&(Array[Array[RetIndex]._prevIndex]._data));
		
		return(NULL);
	}
	
	//these fcts are slightly faster than the Next(ptr)
	//	fctions but they should be used with care
	///	=> if initRun returns null => dont continue
	//	=> when Next() returns Null you have to call
	//	initRun b4 the next call to next()
	
	TClass* StartRun()
	{
		if(FirstValid==NULLINDEX)
			return(NULL);
		
		RunIndex=FirstValid;
		return(&(Array[RunIndex]._data));
	}
	TClass* Next()
	{
		RunIndex=Array[RunIndex]._nextIndex;

		if(RunIndex!=NULLINDEX)
		{
			return(&(Array[RunIndex]._data));
		}

		return(NULL);
	}

	/*
	TClass* StartInverseRun()
	{
		if(FirstValid==NULLINDEX)
			return(NULL);
		
		RunIndex=FirstValid;
		return(&(Array[RunIndex]._data));
	}
	TClass* Previous()
	{
		RunIndex=Array[RunIndex]._nextIndex;

		if(RunIndex!=NULLINDEX)
		{
			return(&(Array[RunIndex]._data));
		}

		return(NULL);
	}
	*/



	TClass* FC Element(WORD wIndex)
	{
		if(Array[wIndex]._state==INVALID)
			return(NULL);

		return(&(Array[wIndex]._data));
	}

	TClass* FC FastElement(WORD wIndex)//but unsafe
	{
		return(&(Array[wIndex]._data));
	}

	BOOL Reallocate(WORD newElementCount)
	{
		TClass fill;
		memset(&fill,0,sizeof(TClass));
		return(Reallocate(newElementCount,&fill));
	}

	
	BOOL Reallocate(WORD newElementCount,TClass* fill)
	{
		if(newElementCount<=AllocatedElementCount)
			return(FALSE);

		
		CPrimitiveArrayElement* New=new CPrimitiveArrayElement[newElementCount];

		if(New==NULL)
			return(FALSE);

		memcpy(New,Array,sizeof(CPrimitiveArrayElement)*AllocatedElementCount);
		

		delete[] Array;
		Array=New;

		CPrimitiveArrayElement _fill;
		_fill._state=INVALID;
		memcpy(&(_fill._data),fill,sizeof(TClass));
		
		//fill the array and setup the links
		for(int i=AllocatedElementCount;i<newElementCount;i++)
		{
			_fill._prevIndex=i-1;
			_fill._nextIndex=i+1;
			
			
			memcpy((LPVOID) &(Array[i]),&_fill,sizeof(CPrimitiveArrayElement));
		}
		
		if(FirstInvalid==NULLINDEX)
			FirstInvalid=AllocatedElementCount;
		

		Array[AllocatedElementCount]._prevIndex=LastInvalid;
		Array[newElementCount-1]._nextIndex=NULLINDEX;

		if(LastInvalid!=NULLINDEX)
			Array[LastInvalid]._nextIndex=AllocatedElementCount;

		LastInvalid=newElementCount-1;

		AllocatedElementCount=newElementCount;


		return(TRUE);
	}


	WORD ElementCount()
	{
		return(ValidElementCount);
	}
	
	WORD AllocatedElements()
	{
		return(AllocatedElementCount);
	}
	
	
	
	VOID Free()
	{
		if(Array)
		{
			delete[] Array;
			
			Array			=NULL;
			FirstValid		=NULLINDEX;
			FirstInvalid	=NULLINDEX;
			LastInvalid		=NULLINDEX;

			AllocatedElementCount	=0;
			ValidElementCount		=0;
		}
	}

	~CPrimitiveArray()
	{
		Free();
	}


private:
	inline VOID _Delete(WORD& DelIndex)
	{
		//already dead
		if(DelIndex>=AllocatedElementCount)
			return;
		if(Array[DelIndex]._state==INVALID)
			return;
	
		_FastDelete(DelIndex);
	}

	inline VOID _FastDelete(WORD& DelIndex)
	{
		
		ValidElementCount--;
		//call the destructor
		Array[DelIndex]._state=INVALID;

		//rem. from valid list
		if(DelIndex==FirstValid)
			FirstValid=Array[DelIndex]._nextIndex;

		if(Array[DelIndex]._prevIndex!=NULLINDEX)
			Array[Array[DelIndex]._prevIndex]._nextIndex=Array[DelIndex]._nextIndex;
		if(Array[DelIndex]._nextIndex!=NULLINDEX)
			Array[Array[DelIndex]._nextIndex]._prevIndex=Array[DelIndex]._prevIndex;
	
		//add to invalid list
		
		//we had no deleted elements
		if(FirstInvalid==NULLINDEX)
		{
			FirstInvalid=LastInvalid=DelIndex;
			Array[DelIndex]._prevIndex=NULLINDEX;
			Array[DelIndex]._nextIndex=NULLINDEX;
			return; //thats it return
		}

		//we have to add it to the list

		//make it first valid
		if(DelIndex<FirstInvalid)
		{
			Array[DelIndex]._nextIndex=FirstInvalid;
			Array[DelIndex]._prevIndex=NULLINDEX;
			FirstInvalid=DelIndex;
		}
		else//make it last invalid
		{
			Array[DelIndex]._prevIndex=LastInvalid;
			Array[DelIndex]._nextIndex=NULLINDEX;

			Array[LastInvalid]._nextIndex=DelIndex;
			LastInvalid=DelIndex;
		}

				
	}

	
};



TCLASS class CQueue
{
protected:
	TClass* Array;
	

	WORD	AllocatedElementCount;

 
   public:
	LONG	LastIndex;

	CQueue()
	{
		Array			=NULL;
		LastIndex		=-1;

		AllocatedElementCount=0;
	};

	
	
	BOOL Allocate(WORD ElementCount)
	{
		Array=new TClass[ElementCount];

		if(!Array)
			return(FALSE);

		AllocatedElementCount=ElementCount;
		
		return(TRUE);
	}

	
	TClass* NewAutoSize()
	{
		if(LastIndex==AllocatedElementCount-1)
		{
			if(!Reallocate(AllocatedElementCount+AllocatedElementCount))
				return(NULL);
		}

		
		LastIndex++;
		return(&(Array[LastIndex]));
	}



	TClass* New()
	{
		if(LastIndex==AllocatedElementCount-1)
			return(NULL);
		
		LastIndex++;
		return(&(Array[LastIndex]));
	}
	
	
	TClass* FC Delete()
	{
		if(LastIndex>=0)
		{	
			LastIndex--;
			return(&(Array[LastIndex+1]));
		}
		else
			return(NULL);
	}

	TClass* FC Peak()
	{
		return(&(Array[LastIndex]));
	}

  	
	BOOL Reallocate(WORD newElementCount)
	{
		if(newElementCount<=AllocatedElementCount)
			return(FALSE);

		
		TClass* New=new TClass[newElementCount];

		if(New==NULL)
			return(FALSE);

		memcpy(New,Array,sizeof(TClass)*AllocatedElementCount);
		

		delete[] Array;
		Array=New;

		AllocatedElementCount=newElementCount;
		return(TRUE);
	}

	VOID Free()
	{
		delete[] Array;
		
		Array			=NULL;
		LastIndex		=-1;
		AllocatedElementCount	=0;
	}

	~CQueue()
	{
		Free();
	}
};


TCLASS class PtrList
{
public:
	TClass** Array;
	LONG	LastIndex;

	WORD	AllocatedElementCount;
	BOOL	bAutoDelete;

 
   public:
	PtrList()
	{
		Array			=NULL;
		LastIndex		=-1;
		
		bAutoDelete=FALSE;
		AllocatedElementCount=0;
	};

	VOID AutoDelete(BOOL Auto)
	{
		bAutoDelete=Auto;
	}

	BOOL AutoDelete()
	{
		return(bAutoDelete);
	}

	BOOL Allocate(WORD ElementCount)
	{
		Array=new TClass*[ElementCount];

		if(!Array)
			return(FALSE);

		AllocatedElementCount=ElementCount;
		
		return(TRUE);
	}

	
	BOOL InsertAutoSize(TClass* ptr)
	{
		if(LastIndex==AllocatedElementCount-1)
		{
			if(!Reallocate(AllocatedElementCount+AllocatedElementCount))
				return(FALSE);
		}

		
		LastIndex++;
		Array[LastIndex]=ptr;
		return(TRUE);
	}



	BOOL Insert(TClass* ptr)
	{
		if(LastIndex==AllocatedElementCount-1)
			return(FALSE);
		
		LastIndex++;
		Array[LastIndex]=ptr;
	}
	
	
	
  	
	BOOL Reallocate(WORD newElementCount)
	{
		if(newElementCount<=AllocatedElementCount)
			return(FALSE);

		
		TClass** New=new TClass*[newElementCount];

		if(New==NULL)
			return(FALSE);

		memcpy(New,Array,sizeof(TClass*)*AllocatedElementCount);
		

		delete[] Array;
		Array=New;

		AllocatedElementCount=newElementCount;
		return(TRUE);
	}

	VOID Free()
	{
		if(bAutoDelete)
		{
			for(int i=0;i<=LastIndex;i++)
				delete Array[i];
		}
		
		delete[] Array;
		
		Array			=NULL;
		LastIndex		=-1;
		AllocatedElementCount	=0;
	}

	~PtrList()
	{
		Free();
	}

};


#endif 
