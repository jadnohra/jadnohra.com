#ifndef _ASEGEOMOBJECT_H__
#define _ASEGEOMOBJECT_H__

#include "..\..\General\Collection\CFastPrimitiveArrayKeepTogether.h"
#include "ASEMATERIAL.h"


class ASEGeomObject
{

public:
	CFastPrimitiveArrayKeepTogether<D3DVERTEX>	Vertices;
	ASEMATERIAL									Material;
	
	ASEGeomObject();

	
	BOOL AddMesh(D3DVERTEX* pFaceVertices,long faceCount);
	BOOL SetMaterial(ASEMATERIAL& material); 

};

#endif