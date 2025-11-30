// TIStaticGeometryRealTime.cpp: implementation of the TIStaticGeometryRealTime class.
//
//////////////////////////////////////////////////////////////////////

#include "TIStaticGeometryRealTime.h"
#include "..\..\..\General\File\ThrashFile.h"
#include "..\..\..\FileFormats\TISFile.h"
#include "..\..\..\General\Helper\AddressResolver.h"
#include "..\..\..\..\Maben\DX\d3dutil.h"
#include "..\..\..\..\Maben\DX\d3dmath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


TIStaticGeometryRealTime::TIStaticGeometryRealTime()
{
	
}

TIStaticGeometryRealTime::~TIStaticGeometryRealTime()
{

}


VOID TIStaticGeometryRealTime::Render()
{
	
	TIInlineSetMaterial(0);
	
 	Root.PrepareRenderCycle();
	Root.RenderID = RenderID;
	Root.Render();
	
}

BOOL TIStaticGeometryRealTime::Test()
{
	
	if(TIMaterialManager::pCurrTIMaterialManager == NULL)
	{
		return(FALSE);
	}
	
	ThrashFile file;
	file.SetPath("map.tis");
	if(!file.OpenReadOnly())
	{
		return(FALSE);
	}

	TISFILE_HEADER header;
	TISFILE_SCENEMANAGMENT scene;
	TISFILE_STRUCTADDRESS treeAddress;
	TISFILE_STRUCTADDRESS matAddress;
	TISFILE_TREE tree;
	
	file.StartRead(&header,0,sizeof(TISFILE_HEADER));
	if(header.IsCorrupt())
	{
		return(FALSE);
	}
	if(!header.IsCurrentVersion())
	{
		return(FALSE);
	}
	file.ReadNext(&scene,sizeof(TISFILE_SCENEMANAGMENT));
	if(scene.IsCorrupt())
	{
		return(FALSE);
	}

	file.ReadNext(&treeAddress,sizeof(TISFILE_STRUCTADDRESS));
	file.ReadNext(&matAddress,sizeof(TISFILE_STRUCTADDRESS));
	
	
	TISFILE_MATERIALARRAY Materials;
	WORD *MaterialMap = NULL;
	file.StartRead(&(Materials.Size),matAddress.Address,sizeof(Materials.Size));

	if(Materials.Size)
	{
		MaterialMap = new WORD[Materials.Size]; 
	}

	for(int i = 0; i < Materials.Size; i++)
	{
		TISFILE_MATERIAL Mat;
		file.ReadNext(&Mat.Material,sizeof(D3DMATERIAL7));
		file.ReadNext(&Mat.TextureFile.Size,sizeof(Mat.TextureFile.Size));
		if(Mat.TextureFile.Size)
		{
			Mat.TextureFile.Chars = new CHAR[Mat.TextureFile.Size];
			file.ReadNext(Mat.TextureFile.Chars,Mat.TextureFile.Size * sizeof(CHAR));
		}
		
		
		MaterialMap[i] = TIMaterialManager::pCurrTIMaterialManager->Add(Mat.Material,Mat.TextureFile.Chars);
	}

	
	
	
	file.StartRead(&tree.BuildAlgorithm,treeAddress.Address,sizeof(tree.BuildAlgorithm));
	file.ReadNext(&tree.TotalNodeCount,sizeof(tree.TotalNodeCount));
	file.ReadNext(&tree.SubdivNodeCount,sizeof(tree.SubdivNodeCount));
	file.ReadNext(&tree.RenderNodeCount,sizeof(tree.RenderNodeCount));
	file.ReadNext(&tree.LODLevelCount,sizeof(tree.LODLevelCount));

	file.ReadNext(&tree.NodeAddresses.Size,sizeof(tree.NodeAddresses.Size));
	if(tree.NodeAddresses.Size)
	{
		tree.NodeAddresses.Longs = new LONG[tree.NodeAddresses.Size];
		file.ReadNext(tree.NodeAddresses.Longs,tree.NodeAddresses.Size * sizeof(LONG));
	}
	file.ReadNext(&tree.SubdivNodeAddresses.Size,sizeof(tree.SubdivNodeAddresses.Size));
	if(tree.SubdivNodeAddresses.Size)
	{
		tree.SubdivNodeAddresses.Longs = new LONG[tree.SubdivNodeAddresses.Size];
		file.ReadNext(tree.SubdivNodeAddresses.Longs,tree.SubdivNodeAddresses.Size * sizeof(LONG));
	}
	file.ReadNext(&tree.RenderNodeAddresses.Size,sizeof(tree.RenderNodeAddresses.Size));
	if(tree.RenderNodeAddresses.Size)
	{
		tree.RenderNodeAddresses.Longs = new LONG[tree.RenderNodeAddresses.Size];
		file.ReadNext(tree.RenderNodeAddresses.Longs,tree.RenderNodeAddresses.Size * sizeof(LONG));
	}

	if(tree.IsCorrupt())
	{
		if(MaterialMap)
		{
			delete[] MaterialMap;
			MaterialMap = NULL;
		}

		return(FALSE);
	}

	TISFILE_BASENODE BaseNode;
	AddressResolver Resolver;
	TIStaticGeometryRealTimeNode* pCurr = &Root;

	for(i = 0; i < tree.NodeAddresses.Size; i++)
	{
		
		if(i != 0)
		{
			pCurr = new TIStaticGeometryRealTimeNode();	
		}

		pCurr->ID = i;

		Resolver.Resolve(tree.NodeAddresses.Longs[i],pCurr);
		

		
		file.StartRead(BaseNode.Box,tree.NodeAddresses.Longs[i],6 * sizeof(FLOAT));
		file.ReadNext(&BaseNode.IsRenderNode,sizeof(BaseNode.IsRenderNode));
		if(BaseNode.IsRenderNode)
		{
			memcpy(pCurr->Box,BaseNode.Box,6 * sizeof(FLOAT));
			
			pCurr->pRenderData = new TIStaticGeometryRealTimeRenderData;
			file.ReadNext(pCurr->pRenderData->Center,3 * sizeof(FLOAT));

			file.ReadNext(&pCurr->pRenderData->Size,sizeof(pCurr->pRenderData->Size));
			if(pCurr->pRenderData->Size)
			{
				pCurr->pRenderData->pLOD = new TIStaticGeometryRealTimeLOD[pCurr->pRenderData->Size];
				for(int j = 0; j < pCurr->pRenderData->Size; j++)
				{
					file.ReadNext(&pCurr->pRenderData->pLOD[j].Size,sizeof(pCurr->pRenderData->pLOD[j].Size));
					if(pCurr->pRenderData->pLOD[j].Size)
					{
						pCurr->pRenderData->pLOD[j].pMaterialGroups = new TIStaticGeometryRealTimeMaterialGroup[pCurr->pRenderData->pLOD[j].Size];
							
						for(int k = 0; k < pCurr->pRenderData->pLOD[j].Size; k++)
						{
							file.ReadNext(&pCurr->pRenderData->pLOD[j].pMaterialGroups[k].MaterialIndex,sizeof(pCurr->pRenderData->pLOD[j].pMaterialGroups[k].MaterialIndex));
							pCurr->pRenderData->pLOD[j].pMaterialGroups[k].MaterialIndex = MaterialMap[pCurr->pRenderData->pLOD[j].pMaterialGroups[k].MaterialIndex];
							file.ReadNext(&pCurr->pRenderData->pLOD[j].pMaterialGroups[k].Size,sizeof(pCurr->pRenderData->pLOD[j].pMaterialGroups[k].Size));

							if(pCurr->pRenderData->pLOD[j].pMaterialGroups[k].Size)
							{
								pCurr->pRenderData->pLOD[j].pMaterialGroups[k].pVertices = new D3DVERTEX[pCurr->pRenderData->pLOD[j].pMaterialGroups[k].Size];
								file.ReadNext(&pCurr->pRenderData->pLOD[j].pMaterialGroups[k].RenderMode,sizeof(pCurr->pRenderData->pLOD[j].pMaterialGroups[k].RenderMode));
								
								for(int l = 0; l < pCurr->pRenderData->pLOD[j].pMaterialGroups[k].Size; l++)
								{
									file.ReadNext(&pCurr->pRenderData->pLOD[j].pMaterialGroups[k].pVertices[l],sizeof(D3DVERTEX));
								}
							}

						}
					}
				}

			}

		}
		else
		{
			memcpy(pCurr->Box,BaseNode.Box,6 * sizeof(FLOAT));

			LONG size;
			file.ReadNext(&size,sizeof(size));
			if(size)
			{
				pCurr->CreateChildArray(size);

				for(int j = 0; j < pCurr->Size; j++)
				{
					LONG NodeIndex;
					file.ReadNext(&NodeIndex,sizeof(NodeIndex));
					Resolver.Add(NodeIndex,(LPVOID*) &pCurr->pChildrenPointers[j]);
				}
			}
		}
		
	}




	if(Resolver.UnresolvedCount())
	{
		//there might be memory leaks!!!
		Root.Destroy();
		return(FALSE);
	}

	if(MaterialMap)
	{
		delete[] MaterialMap;
		MaterialMap = NULL;
	}
		

	return(TRUE);
}
