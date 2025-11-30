#ifndef _CFASTPTRARRAYNORMAL_H__
#define _CFASTPTRARRAYNORMAL_H__


#include "CFastPrimitiveArrayNormal.h"

TEMPLATECLASS CFastPtrArrayNormal : public CFastPrimitiveArrayNormal<TEMPLATETYPE*>
{
public:
	
	CFastPtrArrayNormal();
	VOID AutoDelete(BOOL Auto=TRUE);
	BOOL Autodelete();

	BOOL DeletePtr(TEMPLATETYPE* ptr);
	WORD PtrIndex(TEMPLATETYPE* ptr);
	VOID DeletePtrAll();

	TEMPLATETYPE* FirstPtrElement();
	TEMPLATETYPE* LastPtrElement();
	
	TEMPLATETYPE* StartPtrForwardRun();
	TEMPLATETYPE* NextPtrForwardRun();
	TEMPLATETYPE* DeletePtrForwardRun();
	TEMPLATETYPE* StartPtrBackwardRun();
	TEMPLATETYPE* NextPtrBackwardRun();
	TEMPLATETYPE* DeletePtrBackwardRun();


	virtual VOID Destroy();
	virtual ~CFastPtrArrayNormal();


protected:
	BOOL bAutoDelete;

};


#endif