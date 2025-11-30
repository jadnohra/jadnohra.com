#ifndef _TISTATICGEOMETRYREALTIMESTRUCTS_H__
#define _TISTATICGEOMETRYREALTIMESTRUCTS_H__

#include "..\..\_TIINCLUDES.h"
#include "..\..\TIMaterialManager.h"
#include "..\..\..\General\Collection\PreAllocedTree.h"
#include "..\..\..\..\Maben\DX\d3dmath.h"
#include "..\..\..\..\Maben\DX\d3dutil.h"
#include "..\..\TIFrustum.h"
#include "..\..\..\General\Math\TIMath.cpp"

//#define TISTATICGEOMETRYREALTIMENODE_NORENDER
#define TISTATICGEOMETRYREALTIMENODE_PROFILING 
//#define TISTATICGEOMETRYREALTIMENODE_RENDERBOX 

struct TIStaticGeometryRealTimeMaterialGroup
{
	
	static LONG RenderedPolyCount;
	static LONG RenderedVertexCount;
	
	D3DVERTEX*	pVertices;
	WORD		MaterialIndex;
	WORD		Size;

	DWORD		RenderMode;

	TIStaticGeometryRealTimeMaterialGroup()
	{
		pVertices = NULL;
		Size = 0;
	}

	~TIStaticGeometryRealTimeMaterialGroup()
	{
		Destroy();
	}

	VOID Destroy()
	{
		if(pVertices)
		{
			delete[] pVertices;
			pVertices = NULL;
			Size = 0;
		}
	}

	inline VOID Render()
	{
		#ifdef TISTATICGEOMETRYREALTIMENODE_NORENDER
			return;
		#endif
		
		TIInlineSetMaterial(MaterialIndex);
		
		TIMaterialManager::pCurrTIMaterialManager->pDevice->DrawPrimitive(
										(D3DPRIMITIVETYPE) RenderMode,
										D3DFVF_VERTEX,
										pVertices,
										Size,
										NULL);

		#ifdef TISTATICGEOMETRYREALTIMENODE_PROFILING 
			RenderedVertexCount += Size;
			switch((D3DPRIMITIVETYPE) RenderMode)
			{
			case D3DPT_TRIANGLELIST:
				RenderedPolyCount += Size / 3;
				break;
			case D3DPT_TRIANGLESTRIP:
			case D3DPT_TRIANGLEFAN:
				RenderedPolyCount += Size - 2;
				break;
			}
		#endif
	
	
	}
};

struct TIStaticGeometryRealTimeLOD
{
	TIStaticGeometryRealTimeMaterialGroup*	pMaterialGroups;
	WORD									Size;

	TIStaticGeometryRealTimeLOD()
	{
		pMaterialGroups = NULL;
		Size = 0;
	}

	~TIStaticGeometryRealTimeLOD()
	{
		Destroy();
	}

	VOID Destroy()
	{
		if(pMaterialGroups)
		{
			delete[] pMaterialGroups;
			pMaterialGroups = NULL;
			Size = 0;

		}
	}

	inline VOID Render()
	{
		for(int i = 0; i < Size; i++)
		{
			pMaterialGroups[i].Render();
		}
	}

};

struct TIStaticGeometryRealTimeRenderData
{
	FLOAT							Center[3];
	TIStaticGeometryRealTimeLOD*	pLOD;
	BYTE							Size;

	TIStaticGeometryRealTimeRenderData()
	{
		Size = 0;
		pLOD = NULL;
	}
	
	~TIStaticGeometryRealTimeRenderData()
	{
		Destroy();
	}

	VOID Destroy()
	{
		if(pLOD)
		{
			delete[] pLOD;
			pLOD = NULL;
			Size = 0;
		}
	}

	inline VOID Render()
	{
		for(int i = 0; i < Size; i++)
		{
			pLOD[i].Render();
		}

	}
};


struct TIStaticGeometryRealTimeNode : public PreAllocedTreeNode
{
	FLOAT Box[6];
	TIStaticGeometryRealTimeRenderData* pRenderData;
	
	static DWORD RenderID;
	static D3DMATRIX ViewProjectionMatrix;
	static LONG RenderedNodeCount;
	static D3DVECTOR FrustumPoints[8];
	static TIFrustum Frustum;


	DWORD ID;

	TIStaticGeometryRealTimeNode()
	{
		pRenderData = NULL;
	}

	~TIStaticGeometryRealTimeNode()
	{
		Destroy();
	}

	VOID Destroy()
	{
		if(pRenderData)
		{
			delete pRenderData;
			pRenderData = NULL;
		}
	}

	static VOID PrepareRenderCycle()
	{
		D3DMATRIX world;
		D3DMATRIX view,proj;
		
	
		D3DUtil_SetIdentityMatrix(world);
		TIMaterialManager::pCurrTIMaterialManager->pDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &world);
		TIMaterialManager::pCurrTIMaterialManager->pDevice->GetTransform(D3DTRANSFORMSTATE_VIEW, &view);
		TIMaterialManager::pCurrTIMaterialManager->pDevice->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &proj);
	
		D3DMath_MatrixMultiply(ViewProjectionMatrix,proj,view);
		RenderedNodeCount = 0;
		TIStaticGeometryRealTimeMaterialGroup::RenderedPolyCount = 0;
		TIStaticGeometryRealTimeMaterialGroup::RenderedVertexCount = 0;
		
		Frustum.Setup(proj);
		Frustum.TransformToWorld(view);

		FrustumPoints[0] = Frustum.Near.WorldPts[0];
		FrustumPoints[1] = Frustum.Near.WorldPts[1];
		FrustumPoints[2] = Frustum.Near.WorldPts[2];
		FrustumPoints[3] = Frustum.Near.WorldPts[3];
		FrustumPoints[4] = Frustum.Far.WorldPts[0];
		FrustumPoints[5] = Frustum.Far.WorldPts[1];
		FrustumPoints[6] = Frustum.Far.WorldPts[2];
		FrustumPoints[7] = Frustum.Far.WorldPts[3];

	}

	VOID RenderBox()
	{
		D3DVERTEX v[5];

		v[0].x = Box[0];
		v[0].y = Box[2];
		v[0].z = Box[4];
		v[1].x = Box[1];
		v[1].y = Box[2];
		v[1].z = Box[4];
		v[2].x = Box[1];
		v[2].y = Box[2];
		v[2].z = Box[5];
		v[3].x = Box[0];
		v[3].y = Box[2];
		v[3].z = Box[5];
		v[4].x = Box[0];
		v[4].y = Box[2];
		v[4].z = Box[4];

		TIMaterialManager::pCurrTIMaterialManager->pDevice->DrawPrimitive(
										D3DPT_LINESTRIP,
										D3DFVF_VERTEX,
										v,
										5,
										NULL);

		v[0].x = Box[0];
		v[0].y = Box[3];
		v[0].z = Box[4];
		v[1].x = Box[1];
		v[1].y = Box[3];
		v[1].z = Box[4];
		v[2].x = Box[1];
		v[2].y = Box[3];
		v[2].z = Box[5];
		v[3].x = Box[0];
		v[3].y = Box[3];
		v[3].z = Box[5];
		v[4].x = Box[0];
		v[4].y = Box[3];
		v[4].z = Box[4];

		TIMaterialManager::pCurrTIMaterialManager->pDevice->DrawPrimitive(
										D3DPT_LINESTRIP,
										D3DFVF_VERTEX,
										v,
										5,
										NULL);
	
		v[0].x = Box[0];
		v[0].y = Box[2];
		v[0].z = Box[4];
		v[1].x = Box[0];
		v[1].y = Box[3];
		v[1].z = Box[4];
		TIMaterialManager::pCurrTIMaterialManager->pDevice->DrawPrimitive(
										D3DPT_LINESTRIP,
										D3DFVF_VERTEX,
										v,
										2,
										NULL);

		v[0].x = Box[1];
		v[0].y = Box[2];
		v[0].z = Box[4];
		v[1].x = Box[1];
		v[1].y = Box[3];
		v[1].z = Box[4];
		TIMaterialManager::pCurrTIMaterialManager->pDevice->DrawPrimitive(
										D3DPT_LINESTRIP,
										D3DFVF_VERTEX,
										v,
										2,
										NULL);

		v[0].x = Box[0];
		v[0].y = Box[2];
		v[0].z = Box[5];
		v[1].x = Box[0];
		v[1].y = Box[3];
		v[1].z = Box[5];
		TIMaterialManager::pCurrTIMaterialManager->pDevice->DrawPrimitive(
										D3DPT_LINESTRIP,
										D3DFVF_VERTEX,
										v,
										2,
										NULL);

		v[0].x = Box[1];
		v[0].y = Box[2];
		v[0].z = Box[5];
		v[1].x = Box[1];
		v[1].y = Box[3];
		v[1].z = Box[5];
		TIMaterialManager::pCurrTIMaterialManager->pDevice->DrawPrimitive(
										D3DPT_LINESTRIP,
										D3DFVF_VERTEX,
										v,
										2,
										NULL);


		
		
	}

	VOID RenderFrustum()
	{
		D3DVERTEX v[2];
		
		v[0].x = FrustumPoints[0].x;
		v[0].y = FrustumPoints[0].y;
		v[0].z = FrustumPoints[0].z;
		v[1].x = FrustumPoints[6].x;
		v[1].y = FrustumPoints[6].y;
		v[1].z = FrustumPoints[6].z;
		TIMaterialManager::pCurrTIMaterialManager->pDevice->DrawPrimitive(
										D3DPT_LINESTRIP,
										D3DFVF_VERTEX,
										v,
										2,
										NULL);

		v[0].x = FrustumPoints[1].x;
		v[0].y = FrustumPoints[1].y;
		v[0].z = FrustumPoints[1].z;
		v[1].x = FrustumPoints[7].x;
		v[1].y = FrustumPoints[7].y;
		v[1].z = FrustumPoints[7].z;
		TIMaterialManager::pCurrTIMaterialManager->pDevice->DrawPrimitive(
										D3DPT_LINESTRIP,
										D3DFVF_VERTEX,
										v,
										2,
										NULL);
		v[0].x = FrustumPoints[2].x;
		v[0].y = FrustumPoints[2].y;
		v[0].z = FrustumPoints[2].z;
		v[1].x = FrustumPoints[4].x;
		v[1].y = FrustumPoints[4].y;
		v[1].z = FrustumPoints[4].z;
		TIMaterialManager::pCurrTIMaterialManager->pDevice->DrawPrimitive(
										D3DPT_LINESTRIP,
										D3DFVF_VERTEX,
										v,
										2,
										NULL);
		v[0].x = FrustumPoints[3].x;
		v[0].y = FrustumPoints[3].y;
		v[0].z = FrustumPoints[3].z;
		v[1].x = FrustumPoints[5].x;
		v[1].y = FrustumPoints[5].y;
		v[1].z = FrustumPoints[5].z;
		TIMaterialManager::pCurrTIMaterialManager->pDevice->DrawPrimitive(
										D3DPT_LINESTRIP,
										D3DFVF_VERTEX,
										v,
										2,
										NULL);
	}

	VOID RenderAll()
	{
		if(pRenderData)
		{
			#ifdef TISTATICGEOMETRYREALTIMENODE_PROFILING 
				RenderedNodeCount++;
			#endif
			//if(ID == RenderID)
			{
				pRenderData->Render();
			}
			#ifdef TISTATICGEOMETRYREALTIMENODE_RENDERBOX
				RenderBox();
			#endif
		}
		else
		{
			for(int i = 0; i < Size; i++)
			{
				((TIStaticGeometryRealTimeNode*) pChildrenPointers[i])->RenderAll();
			}
			
		}
	}

	VOID Render()
	{
		
		switch(BoxInterFrustum())
		{
		case 0:
			return;
			break;
		case 1:
			RenderAll();
			return;
			break;
		}
		
		if(pRenderData)
		{
			#ifdef TISTATICGEOMETRYREALTIMENODE_PROFILING 
				RenderedNodeCount++;
			#endif
			//if(ID == RenderID)
			{
				pRenderData->Render();
			}
			#ifdef TISTATICGEOMETRYREALTIMENODE_RENDERBOX
				RenderBox();
			#endif
		}
		else
		{
			
			for(int i = 0; i < Size; i++)
			{
				((TIStaticGeometryRealTimeNode*) pChildrenPointers[i])->Render();
			}
			
		}
	}

	inline int BoxInterFrustum() //0->no, 1->inside, other->inter
	{
    
		int count = 0;	
		FLOAT x,y,z,w;

		x = Box[0]*ViewProjectionMatrix._11 + Box[2]*ViewProjectionMatrix._21 + Box[4]* ViewProjectionMatrix._31 + ViewProjectionMatrix._41;
		y = Box[0]*ViewProjectionMatrix._12 + Box[2]*ViewProjectionMatrix._22 + Box[4]* ViewProjectionMatrix._32 + ViewProjectionMatrix._42;
		z = Box[0]*ViewProjectionMatrix._13 + Box[2]*ViewProjectionMatrix._23 + Box[4]* ViewProjectionMatrix._33 + ViewProjectionMatrix._43;
		w = Box[0]*ViewProjectionMatrix._14 + Box[2]*ViewProjectionMatrix._24 + Box[4]* ViewProjectionMatrix._34 + ViewProjectionMatrix._44;
    	if( w < g_EPSILON && w > -g_EPSILON)
		{
			return(2);
		}
		if( ((x/=w) >= -1 && x <= 1) 
			&&((y/=w) >= -1 && y <= 1) 
			&&((z/=w) >= 0 && z <= 1) 
			)
		{
			count++;
		}

		x = Box[0]*ViewProjectionMatrix._11 + Box[3]*ViewProjectionMatrix._21 + Box[4]* ViewProjectionMatrix._31 + ViewProjectionMatrix._41;
		y = Box[0]*ViewProjectionMatrix._12 + Box[3]*ViewProjectionMatrix._22 + Box[4]* ViewProjectionMatrix._32 + ViewProjectionMatrix._42;
		z = Box[0]*ViewProjectionMatrix._13 + Box[3]*ViewProjectionMatrix._23 + Box[4]* ViewProjectionMatrix._33 + ViewProjectionMatrix._43;
		w = Box[0]*ViewProjectionMatrix._14 + Box[3]*ViewProjectionMatrix._24 + Box[4]* ViewProjectionMatrix._34 + ViewProjectionMatrix._44;
    	if( w < g_EPSILON && w > -g_EPSILON)
		{
			return(2);
		}
		if( ((x/=w) >= -1 && x <= 1) 
			&&((y/=w) >= -1 && y <= 1) 
			&&((z/=w) >= 0 && z <= 1) 
			)
		{
			count++;
			if(count < 2)
			{
				return(2);
			}
		}

		x = Box[1]*ViewProjectionMatrix._11 + Box[2]*ViewProjectionMatrix._21 + Box[4]* ViewProjectionMatrix._31 + ViewProjectionMatrix._41;
		y = Box[1]*ViewProjectionMatrix._12 + Box[2]*ViewProjectionMatrix._22 + Box[4]* ViewProjectionMatrix._32 + ViewProjectionMatrix._42;
		z = Box[1]*ViewProjectionMatrix._13 + Box[2]*ViewProjectionMatrix._23 + Box[4]* ViewProjectionMatrix._33 + ViewProjectionMatrix._43;
		w = Box[1]*ViewProjectionMatrix._14 + Box[2]*ViewProjectionMatrix._24 + Box[4]* ViewProjectionMatrix._34 + ViewProjectionMatrix._44;
    	if( w < g_EPSILON && w > -g_EPSILON)
		{
			return(2);
		}
		if( ((x/=w) >= -1 && x <= 1) 
			&&((y/=w) >= -1 && y <= 1) 
			&&((z/=w) >= 0 && z <= 1) 
			)
		{
			count++;
			if(count < 3)
			{
				return(2);
			}
		}

		x = Box[1]*ViewProjectionMatrix._11 + Box[3]*ViewProjectionMatrix._21 + Box[4]* ViewProjectionMatrix._31 + ViewProjectionMatrix._41;
		y = Box[1]*ViewProjectionMatrix._12 + Box[3]*ViewProjectionMatrix._22 + Box[4]* ViewProjectionMatrix._32 + ViewProjectionMatrix._42;
		z = Box[1]*ViewProjectionMatrix._13 + Box[3]*ViewProjectionMatrix._23 + Box[4]* ViewProjectionMatrix._33 + ViewProjectionMatrix._43;
		w = Box[1]*ViewProjectionMatrix._14 + Box[3]*ViewProjectionMatrix._24 + Box[4]* ViewProjectionMatrix._34 + ViewProjectionMatrix._44;
    	if( w < g_EPSILON && w > -g_EPSILON)
		{
			return(2);
		}
		if( ((x/=w) >= -1 && x <= 1) 
			&&((y/=w) >= -1 && y <= 1) 
			&&((z/=w) >= 0 && z <= 1) 
			)
		{
			count++;
			if(count < 4)
			{
				return(2);
			}
		}



		x = Box[0]*ViewProjectionMatrix._11 + Box[2]*ViewProjectionMatrix._21 + Box[5]* ViewProjectionMatrix._31 + ViewProjectionMatrix._41;
		y = Box[0]*ViewProjectionMatrix._12 + Box[2]*ViewProjectionMatrix._22 + Box[5]* ViewProjectionMatrix._32 + ViewProjectionMatrix._42;
		z = Box[0]*ViewProjectionMatrix._13 + Box[2]*ViewProjectionMatrix._23 + Box[5]* ViewProjectionMatrix._33 + ViewProjectionMatrix._43;
		w = Box[0]*ViewProjectionMatrix._14 + Box[2]*ViewProjectionMatrix._24 + Box[5]* ViewProjectionMatrix._34 + ViewProjectionMatrix._44;
    	if( w < g_EPSILON && w > -g_EPSILON)
		{
			return(2);
		}
		if( ((x/=w) >= -1 && x <= 1) 
			&&((y/=w) >= -1 && y <= 1) 
			&&((z/=w) >= 0 && z <= 1) 
			)
		{
			count++;
			if(count < 5)
			{
				return(2);
			}
		}

		x = Box[0]*ViewProjectionMatrix._11 + Box[3]*ViewProjectionMatrix._21 + Box[5]* ViewProjectionMatrix._31 + ViewProjectionMatrix._41;
		y = Box[0]*ViewProjectionMatrix._12 + Box[3]*ViewProjectionMatrix._22 + Box[5]* ViewProjectionMatrix._32 + ViewProjectionMatrix._42;
		z = Box[0]*ViewProjectionMatrix._13 + Box[3]*ViewProjectionMatrix._23 + Box[5]* ViewProjectionMatrix._33 + ViewProjectionMatrix._43;
		w = Box[0]*ViewProjectionMatrix._14 + Box[3]*ViewProjectionMatrix._24 + Box[5]* ViewProjectionMatrix._34 + ViewProjectionMatrix._44;
    	if( w < g_EPSILON && w > -g_EPSILON)
		{
			return(2);
		}
		if( ((x/=w) >= -1 && x <= 1) 
			&&((y/=w) >= -1 && y <= 1) 
			&&((z/=w) >= 0 && z <= 1) 
			)
		{
			count++;
			if(count < 6)
			{
				return(2);
			}
		}

		x = Box[1]*ViewProjectionMatrix._11 + Box[2]*ViewProjectionMatrix._21 + Box[5]* ViewProjectionMatrix._31 + ViewProjectionMatrix._41;
		y = Box[1]*ViewProjectionMatrix._12 + Box[2]*ViewProjectionMatrix._22 + Box[5]* ViewProjectionMatrix._32 + ViewProjectionMatrix._42;
		z = Box[1]*ViewProjectionMatrix._13 + Box[2]*ViewProjectionMatrix._23 + Box[5]* ViewProjectionMatrix._33 + ViewProjectionMatrix._43;
		w = Box[1]*ViewProjectionMatrix._14 + Box[2]*ViewProjectionMatrix._24 + Box[5]* ViewProjectionMatrix._34 + ViewProjectionMatrix._44;
    	if( w < g_EPSILON && w > -g_EPSILON)
		{
			return(2);
		}
		if( ((x/=w) >= -1 && x <= 1) 
			&&((y/=w) >= -1 && y <= 1) 
			&&((z/=w) >= 0 && z <= 1) 
			)
		{
			count++;
			if(count < 7)
			{
				return(2);
			}
		}

		x = Box[1]*ViewProjectionMatrix._11 + Box[3]*ViewProjectionMatrix._21 + Box[5]* ViewProjectionMatrix._31 + ViewProjectionMatrix._41;
		y = Box[1]*ViewProjectionMatrix._12 + Box[3]*ViewProjectionMatrix._22 + Box[5]* ViewProjectionMatrix._32 + ViewProjectionMatrix._42;
		z = Box[1]*ViewProjectionMatrix._13 + Box[3]*ViewProjectionMatrix._23 + Box[5]* ViewProjectionMatrix._33 + ViewProjectionMatrix._43;
		w = Box[1]*ViewProjectionMatrix._14 + Box[3]*ViewProjectionMatrix._24 + Box[5]* ViewProjectionMatrix._34 + ViewProjectionMatrix._44;
    	if( w < g_EPSILON && w > -g_EPSILON)
		{
			return(2);
		}
		if( ((x/=w) >= -1 && x <= 1) 
			&&((y/=w) >= -1 && y <= 1) 
			&&((z/=w) >= 0 && z <= 1) 
			)
		{
			count++;
			if(count < 8)
			{
				return(2);
			}
		}
		
		//TODO: if count = 0  we could still have intersction : box pts not in frust but box intersects frust!!!
		//try simpl check by checking if any frust point is inside box!
		//if 
		if(count==0)
		{
			if(FrustumPointInsideBox() || FrustumInterBox())
			{
				return(2);
			}
			
			return(0);
		}
		
		return(9 - count);
	}

	
	inline BOOL FrustumInterBox()
	{
		BOOL check = TRUE;
		if(!check)
		{
			return(FALSE);
		}
		//TODO use already calced hom. ccords put in static array 
		//and use these to test intersection.s
		//then were done!!
		int i,j;
		
		
		for(i = 0, j = 4; i < 4; i++,j++)
		{
			if( TIInlineMathSegmentIntersectsSegmentPlaneXY(FrustumPoints[i],FrustumPoints[j],Box[4],Box[0],Box[1],Box[2],Box[3])
				||TIInlineMathSegmentIntersectsSegmentPlaneXY(FrustumPoints[i],FrustumPoints[j],Box[5],Box[0],Box[1],Box[2],Box[3])
				||TIInlineMathSegmentIntersectsSegmentPlaneXZ(FrustumPoints[i],FrustumPoints[j],Box[2],Box[0],Box[1],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneXZ(FrustumPoints[i],FrustumPoints[j],Box[3],Box[0],Box[1],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneYZ(FrustumPoints[i],FrustumPoints[j],Box[0],Box[2],Box[3],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneYZ(FrustumPoints[i],FrustumPoints[j],Box[1],Box[2],Box[3],Box[4],Box[5])
				)
			return(TRUE);
		}
		
		for(i = 0, j = 1; i < 4; i++,j++)
		{
			j %= 4;
			if( TIInlineMathSegmentIntersectsSegmentPlaneXY(FrustumPoints[i],FrustumPoints[j],Box[4],Box[0],Box[1],Box[2],Box[3])
				||TIInlineMathSegmentIntersectsSegmentPlaneXY(FrustumPoints[i],FrustumPoints[j],Box[5],Box[0],Box[1],Box[2],Box[3])
				||TIInlineMathSegmentIntersectsSegmentPlaneXZ(FrustumPoints[i],FrustumPoints[j],Box[2],Box[0],Box[1],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneXZ(FrustumPoints[i],FrustumPoints[j],Box[3],Box[0],Box[1],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneYZ(FrustumPoints[i],FrustumPoints[j],Box[0],Box[2],Box[3],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneYZ(FrustumPoints[i],FrustumPoints[j],Box[1],Box[2],Box[3],Box[4],Box[5])
				)
			return(TRUE);
		
		}	

		for(i = 4 , j = 5; i < 7; i++, j++)
		{
			if( TIInlineMathSegmentIntersectsSegmentPlaneXY(FrustumPoints[i],FrustumPoints[j],Box[4],Box[0],Box[1],Box[2],Box[3])
				||TIInlineMathSegmentIntersectsSegmentPlaneXY(FrustumPoints[i],FrustumPoints[j],Box[5],Box[0],Box[1],Box[2],Box[3])
				||TIInlineMathSegmentIntersectsSegmentPlaneXZ(FrustumPoints[i],FrustumPoints[j],Box[2],Box[0],Box[1],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneXZ(FrustumPoints[i],FrustumPoints[j],Box[3],Box[0],Box[1],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneYZ(FrustumPoints[i],FrustumPoints[j],Box[0],Box[2],Box[3],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneYZ(FrustumPoints[i],FrustumPoints[j],Box[1],Box[2],Box[3],Box[4],Box[5])
				)
			return(TRUE);
		
		}
		
		j = 4;
		if( TIInlineMathSegmentIntersectsSegmentPlaneXY(FrustumPoints[i],FrustumPoints[j],Box[4],Box[0],Box[1],Box[2],Box[3])
				||TIInlineMathSegmentIntersectsSegmentPlaneXY(FrustumPoints[i],FrustumPoints[j],Box[5],Box[0],Box[1],Box[2],Box[3])
				||TIInlineMathSegmentIntersectsSegmentPlaneXZ(FrustumPoints[i],FrustumPoints[j],Box[2],Box[0],Box[1],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneXZ(FrustumPoints[i],FrustumPoints[j],Box[3],Box[0],Box[1],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneYZ(FrustumPoints[i],FrustumPoints[j],Box[0],Box[2],Box[3],Box[4],Box[5])
				||TIInlineMathSegmentIntersectsSegmentPlaneYZ(FrustumPoints[i],FrustumPoints[j],Box[1],Box[2],Box[3],Box[4],Box[5])
				)
			return(TRUE);
			
		
		return(FALSE);
	}
	
	inline BOOL FrustumPointInsideBox()
	{
		return(	TIMathInlinePtInBox(FrustumPoints[0],Box)
				||TIMathInlinePtInBox(FrustumPoints[1],Box)
				||TIMathInlinePtInBox(FrustumPoints[2],Box)
				||TIMathInlinePtInBox(FrustumPoints[3],Box)
				||TIMathInlinePtInBox(FrustumPoints[4],Box)
				||TIMathInlinePtInBox(FrustumPoints[5],Box)
				||TIMathInlinePtInBox(FrustumPoints[6],Box)
				||TIMathInlinePtInBox(FrustumPoints[7],Box)
				);

	}

	
};


#endif