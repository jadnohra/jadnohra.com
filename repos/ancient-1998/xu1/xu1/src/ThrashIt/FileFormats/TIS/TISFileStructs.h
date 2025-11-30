#ifndef _TISFILESTRUCTS_H__
#define _TISFILESTRUCTS_H__

#include "..\..\MabenEngine\_TIINCLUDES.h"
#include "..\..\General\Collection\String.h"

#define TISFILE_VER 1

/*
	HEADER
	SCENE
	ADDRESS(TREE)
*/



struct TISFILE_STRUCTADDRESS
{
	LONG Address;
};


struct TISFILE_HEADER
{
	LONG	ID;
	LONG	IDNeg;
	LONG	Ver;
	LONG	VerNeg;
	CHAR	Comment[256];
	CHAR	NULLCHAR;



	BOOL IsCurrentVersion()
	{
		return(Ver == TISFILE_VER);
	}

	BOOL IsCorrupt()
	{
		return(	!((ID != 0) 
				&& (ID == -IDNeg)
				&& (Ver == -VerNeg)
				&& (NULLCHAR == '\x0'))
				);
	}

	BOOL Build(DWORD id,DWORD ver,LPCHAR comment)
	{
		
		String temp;

		if(ID == 0)
		{
			ID = 1;
		}
		IDNeg = -ID;
		Ver = ver;
		VerNeg = -Ver;
		
		if(comment == NULL)
		{
			memset(Comment,0,256);
		}
		else
		{
			temp.Set(comment);
			temp.Strip(255);
			temp.Get(Comment);
		}
		NULLCHAR = 0;
	
	
		return(!IsCorrupt());
		
	}
};


struct TISFILE_SCENEMANAGMENT
{
	LONG SceneIsManaged;
	LONG Ver;
	
	FLOAT Velocities[3];
	FLOAT Time;
	
	LONG RequiredMemory;

	
	BOOL IsCorrupt()
	{
		if(!SceneIsManaged)
		{
			return(FALSE);
		}

		return( !((Velocities[0] > 0)
				&& (Velocities[1] > 0)
				&& (Velocities[2] > 0)
				&& (Time > 0)
				&& (RequiredMemory > 0))
				);
	};


	VOID Build()
	{
		SceneIsManaged = 0;
	}

	BOOL Build(LONG ver,FLOAT velocities[3],FLOAT time,LONG requiredMemory)
	{
		SceneIsManaged = 1;
		Ver = ver;
		for(int i = 0; i < 3; i++)
		{
			Velocities[i] = velocities[i];
		}

		RequiredMemory = requiredMemory;

		return(!IsCorrupt());
	}
};


struct TISFILE_CHARARRAY
{
	LONG Size;	//include NULL
	LPCHAR Chars;

	VOID Build(String& str)
	{
		Destroy();

		Size = str.lSize;
		Chars = str.toNewLPCHAR();
	}

	VOID Build(LPCHAR str)
	{
		if(str == NULL)
		{
			Destroy();
		}
		else
		{
			String temp;
			temp.Set(str);
			Build(temp);
		}
	}

	
	TISFILE_CHARARRAY()
	{
		Size = 0;
		Chars = NULL;
	}

	~TISFILE_CHARARRAY()
	{
		Destroy();
	}

	VOID Destroy()
	{
		if(Chars)
		{
			delete[] Chars;
		}
		Size = 0;
	}
};

struct TISFILE_PATHARRAY
{
	LONG Size;
	TISFILE_CHARARRAY* Paths;

	TISFILE_PATHARRAY()
	{
		Size = 0;
		Paths = NULL;
	}

	~TISFILE_PATHARRAY()
	{
		Destroy();
	}

	VOID Destroy()
	{
		if(Paths)
		{
			delete[] Paths;
		}
		Size = 0;
	}
};


struct TISFILE_PACKAGEARRAY
{
	LONG Size;
	TISFILE_CHARARRAY* Packages;

	TISFILE_PACKAGEARRAY()
	{
		Size = 0;
		Packages = NULL;
	}

	~TISFILE_PACKAGEARRAY()
	{
		Destroy();
	}

	VOID Destroy()
	{
		if(Packages)
		{
			delete[] Packages;
		}
		Size = 0;
	}
};


struct TISFILE_MATERIAL 
{
	D3DMATERIAL7 Material;
	TISFILE_CHARARRAY TextureFile;
};

struct TISFILE_MATERIALARRAY
{
	LONG Size;
	TISFILE_MATERIAL* Materials;

	TISFILE_MATERIALARRAY()
	{
		Size = 0;
		Materials = NULL;
	}

	~TISFILE_MATERIALARRAY()
	{
		Destroy();
	}

	VOID Destroy()
	{
		if(Materials)
		{
			delete[] Materials;
		}
		Size = 0;
	}

};


struct TISFILE_LONGARRAY
{
	LONG Size;	
	LPLONG Longs;


	TISFILE_LONGARRAY()
	{
		Size = 0;
		Longs = NULL;
	}

	~TISFILE_LONGARRAY()
	{
		Destroy();
	}

	VOID Destroy()
	{
		if(Longs)
		{
			delete[] Longs;
		}
		Size = 0;
	}
};

struct TISFILE_TREE
{
	LONG BuildAlgorithm;
	LONG TotalNodeCount;
	LONG SubdivNodeCount;
	LONG RenderNodeCount;
	BYTE LODLevelCount;

	TISFILE_LONGARRAY NodeAddresses;
	TISFILE_LONGARRAY SubdivNodeAddresses;
	TISFILE_LONGARRAY RenderNodeAddresses;

	BOOL IsCorrupt()
	{
		return( (TotalNodeCount >= 0) 
				&& (SubdivNodeCount >= 0) 
				&& (RenderNodeCount >= 0) 
				&& (LODLevelCount >= 1) 
				&& (RenderNodeCount + RenderNodeCount == TotalNodeCount) 
				&& (SubdivNodeAddresses.Size == SubdivNodeCount)
				&& (RenderNodeAddresses.Size == RenderNodeCount)
				&& (NodeAddresses.Size == TotalNodeCount)
				);
	}
};


#define TISFILE_BASENODE_MINDIM1 0	 
#define TISFILE_BASENODE_MAXDIM1 1		 
#define TISFILE_BASENODE_MINDIM2 2	 
#define TISFILE_BASENODE_MAXDIM2 3	 
#define TISFILE_BASENODE_MINDIM3 4	 
#define TISFILE_BASENODE_MAXDIM3 5	 

struct TISFILE_BASENODE
{
	FLOAT	Box[6]; 
	BYTE	IsRenderNode;
};


struct TISFILE_SUBDIVNODE : TISFILE_BASENODE
{
	TISFILE_LONGARRAY SubnodeIndexes;		
};


struct TISFILE_D3DVERTEXARRAY
{
	WORD		Size;	
	DWORD		RenderMode;
	D3DVERTEX*	Vertices;


	TISFILE_D3DVERTEXARRAY()
	{
		Size = 0;
		Vertices = NULL;
	}

	~TISFILE_D3DVERTEXARRAY()
	{
		Destroy();
	}

	VOID Destroy()
	{
		if(Vertices)
		{
			delete[] Vertices;
		}
		Size = 0;
	}
};

struct TISFILE_RENDERNODEMATERIALGROUP
{
	WORD MaterialIndex;
	TISFILE_D3DVERTEXARRAY Vertices;
};

struct TISFILE_RENDERNODELODLEVEL
{
	WORD Size;	
	TISFILE_RENDERNODEMATERIALGROUP Vertices;
};


struct TISFILE_RENDERNODELODARRAY
{
	BYTE Size;	
	TISFILE_RENDERNODELODLEVEL Levels;
};

struct TISFILE_RENDERNODE : TISFILE_BASENODE
{
	FLOAT Center[3];

	TISFILE_RENDERNODELODARRAY LODLevels;
};

#endif