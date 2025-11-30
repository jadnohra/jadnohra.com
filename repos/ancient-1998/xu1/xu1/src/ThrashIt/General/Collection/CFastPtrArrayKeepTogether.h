#ifndef _CFASTPTRARRAYKEEPTOGETHER_H__
#define _CFASTPTRARRAYKEEPTOGETHER_H__


#include "CFastPrimitiveArrayKeepTogether.h"

TEMPLATECLASS CFastPtrArrayKeepTogether : public CFastPrimitiveArrayKeepTogether<TEMPLATETYPE*>
{
public:
	
	CFastPtrArrayKeepTogether();
	VOID AutoDelete(BOOL Auto=TRUE);
	BOOL Autodelete();

	BOOL DeletePtr(TEMPLATETYPE* ptr);
	WORD PtrIndex(TEMPLATETYPE* ptr);
	VOID DeletePtr(WORD Index);
	VOID DeletePtrAll();

	virtual VOID Destroy();
	virtual ~CFastPtrArrayKeepTogether();

	TEMPLATETYPE* FirstPtrElement();
	TEMPLATETYPE* LastPtrElement();
	
	TEMPLATETYPE* StartPtrForwardRun();
	TEMPLATETYPE* NextPtrForwardRun();
	TEMPLATETYPE* StartPtrBackwardRun();
	TEMPLATETYPE* NextPtrBackwardRun();

protected:
	BOOL bAutoDelete;

};


#endif