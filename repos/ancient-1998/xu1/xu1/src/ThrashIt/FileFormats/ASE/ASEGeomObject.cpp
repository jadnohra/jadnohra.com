#include "ASEGeomObject.h"
#include "..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"


ASEGeomObject::ASEGeomObject()
{
	Vertices.Create(18);
}

BOOL ASEGeomObject::AddMesh(D3DVERTEX* pFaceVertices,long faceCount)
{
	for(int i = 0; i < faceCount; i++)
	{
		
		D3DVERTEX* lpNew1 = Vertices.New();
		D3DVERTEX* lpNew2 = Vertices.New();
		D3DVERTEX* lpNew3 = Vertices.New();
		
		if(lpNew1 && lpNew2 && lpNew3)
		{
			*lpNew1 = pFaceVertices[(i * 3) + 0];
			*lpNew2 = pFaceVertices[(i * 3) + 1];
			*lpNew3 = pFaceVertices[(i * 3) + 2];
			
		}
		else
		{
			if(lpNew1)
			{
				Vertices.Delete(lpNew1);
			}
			if(lpNew2)
			{
				Vertices.Delete(lpNew2);
			}
			if(lpNew3)
			{
				Vertices.Delete(lpNew3);
			}
			
			return(FALSE);
		}
	}

	return(TRUE);
}


BOOL ASEGeomObject::SetMaterial(ASEMATERIAL& material)
{
	Material.Set(material);
	return(TRUE);
}