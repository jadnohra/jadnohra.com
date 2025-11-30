// TIStaticGeometryRealTime.h: interface for the TIStaticGeometryRealTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TISTATICGEOMETRYRUNTIME_H__5B2586BF_8328_4420_AF36_132379E335FB__INCLUDED_)
#define AFX_TISTATICGEOMETRYRUNTIME_H__5B2586BF_8328_4420_AF36_132379E335FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\..\General\File\ThrashFile.h"
#include "..\..\TIMaterialManager.h"
#include "TIStaticGeometryRealTimeStructs.h"

class TIStaticGeometryRealTime  
{
protected:
	TIStaticGeometryRealTimeNode	Root;

public:

	DWORD RenderID;

	TIStaticGeometryRealTime();
	virtual ~TIStaticGeometryRealTime();

	BOOL Test();
	VOID Render();
};

#endif // !defined(AFX_TISTATICGEOMETRYRUNTIME_H__5B2586BF_8328_4420_AF36_132379E335FB__INCLUDED_)
