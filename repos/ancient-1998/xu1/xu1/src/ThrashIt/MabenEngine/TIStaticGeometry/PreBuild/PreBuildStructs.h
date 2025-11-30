#ifndef _PREBUILDSTRUCTS_H__
#define _PREBUILDSTRUCTS_H__

#include "..\..\_TIINCLUDES.h"
#include "..\..\..\General\Collection\Tree.h"
#include "..\..\..\General\File\ThrashFile.h"


struct PreBuildVertex : public D3DVERTEX
{
	WORD MaterialIndex;	
	BYTE LOD;	
	
	PreBuildVertex();
	VOID Set(D3DVERTEX& ref,WORD& materialIndex);

};


struct PreBuildNodeRenderMaterialGroup
{
	D3DVERTEX*	pVertices;
	WORD		size;
	WORD		matIndex;

	PreBuildNodeRenderMaterialGroup();
	~PreBuildNodeRenderMaterialGroup();

};

struct PreBuildNodeRenderLODMatGroupInfo
{
	WORD MatIndex;
	LONG VertexCount;
};

struct PreBuildNodeRenderLOD
{
	PreBuildNodeRenderMaterialGroup* pMatGroup;
	WORD							 size;	
	BYTE							 LOD;	
	

	PreBuildNodeRenderLOD();
	~PreBuildNodeRenderLOD();

	VOID Build(CFastPrimitiveArrayKeepTogether<PreBuildVertex>*	pNodeVertices);
};


struct PreBuildNodeRenderData
{
	 PreBuildNodeRenderLOD* pLOD;
	 BYTE					size;

	PreBuildNodeRenderData();
	~PreBuildNodeRenderData();
	VOID Build(CFastPrimitiveArrayKeepTogether<PreBuildVertex>*	pNodeVertices);
	
};

class PreBuildNode : public TreeNode
{
public:
	BOOL IsRenderNode;
	PreBuildNodeRenderData* pRenderData;
	FLOAT Box[6];

	PreBuildNode();
	VOID SetBox(FLOAT min1,FLOAT max1,FLOAT min2,FLOAT max2,FLOAT min3,FLOAT max3);
	VOID FindOutBoxX();
	VOID FindOutBoxY();
	VOID FindOutBoxZ();
	~PreBuildNode();

	VOID WriteFile(ThrashFile& outputFile);
};

#endif
