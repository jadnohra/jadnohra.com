// TIStaticGeometryPreBuildModuleBinaryAreaSize.h: interface for the TIStaticGeometryPreBuildModuleBinaryAreaSize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITISTATICGEOMETRYPREBUILDMODULEBINARYAREASIZE_H__763204D1_9287_4080_BE7A_051EC111DE15__INCLUDED_)
#define AFX_ITISTATICGEOMETRYPREBUILDMODULEBINARYAREASIZE_H__763204D1_9287_4080_BE7A_051EC111DE15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ITIStaticGeometryPreBuildModule.h"
#include "..\..\..\General\Math\TIMath.h"

class TIStaticGeometryPreBuildModuleBinaryAreaSize : public ITIStaticGeometryPreBuildModule
{

protected:
		
	FLOAT ThresholdSizeDim1;
	FLOAT ThresholdSizeDim2;
	int Mode;
	BOOL ClipPolys;
	LONG ProcessedNodeCount;
	LONG RenderNodeCount;
	DWORD LastPrintTime;
	LONG PrintInterval;
	LONG OutVertexCount;
	

	VOID ProcessNode(PreBuildNode* pNode);
	virtual BOOL _Build(ThrashFile& outputFile);
	VOID PrintSetup();
	BOOL PointInside(D3DVERTEX* pV,FLOAT& dim1Min,FLOAT& dim1Max,FLOAT& dim2Min,FLOAT& dim2Max);
	int TriIntersectsSegmentPlanes(D3DVERTEX* pV1,D3DVERTEX* pV2,D3DVERTEX* pV3,FLOAT& dim1Min,FLOAT& dim1Max,FLOAT& dim2Min,FLOAT& dim2Max,SegmentPlane segmentPlanes[4],D3DVERTEX Intersections[13],int& IntersectionCount,D3DVERTEX* pSortedIntersections[13]);	//return inter. count
	int ProjectToTri(D3DVERTEX* pV[3],FLOAT& dim1Min,FLOAT& dim1Max,FLOAT& dim2Min,FLOAT& dim2Max,D3DVERTEX intersections[10],int& intersectionCount); //returns inter count
	VOID RemoveSimilarIntersections(D3DVERTEX intersections[13],int& intersectionCount);
	BOOL WriteFile(ThrashFile& outputFile,PreBuildNode* pRoot);
public:
	
	const static int MODE_XZ;
	const static int MODE_XY;
	const static int MODE_YZ;

	TIStaticGeometryPreBuildModuleBinaryAreaSize();
	virtual ~TIStaticGeometryPreBuildModuleBinaryAreaSize();

	BOOL CheckSetup();
	BOOL Setup(FLOAT thresholdSizeDim1,FLOAT thresholdSizeDim2,int mode,BOOL PolyClip);
	
};

#endif // !defined(AFX_ITISTATICGEOMETRYPREBUILDMODULEBINARYAREASIZE_H__763204D1_9287_4080_BE7A_051EC111DE15__INCLUDED_)
