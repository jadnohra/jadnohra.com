// ITIStaticGeometryPreBuildModule.h: interface for the ITIStaticGeometryPreBuildModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITISTATICGEOMETRYPREBUILDMODULE_H__30D2285E_09F2_42CC_AC64_F028E4F6660D__INCLUDED_)
#define AFX_ITISTATICGEOMETRYPREBUILDMODULE_H__30D2285E_09F2_42CC_AC64_F028E4F6660D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\..\General\Collection\CFastPrimitiveArrayKeepTogether.h"
#include "..\..\..\General\File\ThrashFile.h"
#include "..\..\..\FileFormats\ASEFile.h"
#include "..\..\..\General\Collection\Tree.h"
#include "..\..\ITIGeometryBuilder3D.h"
#include "stdio.h"
#include "PreBuildStructs.h"

#define LODNULL 0xFF




class ITIStaticGeometryPreBuildModule : public ITIGeometryBuilder3D
{
protected:
	
	DWORD												VerboseLevel;
	CFastPrimitiveArrayKeepTogether<PreBuildVertex>		TempVertices;
	CFastPrimitiveArrayKeepTogether<ASEMATERIAL>		TempMaterials;

	LONG												TotalFileCount;
	LONG												SuccessFileCount;


	WORD RegisterMaterial(ASEMATERIAL* pMaterial);
	VOID PrintCurrentData();
	VOID Print(String& str,DWORD level);

	virtual BOOL _Build(ThrashFile& outputFile);
public:	
	/*
		to be overriden
	*/
	virtual ~ITIStaticGeometryPreBuildModule();
	
		
	
	
	ITIStaticGeometryPreBuildModule(DWORD verboseLevel = 1);
	VOID SetVerboseLevel(DWORD verboseLevel);
	virtual BOOL OnNewMesh(D3DVERTEX* pVertices,long faceCount,ASEMATERIAL* pMaterial);
	
	BOOL AddFile(LPCHAR path);
	BOOL Build(LPCHAR path);
	
	VOID ResetFileCount();

};

#endif // !defined(AFX_ITISTATICGEOMETRYPREBUILDMODULE_H__30D2285E_09F2_42CC_AC64_F028E4F6660D__INCLUDED_)
