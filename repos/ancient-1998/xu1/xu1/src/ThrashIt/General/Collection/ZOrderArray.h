#ifndef _ZORDERARRAYH__
#define _ZORDERARRAYH__

#include "CFastPtrArrayKeepTogether.h"

struct ZOrderElement
{
	int wcZOrder;
	ZOrderElement(){wcZOrder=0x7FFFFFFF;};
};


enum ZOrderActionType
{
	_Delete,
	_Change,
	_Add,
};


TEMPLATECLASS ZOrderArray
{

	
	struct ZOrderAction
	{
		ZOrderActionType	action;
		int					index;
		int					wcNewZOrder;
		TEMPLATETYPE* 		lpObject;
	};


public:
	
	CFastPtrArrayKeepTogether<TEMPLATETYPE> ZArray;
	CFastPrimitiveArrayKeepTogether<ZOrderAction> ZActions;

	BOOL Create(WORD initialElementCount);
	VOID Destroy();

	
	VOID Delete(TEMPLATETYPE* lpElement);
	VOID Change(TEMPLATETYPE* lpElement,int NewZORder);
	VOID Delete(WORD index);
	VOID Change(WORD index,int NewZORder);
	VOID Add(TEMPLATETYPE* lpElement);
	
	
	VOID Lock();
	VOID UnLock();


	VOID ExecuteActions();

	BOOL Locked;

protected:
	inline int SearchForPlace(int currZ,int newZ,int currIndex);
	inline int SearchForNewPlace(int newZ,int currIndex);
	inline VOID DeleteNow(int index);
	inline VOID ChangeNow(int index,int NewZORder);
	inline VOID AddNow(TEMPLATETYPE* lpElement);
};

#endif