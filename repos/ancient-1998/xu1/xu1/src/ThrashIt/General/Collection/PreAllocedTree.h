#ifndef _PREALLOCEDTREE_H__
#define _PREALLOCEDTREE_H__

#include "..\_INCLUDES.h"


class PreAllocedTreeNode
{

public:

#ifdef _DEBUG
	static LONG TreeNodeCount;
#endif

	//PreAllocedTreeNode*		pParent;
	PreAllocedTreeNode**	pChildrenPointers;
	BYTE					Size;
	

	PreAllocedTreeNode();
	virtual ~PreAllocedTreeNode();

	BOOL CreateChildArray(BYTE size);
	BOOL AddChild(PreAllocedTreeNode* lpChild);
	BOOL DestroyChild(PreAllocedTreeNode* lpChild);
	BOOL RemoveChild(PreAllocedTreeNode* lpChild);
	VOID Destroy();
};



#endif