// TIStaticBuilder.cpp : Defines the entry point for the console application.
//
#include "TIStaticGeometryPreBuildModuleBinaryAreaSize.h"
#include "stdio.h"
#include "conio.h"

#include "..\..\..\General\Math\TIMath.h"
#include "..\..\..\General\Math\TIMath.cpp"

int main(int argc, char* argv[])
{
	
	
	
	TIStaticGeometryPreBuildModuleBinaryAreaSize module;
	BOOL ok = TRUE;

	printf("TIStaticBuilder started...\r\n\r\n");
	
	for(int i = 1; i < argc; i++)
	{
		ok &= module.AddFile(argv[i]);	
	}

	if(ok)
	{
		module.Setup(50.0f,50.0f,TIStaticGeometryPreBuildModuleBinaryAreaSize::MODE_XZ,FALSE);
		module.Build((LPCHAR) "map.tis");
	}
	else
	{
		printf("\r\nsome files failed to load ...\r\n");
	}
	
	printf("\r\n\r\nTIStaticBuilder finished ...\r\n");
	
	getch();
	return 0;
}
