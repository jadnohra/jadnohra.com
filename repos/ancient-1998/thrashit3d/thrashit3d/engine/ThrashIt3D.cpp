#include "ThrashIt3D.h"
#include "math.h"
#include "mmsystem.h"

#include "..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"
#include "..\..\General\Collection\CFastPtrArrayKeepTogether.cpp"



DWORD ThrashIt3D::RENDERMODE_WIREFRAME		= THRASHIT3D_RENDER_WIREFRAME;
DWORD ThrashIt3D::RENDERMODE_FLAT			= THRASHIT3D_RENDER_FLAT;
DWORD ThrashIt3D::RENDERMODE_GOURAUD		= THRASHIT3D_RENDER_GOURAUD;
DWORD ThrashIt3D::RENDERMODE_NONE			= THRASHIT3D_RENDER_NONE;
DWORD ThrashIt3D::RENDERMODE_WIREFRAME_COLOR= 0;



/*
VOID ThrashIt3D::_Translate(D3Vector<float>& point,D3Vector<float>& trans,D3Vector<float>& result)
{
	result.x=point.x+trans.x;
	result.y=point.y+trans.y;
	result.z=point.z+trans.z;
	
}


VOID ThrashIt3D::_Rotate(D3Vector<float>& point,D3Vector<float>& trans,D3Vector<float>& result)
{
	D3Vector<float> temp=point;
	// X axis rotation 
	result.y = (FLOAT)((temp.y*cos(trans.x)) - (temp.z*sin(trans.x)));  
	result.z = (FLOAT)((temp.z*cos(trans.x)) + (temp.y*sin(trans.x)));
	
	//(Copy NewY and NewZ into OldY and OldZ)
	temp.y=result.y;
	temp.z=result.z;
	//Y axis rotation 
	
		  result.z = (FLOAT)((temp.z*cos(trans.y)) - (temp.x*sin(trans.y)));  
		  result.x = (FLOAT)((temp.x*cos(trans.y)) + (temp.z*sin(trans.y)));
		  
		  //(Copy NewZ and NewX into OldZ and OldX)
		  temp.x=result.x;
		  temp.z=result.z;
		  
		  // Z axis rotation 
		  result.x = (FLOAT)((temp.x*cos(trans.z)) - (temp.y*sin(trans.z))); 
		  result.y = (FLOAT)((temp.y*cos(trans.z)) + (temp.x*sin(trans.z)));
		  
}



VOID ThrashIt3D::_Project(D3Vector<float>& point,D2Vector<float>& result)
{
	if(point.z)
	{
		result.x = (_Focal * point.x / point.z + _ScreenCenter.x);
		result.y = (_Focal * -point.y / point.z + _ScreenCenter.y);
	}
	else
	{
		result.x = (_Focal * point.x + _ScreenCenter.x);
		result.y = (_Focal * -point.y + _ScreenCenter.y);
	}
	
	
}


VOID ThrashIt3D::_Project(Viewport* lpView, D3Vector<float>& point,D2Vector<float>& result)
{
	if(point.z)
	{
		result.x = (lpView->Focal * point.x / point.z + lpView->ScreenCenter.x);
		result.y = (lpView->Focal * -point.y / point.z + lpView->ScreenCenter.y);
	}
	else
	{
		result.x = (lpView->Focal * point.x + lpView->ScreenCenter.x);
		result.y = (lpView->Focal * -point.y + lpView->ScreenCenter.y);
	}
	
	
}


VOID ThrashIt3D::Translate(D3Vector<float>& point,D3Vector<float>& trans,D3Vector<float>& result)
{
	_Translate(point,trans,result);
}

VOID ThrashIt3D::Rotate(D3Vector<float>& point,D3Vector<float>& trans,D3Vector<float>& result)
{
	_Rotate(point,trans,result);
}


VOID ThrashIt3D::Project(Viewport* lpView,D3Vector<float>& point,D2Vector<float> &result)
{
	_Project(lpView,point,result);
}


VOID ThrashIt3D::_RenderWireframe(Viewport* lpView)
{
	
	_Focal = lpView->Focal;
	_ScreenCenter = lpView->ScreenCenter;
	_lpDestGraphic = lpView->lpDestGraphic;
	
	Vertex Position	= lpView->Viewer.Position;
	D3Vector<float> Rotation = lpView->Viewer.Rotation;
	
	Triangle* lpTriangle = Triangles.StartForwardRun();
	
	
	while(lpTriangle)
	{
		
		_Rotate(lpTriangle->v1.World,lpTriangle->Rotation,lpTriangle->v1.World);
		_Rotate(lpTriangle->v2.World,lpTriangle->Rotation,lpTriangle->v2.World);
		_Rotate(lpTriangle->v3.World,lpTriangle->Rotation,lpTriangle->v3.World);
		
		
		_Translate(lpTriangle->v1.World,Position.World,lpTriangle->v1.Aligned);
		_Translate(lpTriangle->v2.World,Position.World,lpTriangle->v2.Aligned);
		_Translate(lpTriangle->v3.World,Position.World,lpTriangle->v3.Aligned);
		
		
		_Rotate(lpTriangle->v1.Aligned,Rotation,lpTriangle->v1.Aligned);
		_Rotate(lpTriangle->v2.Aligned,Rotation,lpTriangle->v2.Aligned);
		_Rotate(lpTriangle->v3.Aligned,Rotation,lpTriangle->v3.Aligned);
		
		_Project(lpTriangle->v1.Aligned,lpTriangle->v1.Screen);
		_Project(lpTriangle->v2.Aligned,lpTriangle->v2.Screen);
		_Project(lpTriangle->v3.Aligned,lpTriangle->v3.Screen);
		
		_RasterizeWireframe(*lpTriangle);
		
		if((Triangles.RunIndex=Triangles.Array[Triangles.RunIndex]._nextIndex)==CFASTPRIMITIVEARRAYNULLINDEX)
		{
			break;
		}
		
		lpTriangle = (&Triangles.Array[Triangles.RunIndex]._data);
	}
}


VOID ThrashIt3D::_RenderFlat(Viewport* lpView)
{
	
	_Focal = lpView->Focal;
	_ScreenCenter = lpView->ScreenCenter;
	_lpDestGraphic = lpView->lpDestGraphic;
	
	Vertex Position	= lpView->Viewer.Position;
	D3Vector<float> Rotation = lpView->Viewer.Rotation;
	
	
	Triangle* lpTriangle = Triangles.StartForwardRun();
	
	
	while(lpTriangle)
	{
		
		_Rotate(lpTriangle->v1.World,lpTriangle->Rotation,lpTriangle->v1.World);
		_Rotate(lpTriangle->v2.World,lpTriangle->Rotation,lpTriangle->v2.World);
		_Rotate(lpTriangle->v3.World,lpTriangle->Rotation,lpTriangle->v3.World);
		
		
		_Translate(lpTriangle->v1.World,Position.World,lpTriangle->v1.Aligned);
		_Translate(lpTriangle->v2.World,Position.World,lpTriangle->v2.Aligned);
		_Translate(lpTriangle->v3.World,Position.World,lpTriangle->v3.Aligned);
		
		
		_Rotate(lpTriangle->v1.Aligned,Rotation,lpTriangle->v1.Aligned);
		_Rotate(lpTriangle->v2.Aligned,Rotation,lpTriangle->v2.Aligned);
		_Rotate(lpTriangle->v3.Aligned,Rotation,lpTriangle->v3.Aligned);
		
		_Project(lpTriangle->v1.Aligned,lpTriangle->v1.Screen);
		_Project(lpTriangle->v2.Aligned,lpTriangle->v2.Screen);
		_Project(lpTriangle->v3.Aligned,lpTriangle->v3.Screen);
		
		
		_RasterizeFlat(*lpTriangle);
		
		if((Triangles.RunIndex=Triangles.Array[Triangles.RunIndex]._nextIndex)==CFASTPRIMITIVEARRAYNULLINDEX)
		{
			break;
		}
		
		lpTriangle = (&Triangles.Array[Triangles.RunIndex]._data);
	}
}


VOID ThrashIt3D::_RenderGouraud(Viewport* lpView)
{
	
	_Focal = lpView->Focal;
	_ScreenCenter = lpView->ScreenCenter;
	_lpDestGraphic = lpView->lpDestGraphic;
	_ZBuffer = TIZBUFFER_GETBUFFER(lpView->zBuffer);
	_ZIndicator = TIZBUFFER_GETINDICATOR(lpView->zBuffer);
	
	Vertex Position	= lpView->Viewer.Position;
	D3Vector<float> Rotation = lpView->Viewer.Rotation;
	
	
	Triangle* lpTriangle = Triangles.StartForwardRun();
	
	if(lpTriangle)
	{
		TIZBUFFER_RESET(lpView->zBuffer);
	}
	
	while(lpTriangle)
	{
		
		_Rotate(lpTriangle->v1.World,lpTriangle->Rotation,lpTriangle->v1.World);
		_Rotate(lpTriangle->v2.World,lpTriangle->Rotation,lpTriangle->v2.World);
		_Rotate(lpTriangle->v3.World,lpTriangle->Rotation,lpTriangle->v3.World);
		
		
		_Translate(lpTriangle->v1.World,Position.World,lpTriangle->v1.Aligned);
		_Translate(lpTriangle->v2.World,Position.World,lpTriangle->v2.Aligned);
		_Translate(lpTriangle->v3.World,Position.World,lpTriangle->v3.Aligned);
		
		
		_Rotate(lpTriangle->v1.Aligned,Rotation,lpTriangle->v1.Aligned);
		_Rotate(lpTriangle->v2.Aligned,Rotation,lpTriangle->v2.Aligned);
		_Rotate(lpTriangle->v3.Aligned,Rotation,lpTriangle->v3.Aligned);
		
		_Project(lpTriangle->v1.Aligned,lpTriangle->v1.Screen);
		_Project(lpTriangle->v2.Aligned,lpTriangle->v2.Screen);
		_Project(lpTriangle->v3.Aligned,lpTriangle->v3.Screen);
		
		
		_RasterizeGouraud(*lpTriangle);
		
		if((Triangles.RunIndex=Triangles.Array[Triangles.RunIndex]._nextIndex)==CFASTPRIMITIVEARRAYNULLINDEX)
		{
			break;
		}
		
		lpTriangle = (&Triangles.Array[Triangles.RunIndex]._data);
	}
}



VOID ThrashIt3D::_RasterizeWireframe(Triangle& tr)
{
	Vertex *sortedY1,*sortedY2,*sortedY3;
	float interX;	
	float interZ;
	
	
	if(tr.v1.Screen.y<tr.v2.Screen.y)
	{
		if(tr.v1.Screen.y<tr.v3.Screen.y)
		{
			sortedY1=&tr.v1;
			
			if(tr.v2.Screen.y<tr.v3.Screen.y)
			{
				sortedY2=&tr.v2;
				sortedY3=&tr.v3;
				
			}
			else
			{
				sortedY2=&tr.v3;
				sortedY3=&tr.v2;
			}
			
		}
		else
		{
			sortedY1=&tr.v3;
			sortedY2=&tr.v1;
			sortedY3=&tr.v2;
			
		}
		
	}
	else
	{
		if(tr.v2.Screen.y<tr.v3.Screen.y)
		{
			sortedY1=&tr.v2;
			
			if(tr.v1.Screen.y<tr.v3.Screen.y)
			{
				sortedY2=&tr.v1;
				sortedY3=&tr.v3;
			}
			else
			{
				sortedY2=&tr.v3;
				sortedY3=&tr.v1;
			}
		}
		else
		{
			sortedY1=&tr.v3;
			sortedY2=&tr.v2;
			sortedY3=&tr.v1;
			
		}
		
		
	}
	
	
	if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY2->Screen.y))
	{
		if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY3->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY3->Screen.y)<INTCOORD(sortedY1->Screen.y))
			_RasterizeWireframeUp(sortedY3->Screen.x,sortedY3->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,tr.color.color);
		else
			_RasterizeWireframeDown(sortedY3->Screen.x,sortedY3->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,tr.color.color);
		return;
	}		
	
	
	if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY3->Screen.y))
	{
		if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY2->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY2->Screen.y)<INTCOORD(sortedY1->Screen.y))
			_RasterizeWireframeUp(sortedY2->Screen.x,sortedY2->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color.color);
		else
			_RasterizeWireframeDown(sortedY2->Screen.x,sortedY2->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color.color);
		return;
	}		
	
	
	
	if(INTCOORD(sortedY2->Screen.y)==INTCOORD(sortedY3->Screen.y))
	{
		if(INTCOORD(sortedY2->Screen.y)==INTCOORD(sortedY1->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY1->Screen.y)<INTCOORD(sortedY2->Screen.y))
			_RasterizeWireframeUp(sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color.color);
		else
			_RasterizeWireframeDown(sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color.color);
		return;
	}		
	
	
	interX = _LineIntersectWithHorizNoCheck(sortedY1->Screen,sortedY3->Screen,sortedY2->Screen.y);
	_RasterizeWireframeUp(sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,interX,sortedY2->Screen.y,tr.color.color);
	_RasterizeWireframeDown(sortedY3->Screen.x,sortedY3->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,interX,sortedY2->Screen.y,tr.color.color);
	
	
	
}



VOID ThrashIt3D::_RasterizeFlat(Triangle& tr)
{
	Vertex *sortedY1,*sortedY2,*sortedY3;
	float interX;	
	float interZ;
	
	
	if(tr.v1.Screen.y<tr.v2.Screen.y)
	{
		if(tr.v1.Screen.y<tr.v3.Screen.y)
		{
			sortedY1=&tr.v1;
			
			if(tr.v2.Screen.y<tr.v3.Screen.y)
			{
				sortedY2=&tr.v2;
				sortedY3=&tr.v3;
				
			}
			else
			{
				sortedY2=&tr.v3;
				sortedY3=&tr.v2;
			}
			
		}
		else
		{
			sortedY1=&tr.v3;
			sortedY2=&tr.v1;
			sortedY3=&tr.v2;
			
		}
		
	}
	else
	{
		if(tr.v2.Screen.y<tr.v3.Screen.y)
		{
			sortedY1=&tr.v2;
			
			if(tr.v1.Screen.y<tr.v3.Screen.y)
			{
				sortedY2=&tr.v1;
				sortedY3=&tr.v3;
			}
			else
			{
				sortedY2=&tr.v3;
				sortedY3=&tr.v1;
			}
		}
		else
		{
			sortedY1=&tr.v3;
			sortedY2=&tr.v2;
			sortedY3=&tr.v1;
			
		}
		
		
	}
	
	
	if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY2->Screen.y))
	{
		if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY3->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY3->Screen.y)<INTCOORD(sortedY1->Screen.y))
			_RasterizeFlatUp(sortedY3->Screen.x,sortedY3->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,tr.color.color);
		else
			_RasterizeFlatDown(sortedY3->Screen.x,sortedY3->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,tr.color.color);
		return;
	}		
	
	
	if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY3->Screen.y))
	{
		if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY2->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY2->Screen.y)<INTCOORD(sortedY1->Screen.y))
			_RasterizeFlatUp(sortedY2->Screen.x,sortedY2->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color.color);
		else
			_RasterizeFlatDown(sortedY2->Screen.x,sortedY2->Screen.y,sortedY1->Screen.x,sortedY1->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color.color);
		return;
	}		
	
	
	
	if(INTCOORD(sortedY2->Screen.y)==INTCOORD(sortedY3->Screen.y))
	{
		if(INTCOORD(sortedY2->Screen.y)==INTCOORD(sortedY1->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY1->Screen.y)<INTCOORD(sortedY2->Screen.y))
			_RasterizeFlatUp(sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color.color);
		else
			_RasterizeFlatDown(sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,sortedY3->Screen.x,sortedY3->Screen.y,tr.color.color);
		return;
	}		
	
	
	interX = _LineIntersectWithHorizNoCheck(sortedY1->Screen,sortedY3->Screen,sortedY2->Screen.y);
	_RasterizeFlatUp(sortedY1->Screen.x,sortedY1->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,interX,sortedY2->Screen.y,tr.color.color);
	_RasterizeFlatDown(sortedY3->Screen.x,sortedY3->Screen.y,sortedY2->Screen.x,sortedY2->Screen.y,interX,sortedY2->Screen.y,tr.color.color);
	
	
	
}


VOID ThrashIt3D::_RasterizeGouraud(Triangle& tr)
{
	Vertex *sortedY1,*sortedY2,*sortedY3;
	float interX;	
	float interZ;
	CDXColor interColor;
	
	
	if(tr.v1.Screen.y<tr.v2.Screen.y)
	{
		if(tr.v1.Screen.y<tr.v3.Screen.y)
		{
			sortedY1=&tr.v1;
			
			if(tr.v2.Screen.y<tr.v3.Screen.y)
			{
				sortedY2=&tr.v2;
				sortedY3=&tr.v3;
				
			}
			else
			{
				sortedY2=&tr.v3;
				sortedY3=&tr.v2;
			}
			
		}
		else
		{
			sortedY1=&tr.v3;
			sortedY2=&tr.v1;
			sortedY3=&tr.v2;
			
		}
		
	}
	else
	{
		if(tr.v2.Screen.y<tr.v3.Screen.y)
		{
			sortedY1=&tr.v2;
			
			if(tr.v1.Screen.y<tr.v3.Screen.y)
			{
				sortedY2=&tr.v1;
				sortedY3=&tr.v3;
			}
			else
			{
				sortedY2=&tr.v3;
				sortedY3=&tr.v1;
			}
		}
		else
		{
			sortedY1=&tr.v3;
			sortedY2=&tr.v2;
			sortedY3=&tr.v1;
			
		}
		
		
	}
	
	
	if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY2->Screen.y))
	{
		if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY3->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY3->Screen.y)<INTCOORD(sortedY1->Screen.y))
			_RasterizeGouraudUp(sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z,sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z);
		else
			_RasterizeGouraudDown(sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z,sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z);
		return;
	}		
	
	
	if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY3->Screen.y))
	{
		if(INTCOORD(sortedY1->Screen.y)==INTCOORD(sortedY2->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY2->Screen.y)<INTCOORD(sortedY1->Screen.y))
			_RasterizeGouraudUp(sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z);
		else
			_RasterizeGouraudDown(sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z);
		return;
	}		
	
	
	
	if(INTCOORD(sortedY2->Screen.y)==INTCOORD(sortedY3->Screen.y))
	{
		if(INTCOORD(sortedY2->Screen.y)==INTCOORD(sortedY1->Screen.y))
			return;	//reduced to line
		
		if(INTCOORD(sortedY1->Screen.y)<INTCOORD(sortedY2->Screen.y))
			_RasterizeGouraudUp(sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z);
		else
			_RasterizeGouraudDown(sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z);
		return;
	}		
	
	
	
	interX = _LineIntersectWithHorizNoCheck(sortedY1->Screen,sortedY3->Screen,sortedY2->Screen.y);
	
	
	interColor.r = _LineIntersectWithHorizNoCheck(sortedY1->color.r,sortedY1->Screen.y,sortedY3->color.r,sortedY3->Screen.y,sortedY2->Screen.y);
	interColor.g = _LineIntersectWithHorizNoCheck(sortedY1->color.g,sortedY1->Screen.y,sortedY3->color.g,sortedY3->Screen.y,sortedY2->Screen.y);
	interColor.b = _LineIntersectWithHorizNoCheck(sortedY1->color.b,sortedY1->Screen.y,sortedY3->color.b,sortedY3->Screen.y,sortedY2->Screen.y);
	interColor.color = interColor.Color16Bit(interColor.r,interColor.g,interColor.b);
	
	
	interZ = _LineIntersectWithHorizNoCheck(sortedY1->World.z,sortedY1->Screen.y,sortedY3->World.z,sortedY3->Screen.y,sortedY2->Screen.y);
	
	
	_RasterizeGouraudUp(sortedY1->Screen.x,sortedY1->Screen.y,sortedY1->color,sortedY1->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,interX,sortedY2->Screen.y,interColor,interZ);
	_RasterizeGouraudDown(sortedY3->Screen.x,sortedY3->Screen.y,sortedY3->color,sortedY3->World.z,sortedY2->Screen.x,sortedY2->Screen.y,sortedY2->color,sortedY2->World.z,interX,sortedY2->Screen.y,interColor,interZ);
	
	
	
}




VOID ThrashIt3D::_RasterizeWireframeUp(float XUp,float YUp,float XDown1,float YDown1,float XDown2,float YDown2,DWORD color)
{
	
	iVector from,to;
	
	from.x = XDown1;
	from.y = YDown1;
	to.x = XDown2;
	//_lpDestGraphic->ClippedHorizLine(from,to.x,CDXDraw::Color16Bit(255,255,255));
	
	to.x = XUp;
	to.y = YUp;
	_lpDestGraphic->ClippedLine(from,to,ThrashIt3D::RENDERMODE_WIREFRAME_COLOR);
	
	from.x = XDown2;
	from.y = YDown2;
	_lpDestGraphic->ClippedLine(from,to,ThrashIt3D::RENDERMODE_WIREFRAME_COLOR);	
	
}



VOID ThrashIt3D::_RasterizeFlatUp(float XUp,float YUp,float XDown1,float YDown1,float XDown2,float YDown2,DWORD color)
{
	
	//sort XDown1 XDown2
	if(XDown1 > XDown2)
	{
		float temp;
		
		temp = XDown2;
		XDown2 = XDown1;
		XDown1 = temp;
		
		temp = YDown2;
		YDown2 = YDown1;
		YDown1 = temp;
	}
	
	
	float aLeft = SLOPE(XUp,YUp,XDown1,YDown1);
	float aRight = SLOPE(XUp,YUp,XDown2,YDown2);	
	float bLeft = B(XUp,YUp,aLeft);
	float bRight = B(XUp,YUp,aRight);	
	int ScanY = INTCOORD(YUp);
	int EdgeStartX;
	int EdgeEndX;
	int X;
	int ScanYEnd = INTCOORD(YDown1);
	
	
	ScanY++; //dont draw top and right!!!
	
	while(ScanY <= ScanYEnd)
	{
		if(ScanY>YDown1) 
		{
			EdgeStartX = INTCOORD(X(aLeft,bLeft,YDown1));
			EdgeEndX = INTCOORD(X(aRight,bRight,YDown1)) - 1; //dont draw top and right!!!
		} 
		else
		{
			EdgeStartX = INTCOORD(X(aLeft,bLeft,ScanY));
			EdgeEndX = INTCOORD(X(aRight,bRight,ScanY)) - 1; //dont draw top and right!!!
		}
		X = EdgeStartX;
		while(X<=EdgeEndX) 
		{
			_lpDestGraphic->ClippedPoint(X,ScanY,color);
			X++;
		}
		
		ScanY++;
		
	}
	
}





VOID ThrashIt3D::_RasterizeGouraudUp(float XUp,float YUp,CDXColor colorUp,float ZUp,float XDown1,float YDown1,CDXColor colorDown1,float ZDown1,float XDown2,float YDown2,CDXColor colorDown2,float ZDown2)
{
	//sort XDown1 XDown2
	if(XDown1 > XDown2)
	{
		float temp;
		CDXColor tempColor;
		
		temp = XDown2;
		XDown2 = XDown1;
		XDown1 = temp;
		
		temp = YDown2;
		YDown2 = YDown1;
		YDown1 = temp;
		
		tempColor = colorDown2;
		colorDown2 = colorDown1;
		colorDown1 = tempColor;
	}
	
	
	float aLeft = SLOPE(XUp,YUp,XDown1,YDown1);
	float aRight = SLOPE(XUp,YUp,XDown2,YDown2);	
	float bLeft = B(XUp,YUp,aLeft);
	float bRight = B(XUp,YUp,aRight);	
	int ScanY = INTCOORD(YUp);
	int EdgeStartX;
	int EdgeEndX;
	int X;
	int ScanYEnd = INTCOORD(YDown1);
	
	CDXFloatColorComponents ScanColor;
	
	CDXFloatColorComponents lColor;
	lColor.Set(colorUp);
	lColor.rInc = (float)(colorDown1.r - colorUp.r) / (float)(ScanYEnd - ScanY);
	lColor.gInc = (float)(colorDown1.g - colorUp.g) / (float)(ScanYEnd - ScanY);
	lColor.bInc = (float)(colorDown1.b - colorUp.b) / (float)(ScanYEnd - ScanY);
	
	CDXFloatColorComponents rColor;
	rColor.Set(colorUp);
	rColor.rInc = (float)(colorDown2.r - colorUp.r) / (float)(ScanYEnd - ScanY);
	rColor.gInc = (float)(colorDown2.g - colorUp.g) / (float)(ScanYEnd - ScanY);
	rColor.bInc = (float)(colorDown2.b - colorUp.b) / (float)(ScanYEnd - ScanY);
	
	float ScanZ;
	float ScanZInc;
	float leftZInc  = (float)(ZDown1 - ZUp) / (float)(ScanYEnd - ScanY);
	float rightZInc = (float)(ZDown2 - ZUp) / (float)(ScanYEnd - ScanY);
	float leftZ = ZUp;
	float rightZ = ZUp;
	
	
	
	
	ScanY++; //dont draw top and right!!!
	
	while(ScanY <= ScanYEnd)
	{
		if(ScanY>YDown1) 
		{
			EdgeStartX = INTCOORD(X(aLeft,bLeft,YDown1));
			EdgeEndX = INTCOORD(X(aRight,bRight,YDown1)) - 1; //dont draw top and right!!!
			
		} 
		else
		{
			
			EdgeStartX = INTCOORD(X(aLeft,bLeft,ScanY));
			EdgeEndX = INTCOORD(X(aRight,bRight,ScanY)) - 1; //dont draw top and right!!!
		}
		
		
		lColor.r += lColor.rInc;
		lColor.g += lColor.gInc;
		lColor.b += lColor.bInc;
		rColor.r += rColor.rInc;
		rColor.g += rColor.gInc;
		rColor.b += rColor.bInc;
		ScanColor = lColor;
		ScanColor.rInc = ((float) (rColor.r - lColor.r)) / ((float)(EdgeEndX - EdgeStartX));
		ScanColor.gInc = ((float) (rColor.g - lColor.g)) / ((float)(EdgeEndX - EdgeStartX));
		ScanColor.bInc = ((float) (rColor.b - lColor.b)) / ((float)(EdgeEndX - EdgeStartX));
		
		leftZ += leftZInc;
		rightZ += rightZInc;
		ScanZ = leftZ;
		ScanZInc = ((float) (rightZ - leftZ)) / ((float)(EdgeEndX - EdgeStartX));
		
		
		X = EdgeStartX;
		while(X<=EdgeEndX) 
		{
			
			//if(ScanColor.r > 255 || ScanColor.r < 0 ) 
			//{
			//	ScanColor.r = ScanColor.r;
			//}
			//if(ScanColor.g > 255 || ScanColor.g < 0) 
			//{
			//	ScanColor.g = ScanColor.g;
			//}
			//if(ScanColor.b > 255 || ScanColor.b < 0) 
			//{
			//	ScanColor.b = ScanColor.b;
			//}
			
			
			if(TIZBUFFER_SHOULDDRAWPIXEL(ScanZ,_ZBuffer,_ZIndicator,X,ScanY))
			{
				
				_lpDestGraphic->ClippedPoint(X,ScanY,ScanColor.Color16Bit());
				TIZBUFFER_SETZVALUE(ScanZ,_ZBuffer,_ZIndicator,X,ScanY);
			}
			
			
			X++;
			ScanColor.r += ScanColor.rInc;
			ScanColor.g += ScanColor.gInc;
			ScanColor.b += ScanColor.bInc;
			ScanZ +=ScanZInc;
		}
		
		ScanY++;
		
	}
}

VOID ThrashIt3D::_RasterizeWireframeDown(float XDown,float YDown,float XUp1,float YUp1,float XUp2,float YUp2,DWORD color)
{
	
	iVector from,to;
	
	from.x = XUp1;
	from.y = YUp1;
	to.x = XUp2;
	//_lpDestGraphic->ClippedHorizLine(from,to.x,CDXDraw::Color16Bit(180,180,180));
	
	to.x = XDown;
	to.y = YDown;
	_lpDestGraphic->ClippedLine(from,to,ThrashIt3D::RENDERMODE_WIREFRAME_COLOR);
	
	from.x = XUp2;
	from.y = YUp2;
	_lpDestGraphic->ClippedLine(from,to,ThrashIt3D::RENDERMODE_WIREFRAME_COLOR);
	
}




VOID ThrashIt3D::_RasterizeFlatDown(float XDown,float YDown,float XUp1,float YUp1,float XUp2,float YUp2,DWORD color)
{
	
	//sort XUp1 XUp2
	if(XUp1 > XUp2)
	{
		float temp;
		
		temp = XUp2;
		XUp2 = XUp1;
		XUp1 = temp;
		
		temp = YUp2;
		YUp2 = YUp1;
		YUp1 = temp;
	}
	
	
	float aLeft = SLOPE(XUp1,YUp1,XDown,YDown);
	float aRight = SLOPE(XUp2,YUp2,XDown,YDown);	
	float bLeft = B(XUp1,YUp1,aLeft);
	float bRight = B(XUp2,YUp2,aRight);	
	int ScanY = INTCOORD(YUp1);
	int EdgeStartX;
	int EdgeEndX;
	int X;
	int ScanYEnd = INTCOORD(YDown);
	
	
	//
	ScanY++; //dont draw top and right!!!
	
	while(ScanY <= ScanYEnd)
	{
		if(ScanY>YDown) 
		{
			EdgeStartX = INTCOORD(X(aLeft,bLeft,YDown));
			EdgeEndX = INTCOORD(X(aRight,bRight,YDown)) - 1; //dont draw top and right!!!
		} 
		else
		{
			EdgeStartX = INTCOORD(X(aLeft,bLeft,ScanY));
			EdgeEndX = INTCOORD(X(aRight,bRight,ScanY)) - 1; //dont draw top and right!!!
		}
		X = EdgeStartX;
		while(X<=EdgeEndX) 
		{
			_lpDestGraphic->ClippedPoint(X,ScanY,color);
			X++;
		}
		
		ScanY++;
		
	}
	
}



VOID ThrashIt3D::_RasterizeGouraudDown(float XDown,float YDown,CDXColor colorDown,float ZDown,float XUp1,float YUp1,CDXColor colorUp1,float ZUp1,float XUp2,float YUp2,CDXColor colorUp2,float ZUp2)
{
	
	
	//sort XUp1 XUp2
	if(XUp1 > XUp2)
	{
		float temp;
		CDXColor tempColor;
		
		temp = XUp2;
		XUp2 = XUp1;
		XUp1 = temp;
		
		temp = YUp2;
		YUp2 = YUp1;
		YUp1 = temp;
		
		tempColor = colorUp2;
		colorUp2 = colorUp1;
		colorUp1 = tempColor;
	}
	
	
	float aLeft = SLOPE(XUp1,YUp1,XDown,YDown);
	float aRight = SLOPE(XUp2,YUp2,XDown,YDown);	
	float bLeft = B(XUp1,YUp1,aLeft);
	float bRight = B(XUp2,YUp2,aRight);	
	int ScanY = INTCOORD(YUp1);
	int EdgeStartX;
	int EdgeEndX;
	int X;
	int ScanYEnd = INTCOORD(YDown);
	
	CDXFloatColorComponents ScanColor;
	
	CDXFloatColorComponents lColor;
	lColor.Set(colorUp1);
	lColor.rInc = (float)(colorDown.r - colorUp1.r) / (float)(ScanYEnd - ScanY);
	lColor.gInc = (float)(colorDown.g - colorUp1.g) / (float)(ScanYEnd - ScanY);
	lColor.bInc = (float)(colorDown.b - colorUp1.b) / (float)(ScanYEnd - ScanY);
	
	CDXFloatColorComponents rColor;
	rColor.Set(colorUp2);
	rColor.rInc = (float)(colorDown.r - colorUp2.r) / (float)(ScanYEnd - ScanY);
	rColor.gInc = (float)(colorDown.g - colorUp2.g) / (float)(ScanYEnd - ScanY);
	rColor.bInc = (float)(colorDown.b - colorUp2.b) / (float)(ScanYEnd - ScanY);
	
	
	float ScanZ;
	float ScanZInc;
	float leftZInc  = (float)(ZDown - ZUp1) / (float)(ScanYEnd - ScanY);
	float rightZInc = (float)(ZDown - ZUp2) / (float)(ScanYEnd - ScanY);
	float leftZ = ZUp1;
	float rightZ = ZUp2;
	
	
	
	
	ScanY++; //dont draw top and right!!!
	
	while(ScanY <= ScanYEnd)
	{
		if(ScanY>YDown) 
		{
			EdgeStartX = INTCOORD(X(aLeft,bLeft,YDown));
			EdgeEndX = INTCOORD(X(aRight,bRight,YDown)) - 1; //dont draw top and right!!!
		} 
		else
		{
			EdgeStartX = INTCOORD(X(aLeft,bLeft,ScanY));
			EdgeEndX = INTCOORD(X(aRight,bRight,ScanY)) - 1; //dont draw top and right!!!
		}
		
		
		lColor.r += lColor.rInc;
		lColor.g += lColor.gInc;
		lColor.b += lColor.bInc;
		rColor.r += rColor.rInc;
		rColor.g += rColor.gInc;
		rColor.b += rColor.bInc;
		ScanColor = lColor;
		ScanColor.rInc = ((float) (rColor.r - lColor.r)) / ((float)(EdgeEndX - EdgeStartX));
		ScanColor.gInc = ((float) (rColor.g - lColor.g)) / ((float)(EdgeEndX - EdgeStartX));
		ScanColor.bInc = ((float) (rColor.b - lColor.b)) / ((float)(EdgeEndX - EdgeStartX));
		
		leftZ += leftZInc;
		rightZ += rightZInc;
		ScanZ = leftZ;
		ScanZInc = ((float) (rightZ - leftZ)) / ((float)(EdgeEndX - EdgeStartX));
		
		
		
		X = EdgeStartX;
		while(X<=EdgeEndX) 
		{
			
			if(ScanColor.r > 255 || ScanColor.r < 0) 
			{
				ScanColor.r = ScanColor.r;
			}
			if(ScanColor.g > 255 || ScanColor.g < 0) 
			{
				ScanColor.g = ScanColor.g;
			}
			if(ScanColor.b > 255 || ScanColor.b < 0) 
			{
				ScanColor.b = ScanColor.b;
			}
			
			if(TIZBUFFER_SHOULDDRAWPIXEL(ScanZ,_ZBuffer,_ZIndicator,X,ScanY))
			{
				
				_lpDestGraphic->ClippedPoint(X,ScanY,ScanColor.Color16Bit());
				TIZBUFFER_SETZVALUE(ScanZ,_ZBuffer,_ZIndicator,X,ScanY);
			}
			
			X++;
			ScanColor.r += ScanColor.rInc;
			ScanColor.g += ScanColor.gInc;
			ScanColor.b += ScanColor.bInc;
			ScanZ+=ScanZInc;
		}
		
		ScanY++;
		
	}
	
}

FLOAT ThrashIt3D::_LineIntersectWithHorizNoCheck(D2Vector<float> LinePt1,D2Vector<float> LinePt2,float HorizLineY)
{
	FLOAT a = SLOPE(LinePt1.x,LinePt1.y,LinePt2.x,LinePt2.y);
	return( ((HorizLineY - LinePt1.y) / SLOPE(LinePt1.x,LinePt1.y,LinePt2.x,LinePt2.y)) + LinePt1.x);	
}


FLOAT ThrashIt3D::_LineIntersectWithHorizNoCheck(float LinePt1X,float LinePt1Y,float LinePt2X,float LinePt2Y,float HorizLineY)
{
	FLOAT a = SLOPE(LinePt1X,LinePt1Y,LinePt2X,LinePt2Y);
	return( ((HorizLineY - LinePt1Y) / a) + LinePt1X);	
}

*/

TITriangle* ThrashIt3D::NewTriangle()
{
	return(Triangles.New());
}


BOOL ThrashIt3D::Connect(LONG TriangleCount)
{
	
	if(Connected)
	{
		return(TRUE);
	}
	
	#ifdef THRASHIT3D_LOGFILE
		Logfile.SetPath("ThrashIt3D.log");
		Logfile.OpenWriteOnly(TRUE);
		
		
		String temp;
		temp.Set("\r\n\r\n***ThrashIt3D Thrashed by the FiveMagics* \r\n");
		temp.Append("Date [");
		temp.AppendCurrDate();
		temp.Append(" ");
		temp.AppendCurrTime();
		temp.Append("] Version [");
		temp.Append(THRASHIT3D_VERMAJOR);
		temp.Append(THRASHIT3D_VERMINOR);
		temp.Append(THRASHIT3D_VERSTATE);
		temp.Append("] Build [");
		temp.Append(THRASHIT3D_VERBUILD);
		temp.Append("] Mode [");
		#ifdef _DEBUG
			temp.Append("Debug");
		#endif
		#ifndef _DEBUG
			temp.Append("Release");
		#endif
		if(CDXDraw::fullScreen) 
		{
			temp.Append("] [Full Screen");
		} 
		else 
		{
			temp.Append("] [Windowed");
		}
		
		temp.Append("] Sizeof Triangle [");
		temp.Append((LONG) sizeof(TITriangle));
		temp.Append("]\r\n");
		#ifdef BITINDICATOR
			temp.Append("Using ZBuffer BitFlag Array\r\n");
		#endif
		#ifndef BITINDICATOR
			temp.Append("Not Using ZBuffer BitFlag Array\r\n");
		#endif

		temp.Append("ThrashIt3D Gathering Power for ");
		temp.Append((LONG) TriangleCount);
		temp.Append(" Triangles...");
		
		Logfile.Log(temp.startAddress);
		
		lpLogger = Logfile.NewPerformanceLogger("Render");
	#endif
	
	
	if(!CDXDraw::Connect())
	{
		#ifdef THRASHIT3D_LOGFILE
			Logfile.Log("Connection Failure : CDXDraw::Connect()");
		#endif
		return(FALSE);
	}
	
	if(!TIGeometry::Connect())
	{
		#ifdef THRASHIT3D_LOGFILE
			Logfile.Log("Connection Failure : TIGeometry::Connect()");
		#endif
		return(FALSE);
	}
	#ifdef THRASHIT3D_LOGFILE
		TIGeometry::GetLogInfo(temp);
		Logfile.Log(temp);
	#endif
		
	
	
	ThrashIt3D::RENDERMODE_WIREFRAME_COLOR = CDXDraw::Color16Bit(255,255,255);
	
	if(!Triangles.Create((WORD)TriangleCount,TRUE))
	{
		#ifdef THRASHIT3D_LOGFILE
			Logfile.Log("Connection Failure : Triangles::Create()");
		#endif
		
		return(FALSE);
	}
	
	if(!TempTriangles.Create((WORD)TriangleCount / 2,TRUE))
	{
		#ifdef THRASHIT3D_LOGFILE
			Logfile.Log("Connection Failure : TempTriangles::Create()");
		#endif
		
		return(FALSE);
	}
	
	#ifdef THRASHIT3D_LOGFILE
		StageTransformation.lpLogfile = &Logfile;
		StageOcclusion.lpLogfile = &Logfile;
		StageProjection.lpLogfile = &Logfile;
		StageRasterizationWireframe.lpLogfile = &Logfile;
		StageRasterizationFlat.lpLogfile = &Logfile;
		StageRasterizationGouraud.lpLogfile = &Logfile;
	#endif
	
	if(!StageTransformation.Connect())
	{
		#ifdef THRASHIT3D_LOGFILE
			Logfile.Log("Conenction Failure : StageTransformation.Connect()\r\n");
		#endif
		return(FALSE);
	}
	if(!StageOcclusion.Connect())
	{
		#ifdef THRASHIT3D_LOGFILE
			Logfile.Log("Conenction Failure : StageTransformation.Connect()\r\n");
		#endif
		return(FALSE);
	}
	if(!StageProjection.Connect())
	{
		#ifdef THRASHIT3D_LOGFILE
			Logfile.Log("Conenction Failure : StageProjection.Connect()\r\n");
		#endif
		return(FALSE);
	}
	if(!StageRasterizationWireframe.Connect())
	{
		#ifdef THRASHIT3D_LOGFILE
			Logfile.Log("Conenction Failure : StageRasterizationWireframe.Connect()\r\n");
		#endif
		return(FALSE);
	}
	if(!StageRasterizationFlat.Connect())
	{
		#ifdef THRASHIT3D_LOGFILE
			Logfile.Log("Conenction Failure : StageRasterizationFlat.Connect()\r\n");
		#endif
		return(FALSE);
	}
	if(!StageRasterizationGouraud.Connect())
	{
		#ifdef THRASHIT3D_LOGFILE
			Logfile.Log("Conenction Failure : StageRasterizationGouraud.Connect()\r\n");
		#endif
		return(FALSE);
	}
	
	
	#ifdef THRASHIT3D_LOGFILE
		Logfile.Log("Thrahit3D Ready to Thrash...\r\n");
	#endif
	
	
	return(Connected = TRUE);
}


VOID ThrashIt3D::Disconnect()
{
	
	Triangles.Destroy();
	TempTriangles.Destroy();
	Connected = FALSE;
	#ifdef THRASHIT3D_LOGFILE
		Logfile.DeletePerformanceLogger(lpLogger);
	#endif	
	StageTransformation.DisConnect();
	StageOcclusion.DisConnect();
	StageProjection.DisConnect();
	StageRasterizationWireframe.DisConnect();
	StageRasterizationFlat.DisConnect();
	StageRasterizationGouraud.DisConnect();
	
	#ifdef THRASHIT3D_LOGFILE
		Logfile.Log("\r\n***Thrashit3D Rusts In Peace");
	#endif
	
}

VOID ThrashIt3D::LogPerformance()
{
	#ifdef THRASHIT3D_LOGFILE
		String temp;
		temp.Set("Logging Performance [");
		temp.Append((LONG) Triangles.UsedCount);
		temp.Append(" Triangles]...");
		Logfile.Log(temp);
		Logfile.LogPerformanceLoggers();
	#endif
	
}


ThrashIt3D::ThrashIt3D()
{
	Connected = FALSE;
}

ThrashIt3D::~ThrashIt3D()
{
	Disconnect();
}

VOID ThrashIt3D::Render(TIViewport *lpView)
{
	
	StageTransformation.ExecuteStage(lpView->Camera.Position,lpView->Camera.Rotation,Triangles);
	StageOcclusion.ExecuteStage(&lpView->Camera,Triangles,TempTriangles);
	StageProjection.ExecuteStage(lpView->Focal,lpView->ViewportCenter.x,lpView->ViewportCenter.y,Triangles,TempTriangles);
	
	switch(lpView->RenderMode)
	{
	case THRASHIT3D_RENDER_WIREFRAME:
		StageRasterizationWireframe.ExecuteStage(lpView->lpDestGraphic,&lpView->ZBuffer,Triangles,TempTriangles);
		break;
	case THRASHIT3D_RENDER_FLAT:
		StageRasterizationFlat.ExecuteStage(lpView->lpDestGraphic,&lpView->ZBuffer,Triangles,TempTriangles);
		break;
	case THRASHIT3D_RENDER_GOURAUD:
		StageRasterizationGouraud.ExecuteStage(lpView->lpDestGraphic,&lpView->ZBuffer,Triangles,TempTriangles);
		break;
	case THRASHIT3D_RENDER_NONE:
		break;
	}
	
}