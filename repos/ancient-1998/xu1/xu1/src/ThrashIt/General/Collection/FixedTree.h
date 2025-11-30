#ifndef _FIXEDTREE_H__
#define _FIXEDTREE_H__

#include "..\_INCLUDES.h"

#define FIXEDTREETEMPLATECLASS template<int CHILDCOUNT> class
#define FIXEDTREETEMPLATEFUNCTION template<int CHILDCOUNT> 

FIXEDTREETEMPLATECLASS FixedTreeNode
{

public:

#ifdef _DEBUG
	static LONG TreeNodeCount;
#endif

	FixedTreeNode*	lpParent;
	FixedTreeNode*	lpChildrenPointers[CHILDCOUNT];
	

	FixedTreeNode();
	virtual ~FixedTreeNode();

	BOOL AddChild(FixedTreeNode* lpChild);
	BOOL DestroyChild(FixedTreeNode* lpChild);
	BOOL RemoveChild(FixedTreeNode* lpChild);
	VOID Destroy();
};



#endif