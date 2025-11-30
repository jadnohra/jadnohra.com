// TIStaticGeometryPreBuildModuleBinaryAreaSize.cpp: implementation of the TIStaticGeometryPreBuildModuleBinaryAreaSize class.
//
//////////////////////////////////////////////////////////////////////

#include "TIStaticGeometryPreBuildModuleBinaryAreaSize.h"
#include "..\..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"
#include "..\..\..\FileFormats\TISFile.h"
#include "..\..\..\General\Math\TIMath.cpp"
#include "limits.h"
#include "conio.h"
#include "mmsystem.h"

const int TIStaticGeometryPreBuildModuleBinaryAreaSize::MODE_XZ = 1;
const int TIStaticGeometryPreBuildModuleBinaryAreaSize::MODE_XY = 2;
const int TIStaticGeometryPreBuildModuleBinaryAreaSize::MODE_YZ = 3;

#define PRINTINTERVAL	1	//print update each 1 sec.

#define TRI_OUT		0
#define TRI_IN		1
#define TRI_INTER	2

#define TISVERSION	1

TIStaticGeometryPreBuildModuleBinaryAreaSize::TIStaticGeometryPreBuildModuleBinaryAreaSize()
{
	PrintInterval = PRINTINTERVAL*1000;
	ClipPolys = FALSE;
}

TIStaticGeometryPreBuildModuleBinaryAreaSize::~TIStaticGeometryPreBuildModuleBinaryAreaSize()
{
	
}


BOOL TIStaticGeometryPreBuildModuleBinaryAreaSize::CheckSetup()
{
	return( (ThresholdSizeDim1 >= 0)
			&& (ThresholdSizeDim2 >= 0)
			&& (Mode >= 1 && Mode <= 3)
			);
}

BOOL TIStaticGeometryPreBuildModuleBinaryAreaSize::Setup(FLOAT thresholdSizeDim1,FLOAT thresholdSizeDim2,int mode,BOOL PolyClip)
{
	ThresholdSizeDim1 = thresholdSizeDim1 ;
	ThresholdSizeDim2 = thresholdSizeDim2;
	Mode = mode;

	ClipPolys = PolyClip;

	return(CheckSetup());
}

VOID TIStaticGeometryPreBuildModuleBinaryAreaSize::PrintSetup()
{
	String str;

	if(CheckSetup())
	{
		str.Set("Setup OK");
	}
	else
	{
		str.Set("Setup ERROR");
	}
	
	switch(Mode)
	{
	case 1:
		str.Append(" X[");
		break;
	case 2:
		str.Append(" X[");
		break;
	case 3:
		str.Append(" Y[");
		break;
	default:
		str.Append(" ?[");
		break;
	}
	str.Append(ThresholdSizeDim1);
	str.Append("]");

	switch(Mode)
	{
	case 1:
		str.Append(" Z[");
		break;
	case 2:
		str.Append(" Y[");
		break;
	case 3:
		str.Append(" Z[");
		break;
	default:
		str.Append(" ?[");
		break;
	}
	str.Append(ThresholdSizeDim2);
	str.Append("]");

	Print(str,1);
}

BOOL TIStaticGeometryPreBuildModuleBinaryAreaSize::_Build(ThrashFile& outputFile)
{
	
	FLOAT Box[6];
	DWORD Time = timeGetTime();
	DWORD WaitTime = 0;
	String status;

	if(!CheckSetup())
	{
		PrintSetup();
		return(FALSE);
	}

	PrintSetup();

	/*
		determin bounding box
	*/
	
	status.Set("Determining Bouding Box ...");
	Print(status,1);	


	Box[TISFILE_BASENODE_MINDIM1] = FLT_MAX; 	
	Box[TISFILE_BASENODE_MAXDIM1] = -FLT_MAX; 	
	Box[TISFILE_BASENODE_MINDIM2] = FLT_MAX; 	
	Box[TISFILE_BASENODE_MAXDIM2] = -FLT_MAX; 	
	Box[TISFILE_BASENODE_MINDIM3] = FLT_MAX; 	
	Box[TISFILE_BASENODE_MAXDIM3] = -FLT_MAX; 	

	
	for(int i = 0; i < TempVertices.UsedCount; i++)
	{
		if(TempVertices.Array[i].x < Box[TISFILE_BASENODE_MINDIM1])
		{
			Box[TISFILE_BASENODE_MINDIM1] = TempVertices.Array[i].x;
		}

		if(TempVertices.Array[i].y < Box[TISFILE_BASENODE_MINDIM2])
		{
			Box[TISFILE_BASENODE_MINDIM2] = TempVertices.Array[i].y;
		}

		if(TempVertices.Array[i].z < Box[TISFILE_BASENODE_MINDIM3])
		{
			Box[TISFILE_BASENODE_MINDIM3] = TempVertices.Array[i].z;
		}


		if(TempVertices.Array[i].x > Box[TISFILE_BASENODE_MAXDIM1])
		{
			Box[TISFILE_BASENODE_MAXDIM1] = TempVertices.Array[i].x;
		}

		if(TempVertices.Array[i].y > Box[TISFILE_BASENODE_MAXDIM2])
		{
			Box[TISFILE_BASENODE_MAXDIM2] = TempVertices.Array[i].y;
		}

		if(TempVertices.Array[i].z > Box[TISFILE_BASENODE_MAXDIM3])
		{
			Box[TISFILE_BASENODE_MAXDIM3] = TempVertices.Array[i].z;
		}
	}

	status.Set("Bounding Box : X[");
	status.Append(Box[TISFILE_BASENODE_MINDIM1]);
	status.Append(',');
	status.Append(Box[TISFILE_BASENODE_MAXDIM1]);
	status.Append("] Y[");
	status.Append(Box[TISFILE_BASENODE_MINDIM2]);
	status.Append(',');
	status.Append(Box[TISFILE_BASENODE_MAXDIM2]);
	status.Append("] Z[");
	status.Append(Box[TISFILE_BASENODE_MINDIM3]);
	status.Append(',');
	status.Append(Box[TISFILE_BASENODE_MAXDIM3]);
	status.Append(']');

	Print(status,1);

	/*
		confirm continue
	*/

	PrintSetup();
	status.Set("Continue? (Escape to abort) ...");
	Print(status,1);
	WaitTime = WaitTime + timeGetTime();
	char c = getch();
	WaitTime = timeGetTime() - WaitTime;
	if(c == 27)
	{
		status.Set("Aborted by user ...");
		Print(status,1);
		return(FALSE);
	}
	
	ProcessedNodeCount = 0;	
	RenderNodeCount = 0;
	OutVertexCount = 0;
	LastPrintTime = timeGetTime();
		

	status.Set("Building tree ...");
	Print(status,1);
	
	PreBuildNode root;

	memcpy(&root.Box,&Box,6*sizeof(FLOAT));
	ProcessNode(&root);

	switch(Mode)
	{
	case 1:
		root.FindOutBoxY();
		break;
	case 2:
		root.FindOutBoxZ();
		break;
	case 3:
		root.FindOutBoxX();
		break;
	}

	Time = (timeGetTime() - Time);
	Time -= WaitTime;

	status.Set("Tree Built\r\n");
	status.Append(ProcessedNodeCount);
	status.Append(" Nodes ( ");
	status.Append(RenderNodeCount);
	status.Append("  Render Nodes ) (");
	status.Append((LONG) TempVertices.UsedCount);
	status.Append(" In Vertices ) (");
	status.Append(OutVertexCount);
	status.Append(" Out Vertices ) Processed in ");
	status.Append((FLOAT) ((FLOAT) Time / 1000.0f));
	status.Append(" sec.");
	Print(status,1);

	

	Time = timeGetTime();
	BOOL ok = WriteFile(outputFile,&root);
	Time = timeGetTime() - Time;
	status.Set("File Written in ");
	status.Append((FLOAT) ((FLOAT) Time / 1000.0f));
	status.Append(" sec.");
	Print(status,1);

	return(ok);
}


BOOL TIStaticGeometryPreBuildModuleBinaryAreaSize::WriteFile(ThrashFile& outputFile,PreBuildNode* pRoot)
{
	String status;
	
	status.Set("Writing To File");
	Print(status,1);
	
	LONG treeAddressPosID;
	LONG matAddressPosID;
	TISFILE_HEADER header;
	TISFILE_SCENEMANAGMENT scene;
	TISFILE_STRUCTADDRESS treeAddress;
	TISFILE_STRUCTADDRESS matAddress;
	TISFILE_TREE tree;
	int tempRenderNodeIndex = 0;
	int tempSubdivNodeIndex = 0;
	tree.LODLevelCount = 0;
	
	header.Build(1,TISFILE_VER,"TIStaticGeometryPreBuildModuleBinaryAreaSize ver. 1");
	scene.Build();
	treeAddress.Address = 0;

	outputFile.StartWrite(&header,0,sizeof(header));
	outputFile.WriteNext(&scene,sizeof(scene));
	

		
	CFastPrimitiveArrayKeepTogether<TreeNodeFlatNodeInfo> flatArray;
	pRoot->ToFlatNodePtrArrayByLevel(flatArray);

	tree.TotalNodeCount = flatArray.UsedCount;
	tree.RenderNodeCount = RenderNodeCount;
	tree.SubdivNodeCount = flatArray.UsedCount - RenderNodeCount;

	tree.NodeAddresses.Size = tree.TotalNodeCount;
	tree.NodeAddresses.Longs = new LONG[tree.NodeAddresses.Size];
	tree.SubdivNodeAddresses.Size = tree.SubdivNodeCount;
	tree.SubdivNodeAddresses.Longs = new LONG[tree.SubdivNodeAddresses.Size];
	tree.RenderNodeAddresses.Size = tree.RenderNodeCount;
	tree.RenderNodeAddresses.Longs = new LONG[tree.RenderNodeAddresses.Size];


	treeAddressPosID = flatArray.UsedCount; 
	outputFile.AddUnresolvedEntry(treeAddressPosID,outputFile.PointerPosition());
	outputFile.WriteNext(&treeAddress,sizeof(treeAddress));
	matAddressPosID = flatArray.UsedCount + 1; 
	outputFile.AddUnresolvedEntry(matAddressPosID,outputFile.PointerPosition());
	outputFile.WriteNext(&matAddress,sizeof(matAddress));

	
	for(int i = 0; i < flatArray.UsedCount; i++)
	{
		PreBuildNode* pNode = (PreBuildNode*) flatArray.Array[i].pNode;
		tree.NodeAddresses.Longs[i] = outputFile.PointerPosition();
		outputFile.ResolveEntry(i,outputFile.PointerPosition());
		
		if(pNode->IsRenderNode)
		{
			
			tree.RenderNodeAddresses.Longs[tempRenderNodeIndex++] = outputFile.PointerPosition();
			
			TISFILE_RENDERNODE node;
			node.IsRenderNode = 1;
			
			memcpy(node.Box,pNode->Box,6 * sizeof(FLOAT));
			node.Center[0] = (pNode->Box[0] + pNode->Box[1]) / 2.0f;
			node.Center[1] = (pNode->Box[2] + pNode->Box[3]) / 2.0f;
			node.Center[2] = (pNode->Box[4] + pNode->Box[5]) / 2.0f;

			outputFile.WriteNext(node.Box,6 * sizeof(FLOAT));
			outputFile.WriteNext(&node.IsRenderNode,sizeof(BYTE));
			outputFile.WriteNext(node.Center,3 * sizeof(FLOAT));
			
			node.LODLevels.Size = 0;
			if(pNode->pRenderData)
			{
				node.LODLevels.Size = pNode->pRenderData->size;
			}
			if(tree.LODLevelCount < node.LODLevels.Size)
			{
				tree.LODLevelCount = node.LODLevels.Size;
			}
			
			outputFile.WriteNext(&node.LODLevels.Size,sizeof(node.LODLevels.Size));

			for(int j = 0; j < node.LODLevels.Size; j++)
			{
				TISFILE_RENDERNODELODLEVEL level;
				level.Size = pNode->pRenderData->pLOD[j].size;
				outputFile.WriteNext(&level.Size,sizeof(level.Size));
				for(int k = 0; k < level.Size; k++)
				{
					level.Vertices.MaterialIndex = pNode->pRenderData->pLOD[j].pMatGroup[k].matIndex;
					outputFile.WriteNext(&level.Vertices.MaterialIndex,sizeof(level.Vertices.MaterialIndex));
					level.Vertices.Vertices.Size = pNode->pRenderData->pLOD[j].pMatGroup[k].size;
					outputFile.WriteNext(&level.Vertices.Vertices.Size,sizeof(level.Vertices.Vertices.Size));
					level.Vertices.Vertices.RenderMode = D3DPT_TRIANGLELIST;
					outputFile.WriteNext(&level.Vertices.Vertices.RenderMode,sizeof(level.Vertices.Vertices.RenderMode));
					for(int l = 0; l < level.Vertices.Vertices.Size; l++)
					{
						D3DVERTEX* pV = &pNode->pRenderData->pLOD[j].pMatGroup[k].pVertices[l];
						outputFile.WriteNext(pV,sizeof(D3DVERTEX));
					}
				}	
			}

			
		}
		else
		{
			
			tree.SubdivNodeAddresses.Longs[tempSubdivNodeIndex++] = outputFile.PointerPosition();
			
			TISFILE_SUBDIVNODE node;
			node.IsRenderNode = 0;
			memcpy(node.Box,pNode->Box,6 * sizeof(FLOAT));

			if(pNode->lpChildrenPointers && pNode->lpChildrenPointers->UsedCount)
			{
				node.SubnodeIndexes.Size = pNode->lpChildrenPointers->UsedCount;
				node.SubnodeIndexes.Longs = new LONG[node.SubnodeIndexes.Size];
				for(int j = 0; j < node.SubnodeIndexes.Size; j++)
				{
					
					node.SubnodeIndexes.Longs[j] = flatArray.Array[i].pChildIndexes[j];
					
				}
			}
			outputFile.WriteNext(node.Box,6 * sizeof(FLOAT));
			outputFile.WriteNext(&node.IsRenderNode,sizeof(BYTE));
			outputFile.WriteNext(&node.SubnodeIndexes.Size,sizeof(LONG));
			for(int j = 0; j < node.SubnodeIndexes.Size; j++)
			{
				outputFile.AddUnresolvedEntry(node.SubnodeIndexes.Longs[j],outputFile.PointerPosition());
				outputFile.WriteNext(&node.SubnodeIndexes.Longs[j],sizeof(LONG));
			}

		}
	}
	
	outputFile.ResolveEntry(i,outputFile.PointerPosition());
	tree.BuildAlgorithm = 1;
	outputFile.WriteNext(&tree.BuildAlgorithm,sizeof(tree.BuildAlgorithm));
	outputFile.WriteNext(&tree.TotalNodeCount,sizeof(tree.TotalNodeCount));
	outputFile.WriteNext(&tree.SubdivNodeCount,sizeof(tree.SubdivNodeCount));
	outputFile.WriteNext(&tree.RenderNodeCount,sizeof(tree.RenderNodeCount));
	outputFile.WriteNext(&tree.LODLevelCount,sizeof(tree.LODLevelCount));
	
	outputFile.WriteNext(&tree.NodeAddresses.Size,sizeof(tree.NodeAddresses.Size));
	outputFile.WriteNext(tree.NodeAddresses.Longs,tree.NodeAddresses.Size * sizeof(LONG));
	outputFile.WriteNext(&tree.SubdivNodeAddresses.Size,sizeof(tree.SubdivNodeAddresses.Size));
	outputFile.WriteNext(tree.SubdivNodeAddresses.Longs,tree.SubdivNodeAddresses.Size * sizeof(LONG));
	outputFile.WriteNext(&tree.RenderNodeAddresses.Size,sizeof(tree.RenderNodeAddresses.Size));
	outputFile.WriteNext(tree.RenderNodeAddresses.Longs,tree.RenderNodeAddresses.Size * sizeof(LONG));


	outputFile.ResolveEntry(i + 1,outputFile.PointerPosition());
	TISFILE_MATERIALARRAY mats;
	mats.Size = TempMaterials.UsedCount;
	outputFile.WriteNext(&mats.Size,sizeof(mats.Size));
	for(int x = 0; x < mats.Size; x++)
	{
		String temp;
		TISFILE_MATERIAL mat;
		
		temp.Set(TempMaterials.Array[x].pTextureFileName);
		mat.TextureFile.Size = temp.lSize;
		mat.TextureFile.Chars = temp.toNewLPCHAR();
		
		outputFile.WriteNext((D3DMATERIAL7*) &TempMaterials.Array[x],sizeof(D3DMATERIAL7));
		outputFile.WriteNext(&mat.TextureFile.Size,sizeof(mat.TextureFile.Size));
		outputFile.WriteNext(mat.TextureFile.Chars,mat.TextureFile.Size * sizeof(CHAR));
	}



	if(outputFile.UnresolvedEntryCount())
	{
		String status;
		status.Set("WARNING!!! UNEXPECTED ERROR @TIStaticGeometryPreBuildModuleBinaryAreaSize::WriteFile() : ");
		status.Append(outputFile.UnresolvedEntryCount());
		status.Append(" Unresolved Entries");
		Print(status,1);					

		return(FALSE);
	}
	
	return(TRUE);
}


BOOL TIStaticGeometryPreBuildModuleBinaryAreaSize::PointInside(D3DVERTEX* pV,FLOAT& dim1Min,FLOAT& dim1Max,FLOAT& dim2Min,FLOAT& dim2Max)
{
	switch(Mode)
	{
	case 1:
		if(	MACRO_TIMATH_BETWEENSORTED(pV->x,dim1Min,dim1Max)
			&&MACRO_TIMATH_BETWEENSORTED(pV->z,dim2Min,dim2Max)
			)
		{
			return(TRUE);	
		}
		break;
	case 2:
		if(	MACRO_TIMATH_BETWEENSORTED(pV->x,dim1Min,dim1Max)
			&&MACRO_TIMATH_BETWEENSORTED(pV->y,dim2Min,dim2Max)
			)
		{
			return(TRUE);	
		}
		break;
	case 3:
		if(	MACRO_TIMATH_BETWEENSORTED(pV->y,dim1Min,dim1Max)
			&&MACRO_TIMATH_BETWEENSORTED(pV->z,dim2Min,dim2Max)
			)
		{
			return(TRUE);	
		}
		break;
	}

	return(FALSE);
}


int TIStaticGeometryPreBuildModuleBinaryAreaSize::ProjectToTri(D3DVERTEX* pV[3],FLOAT& dim1Min,FLOAT& dim1Max,FLOAT& dim2Min,FLOAT& dim2Max,D3DVERTEX intersections[13],int& intersectionCount)
{
	FLOAT triNormal[3];
	FLOAT triK;
	FLOAT lines[4][2][3]; //line,vertex,coord
	int count = 0;
	
	TIInlineMathGetPlaneParams(pV,triNormal,triK);


	//lines of the wirframe of segplane along the inf. dim.
	switch(Mode)
	{
	case 1:
		lines[0][0][0] = dim1Max;
		lines[0][0][1] = 1;
		lines[0][0][2] = dim2Max;
		lines[0][1][0] = dim1Max;
		lines[0][1][1] = -1;
		lines[0][1][2] = dim2Max;

		lines[1][0][0] = dim1Max;
		lines[1][0][1] = 1;
		lines[1][0][2] = dim2Min;
		lines[1][1][0] = dim1Max;
		lines[1][1][1] = -1;
		lines[1][1][2] = dim2Min;

		lines[2][0][0] = dim1Min;
		lines[2][0][1] = 1;
		lines[2][0][2] = dim2Min;
		lines[2][1][0] = dim1Min;
		lines[2][1][1] = -1;
		lines[2][1][2] = dim2Min;

		lines[3][0][0] = dim1Min;
		lines[3][0][1] = 1;
		lines[3][0][2] = dim2Max;
		lines[3][1][0] = dim1Min;
		lines[3][1][1] = -1;
		lines[3][1][2] = dim2Max;

		break;
	case 2:
		lines[0][0][0] = dim1Max;
		lines[0][0][2] = 1;
		lines[0][0][1] = dim2Max;
		lines[0][1][0] = dim1Max;
		lines[0][1][2] = -1;
		lines[0][1][1] = dim2Max;

		lines[1][0][0] = dim1Max;
		lines[1][0][2] = 1;
		lines[1][0][1] = dim2Min;
		lines[1][1][0] = dim1Max;
		lines[1][1][2] = -1;
		lines[1][1][1] = dim2Min;

		lines[2][0][0] = dim1Min;
		lines[2][0][2] = 1;
		lines[2][0][1] = dim2Min;
		lines[2][1][0] = dim1Min;
		lines[2][1][2] = -1;
		lines[2][1][1] = dim2Min;

		lines[3][0][0] = dim1Min;
		lines[3][0][2] = 1;
		lines[3][0][1] = dim2Max;
		lines[3][1][0] = dim1Min;
		lines[3][1][2] = -1;
		lines[3][1][1] = dim2Max;

		break;
	case 3:
		lines[0][0][1] = dim1Max;
		lines[0][0][0] = 1;
		lines[0][0][2] = dim2Max;
		lines[0][1][1] = dim1Max;
		lines[0][1][0] = -1;
		lines[0][1][2] = dim2Max;

		lines[1][0][1] = dim1Max;
		lines[1][0][0] = 1;
		lines[1][0][2] = dim2Min;
		lines[1][1][1] = dim1Max;
		lines[1][1][0] = -1;
		lines[1][1][2] = dim2Min;

		lines[2][0][1] = dim1Min;
		lines[2][0][0] = 1;
		lines[2][0][2] = dim2Min;
		lines[2][1][1] = dim1Min;
		lines[2][1][0] = -1;
		lines[2][1][2] = dim2Min;

		lines[3][0][1] = dim1Min;
		lines[3][0][0] = 1;
		lines[3][0][2] = dim2Max;
		lines[3][1][1] = dim1Min;
		lines[3][1][0] = -1;
		lines[3][1][2] = dim2Max;

		break;
	}

	
	for(int i = 0; i < 4; i++)
	{
		if(TIInlineMathLineInterPlane(	lines[i][0][0],lines[i][0][1],lines[i][0][2],
										lines[i][1][0],lines[i][1][1],lines[i][1][2],
										triNormal,triK,
										intersections[intersectionCount].x,intersections[intersectionCount].y,intersections[intersectionCount].z
										)
										== 0)
		{
			//couldnt project
		}
		else
		{
			//TODO check if proj inside tri!!!
			if(TIInlineMathPtInsideTriAssumingInTriPlane(pV,&intersections[intersectionCount]))
			{
				intersectionCount++;
				count++;
			}
		
		}
	}

	
	//form a plane from tri v[0] and inter pt that is perp
	//to the tri plane (using plane normal)
	//intersect this plane with line v[1] v[2] -> temp
	//interpolate inter pt coords
	//interpolate intersection[i] coord between
	//v[0] and temp
	D3DVERTEX* temppV[3];
	FLOAT tempNormal[3];
	FLOAT tempK;
	D3DVERTEX temp1;
	D3DVERTEX temp2;

	for(i = intersectionCount - count; i < intersectionCount; i++)
	{
		memcpy(&temp1,&intersections[i], sizeof(D3DVERTEX));
		temp1.x += triNormal[0];
		temp1.y += triNormal[1];
		temp1.z += triNormal[2];
		
		temppV[0] = pV[0];
		temppV[1] = &intersections[i];
		temppV[2] = &temp1;
		
		TIInlineMathGetPlaneParams(temppV,tempNormal,tempK);
		TIInlineMathSegmentInterPlane(*pV[1], *pV[2],tempNormal,tempK,temp2);
		TIInlineMathBlindInterPolateNormalsAndUV(*pV[1],*pV[2],temp2);
		TIInlineMathBlindInterPolateNormalsAndUV(*pV[0],temp2,intersections[i]);	
		
		
	}
		
	return(count);

}


VOID TIStaticGeometryPreBuildModuleBinaryAreaSize::RemoveSimilarIntersections(D3DVERTEX intersections[13],int& intersectionCount)
{
	D3DVERTEX tempIntersections[13];
	int tempIntersectionCount;

	tempIntersectionCount = intersectionCount;
	memcpy(tempIntersections,intersections,13 * sizeof(D3DVERTEX));

	intersectionCount = 0;
	for(int i = 0; i < tempIntersectionCount; i++)
	{
		for(int j = 0; j < intersectionCount; j++)
		{
			if(memcmp(&intersections[j],&tempIntersections[i],sizeof(D3DVERTEX)) == 0)
			{
				break;
			}
		}

		if(j >= intersectionCount)
		{
			intersections[intersectionCount] = tempIntersections[i];
			intersectionCount++;
		}
	}

}

int TIStaticGeometryPreBuildModuleBinaryAreaSize::TriIntersectsSegmentPlanes(D3DVERTEX* pV1,D3DVERTEX* pV2,D3DVERTEX* pV3,FLOAT& dim1Min,FLOAT& dim1Max,FLOAT& dim2Min,FLOAT& dim2Max,SegmentPlane segmentPlanes[4],D3DVERTEX Intersections[13],int& IntersectionCount,D3DVERTEX* pSortedIntersections[13])
{
	D3DVERTEX* pV[3];
	IntersectionCount = 0;
	
	
	pV[0] = pV1;
	pV[1] = pV2;
	pV[2] = pV3;

	
	//get rid of case where all inside or border the segplane
	for(int i = 0; i < 3; i++)
	{
		if(PointInside(pV[i],dim1Min,dim1Max,dim2Min,dim2Max))
		{
			memcpy(&Intersections[IntersectionCount],pV[i],sizeof(D3DVERTEX));
			IntersectionCount++;
		}
	}
	if(IntersectionCount == 3)
	{
		return(TRI_IN);
	}

	int check = IntersectionCount;
	
	IntersectionCount = 13;
	TIInlineMathClipAgainstArea((Mode2D) Mode,pV,3,Intersections,IntersectionCount,segmentPlanes,4);
	if(IntersectionCount == 0)
	{
		if(check > 0)
		{
			String status;
			status.Set("WARNING!!! UNEXPECTED ERROR @ Ploygon was not clipped correctly and was removed (1)");
			Print(status,1);
		}
		return(TRI_OUT);	
	}

	if(IntersectionCount && IntersectionCount < 3)
	{
		String status;
		status.Set("WARNING!!! UNEXPECTED ERROR @ Ploygon was not clipped correctly and was removed (2)");
		Print(status,1);
	}

	for(i = 0; i < IntersectionCount; i++)
	{
		pSortedIntersections[i] = &Intersections[i];		
	}

	return(TRI_INTER);

	

	for(i = 0; i < 3; i++)
	{
		int i1 = i;
		int i2 = (i + 1) % 3;

		for(int j = 0; j < 4; j++)
		{
			if(TIInlineMathSegmentInterPlane(*pV[i1],*pV[i2],
											segmentPlanes[j].normal,segmentPlanes[j].k,
											Intersections[IntersectionCount]
											))
			{
				
				//line inter plane ok see if in segment plane
				int d = -1;
				switch(Mode)
				{
				case 1:
					if(j % 2)
					{
						d = 0;
						if(MACRO_TIMATH_BETWEEN(Intersections[IntersectionCount].x,segmentPlanes[j].limits[0][d],segmentPlanes[j].limits[1][d]))
						{
							IntersectionCount++;
						}
					}
					else
					{
						d = 2;
						if(MACRO_TIMATH_BETWEEN(Intersections[IntersectionCount].z,segmentPlanes[j].limits[0][d],segmentPlanes[j].limits[1][d]))
						{
							IntersectionCount++;	
						}
					}
					break;
				case 2:
					if(j % 2)
					{
						d = 0;
						if(MACRO_TIMATH_BETWEEN(Intersections[IntersectionCount].x,segmentPlanes[j].limits[0][d],segmentPlanes[j].limits[1][d]))
						{
							IntersectionCount++;	
						}
					}
					else
					{
						d = 1;
						if(MACRO_TIMATH_BETWEEN(Intersections[IntersectionCount].y,segmentPlanes[j].limits[0][d],segmentPlanes[j].limits[1][d]))
						{
							IntersectionCount++;	
						}
					}
					break;
				case 3:
					if(j % 2)
					{
						d = 1;
						if(MACRO_TIMATH_BETWEEN(Intersections[IntersectionCount].y,segmentPlanes[j].limits[0][d],segmentPlanes[j].limits[1][d]))
						{
							IntersectionCount++;	
						}
					}
					else
					{
						d = 2;
						if(MACRO_TIMATH_BETWEEN(Intersections[IntersectionCount].z,segmentPlanes[j].limits[0][d],segmentPlanes[j].limits[1][d]))
						{
							IntersectionCount++;	
						}
					}
					break;
				}
				
			}
		}
	}

	ProjectToTri(pV,dim1Min,dim1Max,dim2Min,dim2Max,Intersections,IntersectionCount);
	
	if(IntersectionCount == 0)
	{
		return(TRI_OUT);
	}
	
	if(IntersectionCount < 3)
	{
		String status;
		status.Set("WARNING!!! UNEXPECTED ERROR @ TIStaticGeometryPreBuildModuleBinaryAreaSize::TriIntersectsSegmentPlanes():1");
		Print(status,1);
	}

	
	RemoveSimilarIntersections(Intersections,IntersectionCount);
	
	if(IntersectionCount > 3)
	{
	
		switch(Mode)
		{
		case 1:
			if( !((TIInlineMathOrderCWInXZPlane(Intersections,IntersectionCount,pSortedIntersections))
				||(TIInlineMathOrderCWInXYPlane(Intersections,IntersectionCount,pSortedIntersections))
				||(TIInlineMathOrderCWInYZPlane(Intersections,IntersectionCount,pSortedIntersections))
				))
			{
				String status;
				status.Set("WARNING!!! UNEXPECTED ERROR @ TIStaticGeometryPreBuildModuleBinaryAreaSize::TriIntersectsSegmentPlanes():2");
				Print(status,1);
			}
			break;
		case 2:
			if(	!((TIInlineMathOrderCWInXYPlane(Intersections,IntersectionCount,pSortedIntersections))
				||(TIInlineMathOrderCWInXZPlane(Intersections,IntersectionCount,pSortedIntersections))
				||(TIInlineMathOrderCWInYZPlane(Intersections,IntersectionCount,pSortedIntersections))
				))
			{
				String status;
				status.Set("WARNING!!! UNEXPECTED ERROR @ TIStaticGeometryPreBuildModuleBinaryAreaSize::TriIntersectsSegmentPlanes():2");
				Print(status,1);
			}
			break;
		case 3:
			if(	!((TIInlineMathOrderCWInYZPlane(Intersections,IntersectionCount,pSortedIntersections))
				||(TIInlineMathOrderCWInXZPlane(Intersections,IntersectionCount,pSortedIntersections))
				||(TIInlineMathOrderCWInXYPlane(Intersections,IntersectionCount,pSortedIntersections))
				))
			{
				String status;
				status.Set("WARNING!!! UNEXPECTED ERROR @ TIStaticGeometryPreBuildModuleBinaryAreaSize::TriIntersectsSegmentPlanes():2");
				Print(status,1);
			}
			break;
		}
	}
	else
	{
		pSortedIntersections[0] = &Intersections[0];
		pSortedIntersections[1] = &Intersections[1];
		pSortedIntersections[2] = &Intersections[2];
	}

	
	
	return(TRI_INTER);
	
}


VOID TIStaticGeometryPreBuildModuleBinaryAreaSize::ProcessNode(PreBuildNode* pNode)
{
	
	if(timeGetTime() - LastPrintTime >= (DWORD) PrintInterval)
	{
		LastPrintTime = timeGetTime();
		String temp;
		temp.Set("Processing Node ");
		temp.Append(ProcessedNodeCount);
		Print(temp,1);
	}

	
	pNode->IsRenderNode = TRUE;
	
	
	switch(Mode)
	{
	case 1:
		if(pNode->Box[1] - pNode->Box[0] > ThresholdSizeDim1)
		{
			pNode->IsRenderNode = FALSE;
		}
		
		if(pNode->Box[5] - pNode->Box[4] > ThresholdSizeDim2)
		{
			pNode->IsRenderNode = FALSE;
		}
		break;
	case 2:
		if(pNode->Box[1] - pNode->Box[0] > ThresholdSizeDim1)
		{
			pNode->IsRenderNode = FALSE;
		}
		
		if(pNode->Box[3] - pNode->Box[2] > ThresholdSizeDim2)
		{
			pNode->IsRenderNode = FALSE;
		}
		break;
	case 3:
		if(pNode->Box[3] - pNode->Box[2] > ThresholdSizeDim1)
		{
			pNode->IsRenderNode = FALSE;
		}
		
		if(pNode->Box[5] - pNode->Box[4] > ThresholdSizeDim2)
		{
			pNode->IsRenderNode = FALSE;
		}
		break;
	}
	
	

	if(pNode->IsRenderNode)
	{
		RenderNodeCount++;
		SegmentPlane segmentPlanes[4];
		
		//	TODO SPLIT!!!!!!!!!!!!!!!!!!!!!!!!!!
		
		switch(Mode)
		{
		case 1:
			{
				CFastPrimitiveArrayKeepTogether<PreBuildVertex>	NodeVertices;
				D3DVERTEX Intersections[13];
				D3DVERTEX* pSortedIntersections[13];
				int IntersectionCount;
				
				NodeVertices.Create(60);
				TIMathBuildSegmentPlanes((Mode2D) Mode,pNode->Box[0],pNode->Box[1],pNode->Box[4],pNode->Box[5],segmentPlanes);
				for(int i = 0; i < TempVertices.UsedCount; i+=3)
				{
									
				
					switch(TriIntersectsSegmentPlanes(&TempVertices.Array[i],&TempVertices.Array[i + 1],&TempVertices.Array[i + 2],pNode->Box[0],pNode->Box[1],pNode->Box[4],pNode->Box[5],segmentPlanes,Intersections,IntersectionCount,pSortedIntersections))
					{
					case TRI_OUT:
						break;
					case TRI_IN:
						*NodeVertices.New()	= TempVertices.Array[i];
						*NodeVertices.New()	= TempVertices.Array[i + 1];
						*NodeVertices.New()	= TempVertices.Array[i + 2];
						break;
					case TRI_INTER:
						if(ClipPolys)
						{
							if(IntersectionCount < 3)
							{
								String status;
								status.Set("WARNING!!! UNEXPECTED ERROR @TIStaticGeometryPreBuildModuleBinaryAreaSize::ProcessNode()");
								Print(status,1);					
							}
							else
							{
								PreBuildVertex* pTemp = new PreBuildVertex[IntersectionCount];
								for(int j = 0; j < IntersectionCount; j++)
								{
									pTemp[j].Set(*pSortedIntersections[j],TempVertices.Array[i].MaterialIndex);
									pTemp[j].LOD = TempVertices.Array[i].LOD;
								}
								for(int x = 1; x < IntersectionCount; x++)
								{
									if(pTemp[x].tu == pTemp[x - 1].tu && pTemp[x].tv == pTemp[x - 1].tv)
									{
										String status;
										status.Set("WARNING!!! UNEXPECTED ERROR Possible Textue Mapping Problem");
										Print(status,1);
									}
								}
								*NodeVertices.New()	= pTemp[0];
								*NodeVertices.New()	= pTemp[1];
								*NodeVertices.New()	= pTemp[2];

								for(j = 3; j < IntersectionCount; j++)
								{
									*NodeVertices.New()	= pTemp[0];
									*NodeVertices.New()	= pTemp[j-1];
									*NodeVertices.New()	= pTemp[j];
								}
								delete[] pTemp;

							}
						}
						else
						{
							*NodeVertices.New()	= TempVertices.Array[i];
							*NodeVertices.New()	= TempVertices.Array[i + 1];
							*NodeVertices.New()	= TempVertices.Array[i + 2];
						}
						break;
					}
				}

				if(NodeVertices.UsedCount > 0)
				{
					OutVertexCount += NodeVertices.UsedCount;
					pNode->pRenderData = new PreBuildNodeRenderData();
					pNode->pRenderData->Build(&NodeVertices);
				}
			}
			break;
		case 2:
			////////////
			/// AFTER FINISHING CASE 1 PUT HERE TOOO!!!!!!!!!!!!!!!!!!!!!!
			////
			break;
		case 3:
			////////////
			/// AFTER FINISHING CASE 1 PUT HERE TOOO!!!!!!!!!!!!!!!!!!!!!!
			////
			break;
		}
		
		

	}
	else
	{
		PreBuildNode* pChild[4];
		pChild[0] = new PreBuildNode;
		pChild[1] = new PreBuildNode;
		pChild[2] = new PreBuildNode;
		pChild[3] = new PreBuildNode;

		
		switch(Mode)
		{
		case 1:
			pChild[0]->SetBox(pNode->Box[0],(pNode->Box[0]+pNode->Box[1]) / 2.0f,0,0,pNode->Box[4],(pNode->Box[4]+pNode->Box[5]) / 2.0f);
			pChild[1]->SetBox((pNode->Box[0]+pNode->Box[1]) / 2.0f,pNode->Box[1],0,0,pNode->Box[4],(pNode->Box[4]+pNode->Box[5]) / 2.0f);
			pChild[2]->SetBox(pNode->Box[0],(pNode->Box[0]+pNode->Box[1]) / 2.0f,0,0,(pNode->Box[4]+pNode->Box[5]) / 2.0f,pNode->Box[5]);
			pChild[3]->SetBox((pNode->Box[0]+pNode->Box[1]) / 2.0f,pNode->Box[1],0,0,(pNode->Box[4]+pNode->Box[5]) / 2.0f,pNode->Box[5]);
			break;
		case 2:
			pChild[0]->SetBox(pNode->Box[0],(pNode->Box[0]+pNode->Box[1]) / 2.0f,pNode->Box[2],(pNode->Box[2]+pNode->Box[3]) / 2.0f,0,0);
			pChild[1]->SetBox((pNode->Box[0]+pNode->Box[1]) / 2.0f,pNode->Box[1],pNode->Box[2],(pNode->Box[2]+pNode->Box[3]) / 2.0f,0,0);
			pChild[2]->SetBox(pNode->Box[0],(pNode->Box[0]+pNode->Box[1]) / 2.0f,(pNode->Box[2]+pNode->Box[3]) / 2.0f,pNode->Box[3],0,0);
			pChild[3]->SetBox((pNode->Box[0]+pNode->Box[1]) / 2.0f,pNode->Box[1],(pNode->Box[2]+pNode->Box[3]) / 2.0f,pNode->Box[3],0,0);
			break;
		case 3:
			pChild[0]->SetBox(0,0,pNode->Box[2],(pNode->Box[2]+pNode->Box[3]) / 2.0f,pNode->Box[4],(pNode->Box[4]+pNode->Box[5]) / 2.0f);
			pChild[1]->SetBox(0,0,(pNode->Box[2]+pNode->Box[3]) / 2.0f,pNode->Box[3],pNode->Box[4],(pNode->Box[4]+pNode->Box[5]) / 2.0f);
			pChild[2]->SetBox(0,0,pNode->Box[2],(pNode->Box[2]+pNode->Box[3]) / 2.0f,(pNode->Box[4]+pNode->Box[5]) / 2.0f,pNode->Box[5]);
			pChild[3]->SetBox(0,0,(pNode->Box[2]+pNode->Box[3]) / 2.0f,pNode->Box[3],(pNode->Box[4]+pNode->Box[5]) / 2.0f,pNode->Box[5]);
			break;
		}

		
		pNode->AddChild(pChild[0]);
		pNode->AddChild(pChild[1]);
		pNode->AddChild(pChild[2]);
		pNode->AddChild(pChild[3]);

		ProcessNode(pChild[0]);
		ProcessNode(pChild[1]);
		ProcessNode(pChild[2]);
		ProcessNode(pChild[3]);

		
		
		switch(Mode)
		{
		case 1:
			{
				pNode->Box[2] = FLT_MAX;
				pNode->Box[3] = -FLT_MAX;
				
				for(int i = 0; i < 4; i++)
				{
					if(pChild[i]->Box[2] < pNode->Box[2])
					{
						pNode->Box[2] = pChild[i]->Box[2];
					}
					
					if(pChild[i]->Box[3] > pNode->Box[3])
					{
						pNode->Box[3] = pChild[i]->Box[3];
					}
				}
			}				
			break;
		case 2:
			{
				pNode->Box[4] = FLT_MAX;
				pNode->Box[5] = -FLT_MAX;
				
				for(int i = 0; i < 4; i++)
				{
					if(pChild[i]->Box[4] < pNode->Box[4])
					{
						pNode->Box[4] = pChild[i]->Box[4];
					}
					
					if(pChild[i]->Box[5] > pNode->Box[5])
					{
						pNode->Box[5] = pChild[i]->Box[5];
					}
				}
			}
			break;
		case 3:
			{
				pNode->Box[0] = FLT_MAX;
				pNode->Box[1] = -FLT_MAX;
				
				for(int i = 0; i < 4; i++)
				{
					if(pChild[i]->Box[0] < pNode->Box[0])
					{
						pNode->Box[0] = pChild[i]->Box[0];
					}
					
					if(pChild[i]->Box[1] > pNode->Box[1])
					{
						pNode->Box[1] = pChild[i]->Box[1];
					}
				}
			}			
			break;
		}
		
		

	}

	ProcessedNodeCount++;
	
}