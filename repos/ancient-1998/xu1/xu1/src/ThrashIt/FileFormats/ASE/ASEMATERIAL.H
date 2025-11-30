#ifndef _ASEMATERIAL_H__
#define _ASEMATERIAL_H__

#include "d3d.h"
#include "..\..\General\Collection\String.h"

struct ASEMATERIAL : public D3DMATERIAL7
{
	LPCHAR pTextureFileName;
	ASEMATERIAL()
	{
		memset(this,0,sizeof(ASEMATERIAL));
	}
	~ASEMATERIAL()
	{
		if(pTextureFileName)
		{
			delete pTextureFileName;
			pTextureFileName = NULL;
		}
	}

	VOID Set(ASEMATERIAL& ref)
	{
		memcpy((D3DMATERIAL7*) this,(D3DMATERIAL7*) &ref,sizeof(D3DMATERIAL));
		if(ref.pTextureFileName)
		{
			String temp;
			temp.Set(ref.pTextureFileName);
			pTextureFileName = temp.toNewLPCHAR();
		}
	}

	BOOL Equals(ASEMATERIAL& ref)
	{
		if(memcmp((D3DMATERIAL7*) this,(D3DMATERIAL7*) &ref,sizeof(D3DMATERIAL7)) == 0)
		{
			String temp;
			temp.Set(ref.pTextureFileName);
			return(temp.Equals(pTextureFileName));
		}
		
		return(FALSE);
	}
};

#endif