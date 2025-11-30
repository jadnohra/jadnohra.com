#include "PreBuildStructs.h"
#include "..\..\..\FileFormats\TISFile.h"
#include "..\..\..\General\Collection\CFastPrimitiveArrayKeepTogether.h"
#include "..\..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"


PreBuildNodeRenderMaterialGroup::PreBuildNodeRenderMaterialGroup()
{
	pVertices = NULL;
	size = 0;
	matIndex = -1;
}

PreBuildNodeRenderMaterialGroup::~PreBuildNodeRenderMaterialGroup()
{
	if(pVertices)
	{
		delete[] pVertices;
		pVertices = NULL;
	}
	size = 0;
	matIndex = -1;
}


PreBuildNodeRenderLOD::PreBuildNodeRenderLOD()
{
	pMatGroup = NULL;
	size = 0;
}

PreBuildNodeRenderLOD::~PreBuildNodeRenderLOD()
{
	if(pMatGroup)
	{
		delete[] pMatGroup;
		pMatGroup = NULL;
	}
	size = 0;
}

VOID PreBuildNodeRenderLOD::Build(CFastPrimitiveArrayKeepTogether<PreBuildVertex>*	pNodeVertices)
{
	
	
	CFastPrimitiveArrayKeepTogether<PreBuildNodeRenderLODMatGroupInfo> MatInfo;

	MatInfo.Create(5);
	for(int i = 0; i < pNodeVertices->UsedCount; i+=3)
	{
		if(pNodeVertices->Array[i].LOD == LOD)
		{
			WORD matIndex = pNodeVertices->Array[i].MaterialIndex;
			for(int j = 0; j < MatInfo.UsedCount; j++)
			{
				if(MatInfo.Array[j].MatIndex == matIndex)
				{
					MatInfo.Array[j].VertexCount += 3;
					break;
				}
			}
			if(j == MatInfo.UsedCount)
			{
				PreBuildNodeRenderLODMatGroupInfo* pTemp = MatInfo.New();
				pTemp->MatIndex = matIndex;
				pTemp->VertexCount = 3;
			}
			
		}
	}

	if(MatInfo.UsedCount == 0)
	{
		return;
	}

	size = MatInfo.UsedCount;
	pMatGroup = new PreBuildNodeRenderMaterialGroup[size];

	for(i = 0; i < MatInfo.UsedCount; i++)
	{
		pMatGroup[i].matIndex = MatInfo.Array[i].MatIndex;
		pMatGroup[i].size = MatInfo.Array[i].VertexCount;
		pMatGroup[i].pVertices = new D3DVERTEX[pMatGroup[i].size];

		int tempCount = 0;
		for(int j = 0; j < pNodeVertices->UsedCount; j+=3)
		{
			if(pNodeVertices->Array[j].LOD == LOD)
			{
				WORD matIndex = pNodeVertices->Array[j].MaterialIndex;
				if(matIndex == pMatGroup[i].matIndex)
				{
					pMatGroup[i].pVertices[tempCount++] = (D3DVERTEX) pNodeVertices->Array[j];
					pMatGroup[i].pVertices[tempCount++] = (D3DVERTEX) pNodeVertices->Array[j + 1];
					pMatGroup[i].pVertices[tempCount++] = (D3DVERTEX) pNodeVertices->Array[j + 2];
				}
			}
		}
	}
	
}

PreBuildNodeRenderData::PreBuildNodeRenderData()
{
	pLOD = NULL;
}

PreBuildNodeRenderData::~PreBuildNodeRenderData()
{
	if(pLOD)
	{
		delete[] pLOD;
		pLOD = NULL;
	}
}

VOID PreBuildNodeRenderData::Build(CFastPrimitiveArrayKeepTogether<PreBuildVertex>*	pNodeVertices)
{
	size = 0;
	for(int i = 0; i < pNodeVertices->UsedCount; i+=3)
	{
		if(pNodeVertices->Array[i].LOD > size)
		{
			size = pNodeVertices->Array[i].LOD;
		}
	}


	size++;

	if(size == 0)
	{
		return;
	}

	pLOD = new PreBuildNodeRenderLOD[size];
	
	for(i = 0; i < size; i++)
	{
		pLOD[i].LOD = i;
		pLOD[i].Build(pNodeVertices);
	}

}

PreBuildNode::PreBuildNode()
{
	IsRenderNode = FALSE;
	pRenderData = NULL;	
}

VOID PreBuildNode::SetBox(FLOAT min1,FLOAT max1,FLOAT min2,FLOAT max2,FLOAT min3,FLOAT max3)
{
	Box[TISFILE_BASENODE_MINDIM1] = min1;
	Box[TISFILE_BASENODE_MAXDIM1] = max1;
	Box[TISFILE_BASENODE_MINDIM2] = min2;
	Box[TISFILE_BASENODE_MAXDIM2] = max2;
	Box[TISFILE_BASENODE_MINDIM3] = min3;
	Box[TISFILE_BASENODE_MAXDIM3] = max3;
}

PreBuildNode::~PreBuildNode()
{
	if(pRenderData)
	{
		delete pRenderData;
		pRenderData = NULL;
	}
}

VOID PreBuildNode::FindOutBoxX()
{
	Box[0] = FLT_MAX;
	Box[1] = -FLT_MAX;
	
	if(IsRenderNode)
	{
		if(pRenderData)
		{
			for(int i = 0; i < pRenderData->size; i++)
			{
				for(int j = 0; j < pRenderData->pLOD[i].size; j++)
				{
					for(int k = 0; k < pRenderData->pLOD[i].pMatGroup[j].size; k++)
					{
						if(pRenderData->pLOD[i].pMatGroup[j].pVertices[k].x < Box[0])
						{
							Box[0] = pRenderData->pLOD[i].pMatGroup[j].pVertices[k].x;
						}
						if(pRenderData->pLOD[i].pMatGroup[j].pVertices[k].x > Box[1])
						{
							Box[1] = pRenderData->pLOD[i].pMatGroup[j].pVertices[k].x;
						}

					}
				}
			}
		}
	}
	else
	{
		for(int i = 0; i < lpChildrenPointers->UsedCount; i++)
		{
			((PreBuildNode*) (lpChildrenPointers->Array[i]))->FindOutBoxX();
			if(((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[0] < Box[0])
			{
				Box[0] = ((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[0];
			}
			if(((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[1] > Box[1])
			{
				Box[1] = ((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[1];
			}
		}
	}
}

VOID PreBuildNode::FindOutBoxY()
{
	Box[2] = FLT_MAX;
	Box[3] = -FLT_MAX;
	
	if(IsRenderNode)
	{
		if(pRenderData)
		{
			for(int i = 0; i < pRenderData->size; i++)
			{
				for(int j = 0; j < pRenderData->pLOD[i].size; j++)
				{
					for(int k = 0; k < pRenderData->pLOD[i].pMatGroup[j].size; k++)
					{
						if(pRenderData->pLOD[i].pMatGroup[j].pVertices[k].y < Box[2])
						{
							Box[2] = pRenderData->pLOD[i].pMatGroup[j].pVertices[k].y;
						}
						if(pRenderData->pLOD[i].pMatGroup[j].pVertices[k].y > Box[3])
						{
							Box[3] = pRenderData->pLOD[i].pMatGroup[j].pVertices[k].y;
						}

					}
				}
			}
		}
	}
	else
	{
		for(int i = 0; i < lpChildrenPointers->UsedCount; i++)
		{
			((PreBuildNode*) (lpChildrenPointers->Array[i]))->FindOutBoxY();
			if(((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[2] < Box[2])
			{
				Box[2] = ((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[2];
			}
			if(((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[3] > Box[3])
			{
				Box[3] = ((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[3];
			}
		}
	}
}

VOID PreBuildNode::FindOutBoxZ()
{
	Box[4] = FLT_MAX;
	Box[5] = -FLT_MAX;
	
	if(IsRenderNode)
	{
		if(pRenderData)
		{
			for(int i = 4; i < pRenderData->size; i++)
			{
				for(int j = 4; j < pRenderData->pLOD[i].size; j++)
				{
					for(int k = 4; k < pRenderData->pLOD[i].pMatGroup[j].size; k++)
					{
						if(pRenderData->pLOD[i].pMatGroup[j].pVertices[k].z < Box[4])
						{
							Box[4] = pRenderData->pLOD[i].pMatGroup[j].pVertices[k].z;
						}
						if(pRenderData->pLOD[i].pMatGroup[j].pVertices[k].z > Box[5])
						{
							Box[5] = pRenderData->pLOD[i].pMatGroup[j].pVertices[k].z;
						}

					}
				}
			}
		}
	}
	else
	{
		for(int i = 4; i < lpChildrenPointers->UsedCount; i++)
		{
			((PreBuildNode*) (lpChildrenPointers->Array[i]))->FindOutBoxZ();
			if(((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[4] < Box[4])
			{
				Box[4] = ((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[4];
			}
			if(((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[5] > Box[5])
			{
				Box[5] = ((PreBuildNode*) (lpChildrenPointers->Array[i]))->Box[5];
			}
		}
	}
}

VOID PreBuildNode::WriteFile(ThrashFile& outputFile)
{

}


PreBuildVertex::PreBuildVertex()
{
	MaterialIndex = -1;
	LOD = 0;
}

VOID PreBuildVertex::Set(D3DVERTEX& ref,WORD& materialIndex)
{
	memcpy((D3DVERTEX*) this,&ref,sizeof(D3DVERTEX));
	MaterialIndex = materialIndex;
}




