#include "ITIStaticGeometryPreBuildModule.h"
#include "..\..\..\General\Collection\CFastPrimitiveArrayKeepTogether.cpp"

ITIStaticGeometryPreBuildModule::ITIStaticGeometryPreBuildModule(DWORD verboseLevel)
{
	SuccessFileCount = 0;
	TotalFileCount = 0;
	VerboseLevel = verboseLevel;
	TempVertices.Create(18);
	TempMaterials.Create(256);
}

ITIStaticGeometryPreBuildModule::~ITIStaticGeometryPreBuildModule()
{
}


WORD ITIStaticGeometryPreBuildModule::RegisterMaterial(ASEMATERIAL* pMaterial)
{
	
	if(pMaterial == NULL)
	{
		return(-1);
	}
	
	for(int i = 0;i < TempMaterials.UsedCount; i++)
	{
		if(TempMaterials.Array[i].Equals(*pMaterial))
		{
			return(i);
		}
	}

	

	TempMaterials.New()->Set(*pMaterial);

	return(TempMaterials.UsedCount - 1);
}

BOOL ITIStaticGeometryPreBuildModule::OnNewMesh(D3DVERTEX* pVertices,long faceCount,ASEMATERIAL* pMaterial)
{
	
	WORD MatIndex = RegisterMaterial(pMaterial);
	
	for(int i = 0; i < faceCount; i++)
	{
		
		PreBuildVertex* lpNew1 = TempVertices.New();
		PreBuildVertex* lpNew2 = TempVertices.New();
		PreBuildVertex* lpNew3 = TempVertices.New();
		
		if(lpNew1 && lpNew2 && lpNew3)
		{
			lpNew1->Set(pVertices[(i * 3) + 0],MatIndex);
			lpNew2->Set(pVertices[(i * 3) + 1],MatIndex);
			lpNew3->Set(pVertices[(i * 3) + 2],MatIndex);
			
		}
		else
		{
			if(lpNew1)
			{
				TempVertices.Delete(lpNew1);
			}
			if(lpNew2)
			{
				TempVertices.Delete(lpNew2);
			}
			if(lpNew3)
			{
				TempVertices.Delete(lpNew3);
			}
			
			return(FALSE);
		}
	}

	return(TRUE);
}

BOOL ITIStaticGeometryPreBuildModule::_Build(ThrashFile& outputFile)
{
	String temp;
	temp.Set("Nothing built : No Module Defined");
	Print(temp,1);
	return(FALSE);
}


VOID ITIStaticGeometryPreBuildModule::SetVerboseLevel(DWORD verboseLevel)
{
	VerboseLevel = verboseLevel;
}

VOID ITIStaticGeometryPreBuildModule::PrintCurrentData()
{
	String temp;

	temp.Set("Module Data [ ");
	temp.Append((LONG) TempVertices.UsedCount);
	temp.Append(" Vertices] [ ");
	temp.Append((LONG) (TempVertices.UsedCount * sizeof(D3DVERTEX)));
	temp.Append(" Vertex Bytes] \r\n[ ");
	temp.Append((LONG) TempMaterials.UsedCount);
	temp.Append(" Materials] [ ");
	temp.Append((LONG) (TempMaterials.UsedCount * sizeof(D3DMATERIAL)));
	temp.Append(" Material Bytes] \r\n");
	temp.Append("Succesfully Processed Files ");
	temp.Append(SuccessFileCount);
	temp.Append(" / ");
	temp.Append(TotalFileCount);
	temp.Append(" ...");

	Print(temp,1);
}


VOID ITIStaticGeometryPreBuildModule::ResetFileCount()
{
	SuccessFileCount = 0;
	TotalFileCount = 0;
}


BOOL ITIStaticGeometryPreBuildModule::AddFile(LPCHAR path)
{
	BOOL ret;
	String temp;
	ASEFile file;
	
	TotalFileCount++;

	temp.Set("Processing ");
	temp.Append(path);
	temp.Append(" ...");
	Print(temp,1);
	
	file.lpBuilder = this;
	ret = file.Process(path);

	if(ret)
	{
		SuccessFileCount++;
		temp.Set(" Processed Succesfully");
	}
	else
	{
		temp.Set(" Failed, Error at Line ");
		temp.Append(file.LastErrorLine);
		temp.Append(" All Loaded Data Deleted");
		TempVertices.Create(18);
		TempMaterials.Create(5);
	}

	

	Print(temp,1);
	PrintCurrentData();
	return(ret);
}


BOOL ITIStaticGeometryPreBuildModule::Build(LPCHAR path)
{
	String temp;
	ThrashFile outputFile;


	
	outputFile.SetPath(path);
	outputFile.OpenWriteOnly(TRUE);

	if(!outputFile.CanWrite())
	{
		temp.Set("Build Failed : Unable to open output file");
		Print(temp,1);
		return(FALSE);
	}

	temp.Set("Starting to Build ...");

	Print(temp,1);

	BOOL ret = _Build(outputFile);
	TempVertices.Create(18);
	TempMaterials.Create(5);

	if(ret)
	{
		temp.Set("Build Succeeded");
	}
	else
	{
		temp.Set("Build Failed, File Deleted");
		outputFile.Delete();
	}

	Print(temp,1);
	PrintCurrentData();

	return(ret);
}


VOID ITIStaticGeometryPreBuildModule::Print(String& str,DWORD level)
{
	if(level == 0)
	{
		return;
	}
	if(VerboseLevel >= level)
	{
		printf(str.startAddress);
		printf("\r\n");
	}
}
