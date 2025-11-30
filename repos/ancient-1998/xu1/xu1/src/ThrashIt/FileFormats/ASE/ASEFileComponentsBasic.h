#ifndef _ASEFILECOMPONENTSBASIC_H__
#define _ASEFILECOMPONENTSBASIC_H__

#include "..\..\MabenEngine\ITIGeometryBuilder3D.h"
#include "..\..\General\File\AsciiFileParser.h"
#include "..\..\General\Collection\CHashtable.h"
#include "ASEFileComponentsBasicDefines.h"
#include "ASEMATERIAL.h"
#include "d3d.h"
#include "ASEGeomObject.h"



class ASEFileComponent
{
public:

	ASEFileComponent*				lpParent;
	static ITIGeometryBuilder3D*	lpBuilder;	
	static LONG						LastErrorLine;
	static ASEGeomObject*			pCurrObject;

	ASEFileComponent() {lpParent = NULL;};
	virtual BOOL	Process(AsciiFileParser* lpFile) {return(FALSE);}; 
	virtual BOOL	GetKey(String& str) {return(FALSE);};
	virtual ~ASEFileComponent() {};

	static BOOL SkipComponent(AsciiFileParser* lpFile); //if we encountered a component key that we dont know how to process we should use this
	static ASEGeomObject* NewGeomObject();
	static VOID StaticCleanup();
};


class ASEFileComponentManager
{
protected:
	CHashTable<ASEFileComponent> Components;

public:
	ASEFileComponentManager();

	BOOL AddComponent(ASEFileComponent* lpParent,ASEFileComponent* lpComponent);
	BOOL RemoveComponent(String& Key);
	
	ASEFileComponent* FindComponentFor(String& string);
	VOID AutoDelete(BOOL Auto);

};


class ASEFileComponent_3DSMAX_EXPORT : public ASEFileComponent
{
public:
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};


class ASEFileComponent_COMMENT : public ASEFileComponent
{
public:
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};



class ASEFileComponent_SCENE : public ASEFileComponent
{
protected:
	
public:
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};



class ASEFileComponent_MATERIAL_LIST : public ASEFileComponent
{
protected:
	
public:

	ASEMATERIAL*	pMaterials;
	LONG			MaterialCount;

	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);

	ASEFileComponent_MATERIAL_LIST();
	~ASEFileComponent_MATERIAL_LIST();
	Free();
};



class ASEFileComponent_GEOMOBJECT__NODE_NAME : public ASEFileComponent
{
protected:
	
public:
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};

class ASEFileComponent_GEOMOBJECT__NODE_TM : public ASEFileComponent
{
public:
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};



class ASEFileComponent_GEOMOBJECT__MESH__TIMEVALUE : public ASEFileComponent
{
public:
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};

class ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMVERTEX : public ASEFileComponent 
{
public:

	long			VAL_NUMVERTEX;

	ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMVERTEX();

	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};

class ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMFACES : public ASEFileComponent
{
public:

	long			VAL_NUMFACES;

	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};

class ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMTFACES : public ASEFileComponent
{
public:

	long			VAL_NUMTFACES;

	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};
						
class ASEFileComponent_GEOMOBJECT__MESH__MESH_VERTEX_LIST : public ASEFileComponent
{
public:
	
	
	D3DVERTEX*		lpVertices;
	

	
	ASEFileComponent_GEOMOBJECT__MESH__MESH_VERTEX_LIST();
	virtual ~ASEFileComponent_GEOMOBJECT__MESH__MESH_VERTEX_LIST();
	

	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);

	
	VOID	Free();
	
};


class ASEFileComponent_GEOMOBJECT__MESH__MESH_TVERT_LIST : public ASEFileComponent
{
public:
	
	D3DVERTEX*		pTVertices;
	
	ASEFileComponent_GEOMOBJECT__MESH__MESH_TVERT_LIST();
	virtual ~ASEFileComponent_GEOMOBJECT__MESH__MESH_TVERT_LIST();
	

	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
	
	VOID	Free();
};


			
class ASEFileComponent_GEOMOBJECT__MESH__MESH_FACE_LIST : public ASEFileComponent
{
public:

	
	D3DVERTEX*		lpFaceVertices;
	

	
	ASEFileComponent_GEOMOBJECT__MESH__MESH_FACE_LIST();
	~ASEFileComponent_GEOMOBJECT__MESH__MESH_FACE_LIST();
	
	VOID Free();

	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};


class ASEFileComponent_GEOMOBJECT__MESH__MESH_TFACE_LIST : public ASEFileComponent
{
public:

	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};


			
class ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMTVERTEX : public ASEFileComponent
{
public:
	LONG VAL_NUMTVERTEX;

	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};

class ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMCVERTEX : public ASEFileComponent
{
public:
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};

class ASEFileComponent_GEOMOBJECT__MESH__MESH_NORMALS : public ASEFileComponent
{
public:
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};



class ASEFileComponent_GEOMOBJECT__MESH : public ASEFileComponent
{
protected:
	ASEFileComponentManager Components;
	
public:

	ASEFileComponent_GEOMOBJECT__MESH__TIMEVALUE		_TIMEVALUE;
	ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMVERTEX	_MESH_NUMVERTEX;
	ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMFACES	_MESH_NUMFACES;
	ASEFileComponent_GEOMOBJECT__MESH__MESH_VERTEX_LIST	_MESH_VERTEX_LIST;
	ASEFileComponent_GEOMOBJECT__MESH__MESH_FACE_LIST	_MESH_FACE_LIST;
	ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMTVERTEX	_MESH_NUMTVERTEX;
	ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMCVERTEX	_MESH_NUMCVERTEX;
	ASEFileComponent_GEOMOBJECT__MESH__MESH_NORMALS		_MESH_NORMALS;
	ASEFileComponent_GEOMOBJECT__MESH__MESH_TVERT_LIST	_MESH_TVERT_LIST;
	ASEFileComponent_GEOMOBJECT__MESH__MESH_TFACE_LIST	_MESH_TFACE_LIST;
	ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMTFACES	_MESH_NUMTFACES;


	ASEFileComponent_GEOMOBJECT__MESH();

	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};
	

class ASEFileComponent_GEOMOBJECT__PROP_MOTIONBLUR : public ASEFileComponent
{
protected:
	
public:
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};


class ASEFileComponent_GEOMOBJECT__PROP_CASTSHADOW : public ASEFileComponent
{
protected:
	
public:
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};


class ASEFileComponent_GEOMOBJECT__PROP_RECVSHADOW : public ASEFileComponent
{
protected:
	
public:
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};


class ASEFileComponent_GEOMOBJECT__MATERIAL_REF : public ASEFileComponent
{
protected:
	
public:

	ASEMATERIAL*	pMaterial;

	ASEFileComponent_GEOMOBJECT__MATERIAL_REF();

	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};

class ASEFileComponent_GEOMOBJECT : public ASEFileComponent
{
protected:
	ASEFileComponentManager Components;	

public:

	
	ASEFileComponent_GEOMOBJECT__NODE_NAME			_NODE_NAME;
	ASEFileComponent_GEOMOBJECT__NODE_TM			_NODE_TM;
	ASEFileComponent_GEOMOBJECT__MESH				_MESH;
	ASEFileComponent_GEOMOBJECT__PROP_MOTIONBLUR	_PROP_MOTIONBLUR;
	ASEFileComponent_GEOMOBJECT__PROP_CASTSHADOW	_PROP_CASTSHADOW;
	ASEFileComponent_GEOMOBJECT__PROP_RECVSHADOW	_PROP_RECVSHADOW;
	ASEFileComponent_GEOMOBJECT__MATERIAL_REF		_MATERIAL_REF;


	ASEFileComponent_GEOMOBJECT();
	
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};

#endif