#include "ASEFile.h"



ASEFile::ASEFile()
{
	Components.AddComponent(this,&_3DSMAX_EXPORT);
	Components.AddComponent(this,&_COMMENT);
	Components.AddComponent(this,&_SCENE);
	Components.AddComponent(this,&_MATERIAL_LIST);
	Components.AddComponent(this,&_GEOMOBJECT);

	Components.AutoDelete(FALSE);
}
	
ASEFile::~ASEFile()
{
	ASEFileComponent::StaticCleanup();
}


BOOL ASEFile::Process(LPCHAR FileName)
{
	AsciiFileParser parser;

	if(!parser.Connect(FileName))
	{
		return(FALSE);	
	}

	return(Process(&parser));

}

BOOL ASEFile::Process(AsciiFileParser* lpFile)
{
	BOOL success = TRUE;
	String next;

	if(lpBuilder)
	{
		success &= lpBuilder->OnStartBuild();
	}


	while(lpFile->ParserGetNextString(next) == ASCIIFILEPARSER_SUCCESS)
	{
		ASEFileComponent* lpProcessor = Components.FindComponentFor(next);

		if(lpProcessor)
		{
			success &= lpProcessor->Process(lpFile);
			
		}

	}

	if(lpBuilder)
	{
		success &= lpBuilder->OnEndBuild();
	}
	
	return(success);
}


BOOL ASEFile::GetKey(String& str)
{
	return(FALSE);
}

