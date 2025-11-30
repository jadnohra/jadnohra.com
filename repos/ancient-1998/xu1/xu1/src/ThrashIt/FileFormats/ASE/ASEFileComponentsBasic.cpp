#include "ASEFileComponentsBasic.h"
#include "..\..\General\Math\WLRandom.h"
#include "ASEFile.h"

#define MAP_X x
#define MAP_Y z
#define MAP_Z -y

#define MAP_NX nx
#define MAP_NY nz
#define MAP_NZ -ny

#define MAP_TUSIGN 
#define MAP_TVSIGN -
#define MAP_TU x
#define MAP_TV z


ITIGeometryBuilder3D*	ASEFileComponent::lpBuilder = NULL;	
LONG					ASEFileComponent::LastErrorLine = -1;
ASEGeomObject*			ASEFileComponent::pCurrObject = NULL;



VOID ASEFileComponent::StaticCleanup()
{
	if(pCurrObject)
	{
		delete pCurrObject;
		pCurrObject = NULL;
	}

	lpBuilder = NULL;
}

ASEGeomObject* ASEFileComponent::NewGeomObject()
{
	if(pCurrObject)
	{
		delete pCurrObject;
	}

	return(pCurrObject = new ASEGeomObject);
}

BOOL ASEFileComponent::SkipComponent(AsciiFileParser* lpFile)
{
	String next;
	String open;
	String close;
	BOOL success = TRUE;
	LONG BraceDiffCount = 0;
		
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount();
		return(FALSE);
	}

	open.Set("{");
	close.Set("}");

	if(!open.Equals(next))
	{
		return(success);
	}
	else
	{	
		BraceDiffCount++;


		
		while(BraceDiffCount)
		{
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount();
				return(FALSE);
			}		

			if(open.Equals(next))
			{
				BraceDiffCount++;
			}
			if(close.Equals(next))
			{
				BraceDiffCount--;
			}

		}

	}

	return(success);

}


ASEFileComponentManager::ASEFileComponentManager()
{
	Components.Init(10);
}

BOOL ASEFileComponentManager::AddComponent(ASEFileComponent* lpParent,ASEFileComponent* lpComponent)
{
	DWORD key;
	String strKey;
	
	if(!lpComponent->GetKey(strKey))
	{
		return(FALSE);
	}
	
	key = strKey.CreateHashKey();
	
	if(Components.Insert(key,lpComponent) != S_SUCCESS)
	{
		return(FALSE);
	}
	
	lpComponent->lpParent = lpParent;
	return(TRUE);
}



BOOL ASEFileComponentManager::RemoveComponent(String& Key)
{
	return(Components.Delete(Key.CreateHashKey()) == S_SUCCESS);
}


ASEFileComponent* ASEFileComponentManager::FindComponentFor(String& string)
{
	DWORD key = string.CreateHashKey();
	
	return(Components.Get(key));
}


VOID ASEFileComponentManager::AutoDelete(BOOL Auto)
{
	Components.AutoDelete(Auto);
}


BOOL ASEFileComponent_3DSMAX_EXPORT::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount();
		return(FALSE);
	}
	
	return(success);
}



BOOL ASEFileComponent_3DSMAX_EXPORT::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_3DSMAX_EXPORT);
	return(TRUE);
}

BOOL ASEFileComponent_COMMENT::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String comment;
	
	success &= (lpFile->ParserGetNextQuotedString(comment) == ASCIIFILEPARSER_SUCCESS);
	
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount();
		return(FALSE);
	}
	
	return(success);
}



BOOL ASEFileComponent_COMMENT::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_COMMENT);
	return(TRUE);
}


BOOL ASEFileComponent_SCENE::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String search;
	String next;
	String compare;
	
	compare.Set('{');
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount();
		return(FALSE);
	}
	
	success &= compare.Equals(next);
	
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount();
		return(FALSE);
	}
	
	search.Set('}');
	success &= (lpFile->ParserFindString(search) == ASCIIFILEPARSER_SUCCESS);
	
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount();
		return(FALSE);
	}
	
	
	return(success);
}



BOOL ASEFileComponent_SCENE::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_SCENE);
	return(TRUE);
}


ASEFileComponent_MATERIAL_LIST::ASEFileComponent_MATERIAL_LIST()
{
	pMaterials = NULL;
	MaterialCount = 0;
}

ASEFileComponent_MATERIAL_LIST::~ASEFileComponent_MATERIAL_LIST()
{
	Free();
}

ASEFileComponent_MATERIAL_LIST::Free()
{
	if(pMaterials)
	{
		delete[] pMaterials;
		pMaterials = NULL;
		MaterialCount = 0;
	}
}


BOOL ASEFileComponent_MATERIAL_LIST::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String search;
	String next;
	String compare;
	int skip;
	
	Free();
	
	compare.Set('{');
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount();
		return(FALSE);
	}
	success &= compare.Equals(next);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	
	compare.Set(ASECOMPONENT_KEY_MATERIAL_LIST__MATERIAL_COUNT);
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	success &= compare.Equals(next);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	success &= next.toLong(MaterialCount);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	
	
	if(MaterialCount > 0)
	{
		
		pMaterials = new ASEMATERIAL[MaterialCount];
	}


	
	for(int i = 0; i < MaterialCount; i++)
	{
		compare.Set(ASECOMPONENT_KEY_MATERIAL_LIST__MATERIAL);
		success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);	

		success &= compare.Equals(next);

		if(success)
		{
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			compare.Set("{");
			success &= compare.Equals(next);	
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			//name
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextQuotedString(next) == ASCIIFILEPARSER_SUCCESS);
			//class
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextQuotedString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			//ambient
			compare.Set(ASECOMPONENT_KEY_MATERIAL_LIST__MATERIAL__MATERIAL_AMBIENT);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= compare.Equals(next);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			next.toFloat(pMaterials[i].ambient.r);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			next.toFloat(pMaterials[i].ambient.g);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			next.toFloat(pMaterials[i].ambient.b);


			//diffuse
			compare.Set(ASECOMPONENT_KEY_MATERIAL_LIST__MATERIAL__MATERIAL_DIFFUSE);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= compare.Equals(next);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			next.toFloat(pMaterials[i].diffuse.r);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			next.toFloat(pMaterials[i].diffuse.g);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			next.toFloat(pMaterials[i].diffuse.b);

			//specular
			compare.Set(ASECOMPONENT_KEY_MATERIAL_LIST__MATERIAL__MATERIAL_SPECULAR);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= compare.Equals(next);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			next.toFloat(pMaterials[i].specular.r);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			next.toFloat(pMaterials[i].specular.g);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			next.toFloat(pMaterials[i].specular.b);

			//emissive = shine?
			compare.Set(ASECOMPONENT_KEY_MATERIAL_LIST__MATERIAL__MATERIAL_SHINE);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= compare.Equals(next);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			next.toFloat(pMaterials[i].emissive.r);
			next.toFloat(pMaterials[i].emissive.g);
			next.toFloat(pMaterials[i].emissive.b);

			//power = shine strength?
			compare.Set(ASECOMPONENT_KEY_MATERIAL_LIST__MATERIAL__MATERIAL_SHINESTRENGTH);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= compare.Equals(next);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			next.toFloat(pMaterials[i].power);
			
			for(skip=0;skip<7;skip++)
			{
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			}
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			compare.Set(ASECOMPONENT_KEY_MATERIAL_LIST__MATERIAL__MAP_DIFFUSE);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= compare.Equals(next);
			compare.Set("{");
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= compare.Equals(next);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			for(skip=0;skip<2;skip++)
			{
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
				success &= (lpFile->ParserGetNextQuotedString(next) == ASCIIFILEPARSER_SUCCESS);
			}
			for(skip=0;skip<2;skip++)
			{
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			}
			compare.Set(ASECOMPONENT_KEY_MATERIAL_LIST__MATERIAL__MAP_DIFFUSE__BITMAP);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= compare.Equals(next);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			success &= (lpFile->ParserGetNextQuotedString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			pMaterials[i].pTextureFileName = next.toNewLPCHAR();
			for(skip=0;skip<13;skip++)
			{
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			}
			compare.Set("}");
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= compare.Equals(next);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= compare.Equals(next);
			
			
		}
		else
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}

		
	}
	
	compare.Set('}');
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	
	return(success);
}



BOOL ASEFileComponent_MATERIAL_LIST::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_MATERIAL_LIST);
	return(TRUE);
}


ASEFileComponent_GEOMOBJECT::ASEFileComponent_GEOMOBJECT()
{
	Components.AddComponent(this,&_NODE_NAME);
	Components.AddComponent(this,&_NODE_TM);
	Components.AddComponent(this,&_MESH);
	Components.AddComponent(this,&_PROP_MOTIONBLUR);
	Components.AddComponent(this,&_PROP_CASTSHADOW);
	Components.AddComponent(this,&_PROP_RECVSHADOW);
	Components.AddComponent(this,&_MATERIAL_REF);
	
	Components.AutoDelete(FALSE);
}

BOOL ASEFileComponent_GEOMOBJECT::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	String compare;
	
	compare.Set('{');
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	success &= compare.Equals(next);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	
	if(lpBuilder)
	{
		ASEFileComponent::NewGeomObject();
		success &= lpBuilder->OnStartObject();
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
	}
	
	
	while(lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS)
	{
		ASEFileComponent* lpProcessor = Components.FindComponentFor(next);
		
		if(lpProcessor)
		{
			success &= lpProcessor->Process(lpFile);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
		}
		else
		{
			compare.Set('}');
			if(compare.Equals(next))
			{
				if(lpBuilder)
				{
					if(!ASEFileComponent::pCurrObject)
					{
						ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount();
						return(FALSE);
					}

					success &= lpBuilder->OnNewMesh(ASEFileComponent::pCurrObject->Vertices.Array,ASEFileComponent::pCurrObject->Vertices.UsedCount / 3,_MATERIAL_REF.pMaterial);
					success &= lpBuilder->OnEndObject();
					if(success == FALSE)
					{
						ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
					}
				}
				return(success);
			}
			else
			{
				ASEFileComponent::SkipComponent(lpFile);
			}
		}
		
	}
	
	
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT);
	return(TRUE);
	
}


BOOL ASEFileComponent_GEOMOBJECT__NODE_NAME::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__NODE_NAME::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__NODE_NAME);
	return(TRUE);
}

BOOL ASEFileComponent_GEOMOBJECT__NODE_TM::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	LONG count = 0;	
	String search;
	String next;
	String compare;
	
	compare.Set('{');
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	success &= compare.Equals(next);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	search.Set('}');
	success &= (lpFile->ParserFindString(search) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__NODE_TM::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__NODE_TM);
	return(TRUE);
}

ASEFileComponent_GEOMOBJECT__MESH::ASEFileComponent_GEOMOBJECT__MESH()
{
	Components.AddComponent(this,&_TIMEVALUE);
	Components.AddComponent(this,&_MESH_NUMVERTEX);
	Components.AddComponent(this,&_MESH_NUMFACES);
	Components.AddComponent(this,&_MESH_VERTEX_LIST);
	Components.AddComponent(this,&_MESH_FACE_LIST);
	Components.AddComponent(this,&_MESH_NUMTVERTEX);
	Components.AddComponent(this,&_MESH_NUMCVERTEX);
	Components.AddComponent(this,&_MESH_NORMALS);
	Components.AddComponent(this,&_MESH_TVERT_LIST);
	Components.AddComponent(this,&_MESH_TFACE_LIST);
	Components.AddComponent(this,&_MESH_NUMTFACES);
	
	Components.AutoDelete(FALSE);
	
}

BOOL ASEFileComponent_GEOMOBJECT__MESH::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	String compare;
	
	compare.Set('{');
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	success &= compare.Equals(next);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	
	while(lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS)
	{
		ASEFileComponent* lpProcessor = Components.FindComponentFor(next);
		
		if(lpProcessor)
		{
			success &= lpProcessor->Process(lpFile);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
		}
		else
		{
			compare.Set('}');
			if(compare.Equals(next))
			{
				break;
			}
			else
			{
				ASEFileComponent::SkipComponent(lpFile);
			}
		}
		
	}
	
	
	if(lpBuilder)
	{
		
		if(ASEFileComponent::pCurrObject == NULL)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}

		success &= ASEFileComponent::pCurrObject->AddMesh(_MESH_FACE_LIST.lpFaceVertices,_MESH_NUMFACES.VAL_NUMFACES);
		
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
	}
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__MESH::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH);
	return(TRUE);
}


BOOL ASEFileComponent_GEOMOBJECT__PROP_MOTIONBLUR::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__PROP_MOTIONBLUR::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__PROP_MOTIONBLUR);
	return(TRUE);
}


BOOL ASEFileComponent_GEOMOBJECT__PROP_CASTSHADOW::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__PROP_CASTSHADOW::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__PROP_CASTSHADOW);
	return(TRUE);
}


BOOL ASEFileComponent_GEOMOBJECT__PROP_RECVSHADOW::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__PROP_RECVSHADOW::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__PROP_RECVSHADOW);
	return(TRUE);
}


ASEFileComponent_GEOMOBJECT__MATERIAL_REF::ASEFileComponent_GEOMOBJECT__MATERIAL_REF()
{
	pMaterial = NULL;
}


BOOL ASEFileComponent_GEOMOBJECT__MATERIAL_REF::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	LONG index = -1;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	next.toLong(index);

	if(index < 0 || index >= ((ASEFile*) (lpParent->lpParent))->_MATERIAL_LIST.MaterialCount)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		pMaterial = NULL;
	}
	else
	{
		pMaterial = & (((ASEFile*) (lpParent->lpParent))->_MATERIAL_LIST.pMaterials[index]);
	}

	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__MATERIAL_REF::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MATERIAL_REF);
	return(TRUE);
}



BOOL ASEFileComponent_GEOMOBJECT__MESH__TIMEVALUE::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__TIMEVALUE::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__TIMEVALUE);
	return(TRUE);
}


ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMVERTEX::ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMVERTEX()
{
	VAL_NUMVERTEX = 0;
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMVERTEX::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	next.toLong(VAL_NUMVERTEX);
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMVERTEX::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_NUMVERTEX);
	return(TRUE);
}




BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMFACES::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	next.toLong(VAL_NUMFACES);
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMFACES::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_NUMFACES);
	return(TRUE);
}




BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMTFACES::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	next.toLong(VAL_NUMTFACES);
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMTFACES::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_NUMTFACES);
	return(TRUE);
}



BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_VERTEX_LIST::Process(AsciiFileParser* lpFile)
{
	
	BOOL success = TRUE;
	LONG count = 0;	
	String search;
	String next;
	String compare;
	
	compare.Set('{');
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	success &= compare.Equals(next);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	Free();
	
	if( ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_NUMVERTEX.VAL_NUMVERTEX > 0)
	{
		
		long count = -1;
		int num = ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_NUMVERTEX.VAL_NUMVERTEX;
		float x,y,z;
		lpVertices = new D3DVERTEX[num];
		
		ZeroMemory(lpVertices,sizeof(D3DVERTEX) * num);
		
		
		success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
		
		compare.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_VERTEX_LIST__MESH_VERTEX);
		
		while(next.Equals(compare))
		{
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toLong(count);
			
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toFloat(x);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toFloat(y);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toFloat(z);
			lpVertices[count].x = MAP_X;
			lpVertices[count].y = MAP_Y;
			lpVertices[count].z = MAP_Z;
			
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			
		}
		
		if(count < num - 1)
		{
			success = FALSE;
		}
		
		compare.Set('}');
		success &= next.Equals(compare);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
	}
	else
	{
		search.Set('}');
		success &= (lpFile->ParserFindString(search) == ASCIIFILEPARSER_SUCCESS);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
	}
	
	
	return(success);	
}


ASEFileComponent_GEOMOBJECT__MESH__MESH_VERTEX_LIST::ASEFileComponent_GEOMOBJECT__MESH__MESH_VERTEX_LIST()
{
	lpVertices = NULL;
}

ASEFileComponent_GEOMOBJECT__MESH__MESH_VERTEX_LIST::~ASEFileComponent_GEOMOBJECT__MESH__MESH_VERTEX_LIST()
{
	Free();
}


VOID ASEFileComponent_GEOMOBJECT__MESH__MESH_VERTEX_LIST::Free()
{
	if(lpVertices)
	{
		delete[] lpVertices;
		lpVertices = NULL;
	}
}


BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_VERTEX_LIST::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_VERTEX_LIST);
	return(TRUE);
}


ASEFileComponent_GEOMOBJECT__MESH__MESH_FACE_LIST::ASEFileComponent_GEOMOBJECT__MESH__MESH_FACE_LIST()
{
	lpFaceVertices = NULL;
}


ASEFileComponent_GEOMOBJECT__MESH__MESH_FACE_LIST::~ASEFileComponent_GEOMOBJECT__MESH__MESH_FACE_LIST()
{
	Free();
}


VOID ASEFileComponent_GEOMOBJECT__MESH__MESH_FACE_LIST::Free()
{
	if(lpFaceVertices)
	{
		delete[] lpFaceVertices;
		lpFaceVertices = NULL;
	}
}


BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_FACE_LIST::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;	
	LONG count = 0;	
	String search;
	String next;
	String compare;
	
	compare.Set('{');
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	success &= compare.Equals(next);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	
	Free();
	
	if(lpBuilder)
	{
		long v1,v2,v3;
		long count = -1;
		long vertexCount = ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_NUMVERTEX.VAL_NUMVERTEX;
		long faceCount = ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_NUMFACES.VAL_NUMFACES;
		D3DVERTEX*	lpVertices	= ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_VERTEX_LIST.lpVertices;	
		lpFaceVertices = new D3DVERTEX[faceCount * 3];

		compare.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_FACE_LIST__MESH_FACE);
		success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
		
		while(next.Equals(compare))
		{
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.Strip(next.lSize - 2);
			next.toLong(count);
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toLong(v1);
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toLong(v2);
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toLong(v3);
			
			

			if(lpVertices && (v1 >= 0 && v2 >= 0 && v3 >= 0) && v1 <= vertexCount && v2 <= vertexCount && v3 <= vertexCount)
			{
				lpFaceVertices[(count * 3) + 0] = lpVertices[v1];
				lpFaceVertices[(count * 3) + 1] = lpVertices[v2];
				lpFaceVertices[(count * 3) + 2] = lpVertices[v3];
			}
			else
			{
				success = FALSE;
			}
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			
			
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
		}
		
		
		compare.Set('}');
		success &= next.Equals(compare);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
		
	}
	else
	{
		search.Set('}');
		success &= (lpFile->ParserFindString(search) == ASCIIFILEPARSER_SUCCESS);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
	}
	
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_FACE_LIST::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_FACE_LIST);
	return(TRUE);
}


BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMTVERTEX::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	next.toLong(VAL_NUMTVERTEX);
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMTVERTEX::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_NUMTVERTEX);
	return(TRUE);
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMCVERTEX::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;
	
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NUMCVERTEX::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_NUMCVERTEX);
	return(TRUE);
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NORMALS::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	LONG count = 0;	
	String search;
	String next;
	String compare;
	String compare1;
	
	compare.Set('{');
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	success &= compare.Equals(next);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	
	if(lpBuilder)
	{
		float nx,ny,nz;
		long count = -1;
		long faceCount = ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_NUMFACES.VAL_NUMFACES;
		D3DVERTEX*	lpFaceVertices	= ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_FACE_LIST.lpFaceVertices;	
		
		compare.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_NORMALS__MESH_FACENORMAL);
		compare1.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_NORMALS__MESH_VERTEXNORMAL);
		success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
		
		while(next.Equals(compare))
		{
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toLong(count);
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toFloat(nx);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toFloat(ny);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toFloat(nz);
			
			if(count < faceCount && count >= 0)
			{
				//set all vertex normals to this just in case they dont get any
				lpFaceVertices[(count * 3) + 0].nx = MAP_NX;
				lpFaceVertices[(count * 3) + 0].ny = MAP_NY;
				lpFaceVertices[(count * 3) + 0].nz = MAP_NZ;
				
				lpFaceVertices[(count * 3) + 1].nx = MAP_NX;
				lpFaceVertices[(count * 3) + 1].ny = MAP_NY;
				lpFaceVertices[(count * 3) + 1].nz = MAP_NZ;
				
				lpFaceVertices[(count * 3) + 2].nx = MAP_NX;
				lpFaceVertices[(count * 3) + 2].ny = MAP_NY;
				lpFaceVertices[(count * 3) + 2].nz = MAP_NZ;
				
			}
			else
			{
				success = FALSE;
			}
			
			
			//this way of parsing doesnt consider the vertex indexes so they might and
			//will get messed up if there are not all 3 vertex normals in the file
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			if(next.Equals(compare1))
			{
				//1st vertex normal
				
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
				if(success == FALSE)
				{
					ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
				}
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
				if(success == FALSE)
				{
					ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
				}
				next.toFloat(nx);
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
				
				next.toFloat(ny);
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
				if(success == FALSE)
				{
					ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
				}
				next.toFloat(nz);
				
				
				if(count < faceCount && count >= 0)
				{
					lpFaceVertices[(count * 3) + 0].nx = MAP_NX;
					lpFaceVertices[(count * 3) + 0].ny = MAP_NY;
					lpFaceVertices[(count * 3) + 0].nz = MAP_NZ;
					
				}
				else
				{
					success = FALSE;
				}
				
				success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
				if(success == FALSE)
				{
					ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
				}
				if(next.Equals(compare1))
				{
					//2nd vertex normal
					
					success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
					if(success == FALSE)
					{
						ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
					}
					success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
					if(success == FALSE)
					{
						ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
					}
					next.toFloat(nx);
					success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
					if(success == FALSE)
					{
						ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
					}
					next.toFloat(ny);
					success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
					if(success == FALSE)
					{
						ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
					}
					next.toFloat(nz);
					
					
					if(count < faceCount && count >= 0)
					{
						lpFaceVertices[(count * 3) + 1].nx = MAP_NX;
						lpFaceVertices[(count * 3) + 1].ny = MAP_NY;
						lpFaceVertices[(count * 3) + 1].nz = MAP_NZ;
						
					}
					else
					{
						success = FALSE;
					}
					
					success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
					if(success == FALSE)
					{
						ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
					}
					if(next.Equals(compare1))
					{
						//3rd vertex normal
						
						success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
						if(success == FALSE)
						{
							ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
						}
						success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
						if(success == FALSE)
						{
							ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
						}
						next.toFloat(nx);
						success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
						if(success == FALSE)
						{
							ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
						}
						next.toFloat(ny);
						success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
						
						next.toFloat(nz);
						
						
						if(count < faceCount && count >= 0)
						{
							lpFaceVertices[(count * 3) + 2].nx = MAP_NX;
							lpFaceVertices[(count * 3) + 2].ny = MAP_NY;
							lpFaceVertices[(count * 3) + 2].nz = MAP_NZ;
							
						}
						else
						{
							success = FALSE;
						}
						
						success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
						if(success == FALSE)
						{
							ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
						}
					}
					
					
				}
				
			}
			
			
		}
		
		
		compare.Set('}');
		success &= next.Equals(compare);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
		
	}
	else
	{
		search.Set('}');
		success &= (lpFile->ParserFindString(search) == ASCIIFILEPARSER_SUCCESS);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
	}
	
	
	
	return(success);	
	
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_NORMALS::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_NORMALS);
	return(TRUE);
}


BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_TVERT_LIST::Process(AsciiFileParser* lpFile)
{
	
	BOOL success = TRUE;
	LONG count = 0;	
	String search;
	String next;
	String compare;
	
	compare.Set('{');
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	success &= compare.Equals(next);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	Free();
	
	if( ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_NUMTVERTEX.VAL_NUMTVERTEX > 0)
	{
		
		long count = -1;
		int num = ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_NUMTVERTEX.VAL_NUMTVERTEX;
		float x,y,z;
		pTVertices = new D3DVERTEX[num];
		
		ZeroMemory(pTVertices,sizeof(D3DVERTEX) * num);
		
		
		success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
		
		compare.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_TVERT_LIST__MESH_TVERT);
		
		while(next.Equals(compare))
		{
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toLong(count);
			
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toFloat(x);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toFloat(y);
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toFloat(z);
			pTVertices[count].x = MAP_X;
			pTVertices[count].y = MAP_Y;
			pTVertices[count].z = MAP_Z;
			
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			
		}
		
		if(count < num - 1)
		{
			success = FALSE;
		}
		
		compare.Set('}');
		success &= next.Equals(compare);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
	}
	else
	{
		search.Set('}');
		success &= (lpFile->ParserFindString(search) == ASCIIFILEPARSER_SUCCESS);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
	}
	
	
	return(success);	
}


ASEFileComponent_GEOMOBJECT__MESH__MESH_TVERT_LIST::ASEFileComponent_GEOMOBJECT__MESH__MESH_TVERT_LIST()
{
	pTVertices = NULL;
}

ASEFileComponent_GEOMOBJECT__MESH__MESH_TVERT_LIST::~ASEFileComponent_GEOMOBJECT__MESH__MESH_TVERT_LIST()
{
	Free();
}


VOID ASEFileComponent_GEOMOBJECT__MESH__MESH_TVERT_LIST::Free()
{
	if(pTVertices)
	{
		delete[] pTVertices;
		pTVertices = NULL;
	}
}


BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_TVERT_LIST::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_TVERT_LIST);
	return(TRUE);
}



BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_TFACE_LIST::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;	
	LONG count = 0;	
	String search;
	String next;
	String compare;
	
	compare.Set('{');
	success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	success &= compare.Equals(next);
	if(success == FALSE)
	{
		ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
	}
	
	
	
	if(lpBuilder)
	{
		long v1,v2,v3;
		long count = -1;
		long vertexCount = ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_NUMTVERTEX.VAL_NUMTVERTEX;
		long faceCount = ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_NUMFACES.VAL_NUMFACES;
		D3DVERTEX* pTVertices	= ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_TVERT_LIST.pTVertices;	
		D3DVERTEX* pTFaceVertices = ((ASEFileComponent_GEOMOBJECT__MESH*) lpParent)->_MESH_FACE_LIST.lpFaceVertices;

		
		
		compare.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_TFACE_LIST__MESH_TFACE);
		success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
		
		while(next.Equals(compare))
		{
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toLong(count);
			
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toLong(v1);
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toLong(v2);
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			next.toLong(v3);
			
			

			if(pTVertices && (v1 >= 0 && v2 >= 0 && v3 >= 0) && v1 <= vertexCount && v2 <= vertexCount && v3 <= vertexCount)
			{
				pTFaceVertices[(count * 3) + 0].tu = MAP_TUSIGN pTVertices[v1].MAP_TU;
				pTFaceVertices[(count * 3) + 0].tv = MAP_TVSIGN pTVertices[v1].MAP_TV;
				pTFaceVertices[(count * 3) + 1].tu = MAP_TUSIGN pTVertices[v2].MAP_TU;
				pTFaceVertices[(count * 3) + 1].tv = MAP_TVSIGN pTVertices[v2].MAP_TV;
				pTFaceVertices[(count * 3) + 2].tu = MAP_TUSIGN pTVertices[v3].MAP_TU;
				pTFaceVertices[(count * 3) + 2].tv = MAP_TVSIGN pTVertices[v3].MAP_TV;
			}
			else
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
			
			
			
			success &= (lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS);
			if(success == FALSE)
			{
				ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
			}
		}
		
		
		compare.Set('}');
		success &= next.Equals(compare);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
		
	}
	else
	{
		search.Set('}');
		success &= (lpFile->ParserFindString(search) == ASCIIFILEPARSER_SUCCESS);
		if(success == FALSE)
		{
			ASEFileComponent::LastErrorLine = lpFile->GetCurrentLineCount(); return(FALSE);
		}
	}
	
	
	return(success);
}

BOOL ASEFileComponent_GEOMOBJECT__MESH__MESH_TFACE_LIST::GetKey(String& str)
{
	str.Set(ASECOMPONENT_KEY_GEOMOBJECT__MESH__MESH_TFACE_LIST);
	return(TRUE);
}

