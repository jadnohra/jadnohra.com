#include "FixedTree.h"


#ifdef _DEBUG
	FIXEDTREETEMPLATEFUNCTION
	LONG FixedTreeNode<CHILDCOUNT>::TreeNodeCount = 0;
#endif


FIXEDTREETEMPLATEFUNCTION
FixedTreeNode<CHILDCOUNT>::FixedTreeNode()
{
	lpParent = NULL;
	ZeroMemory(lpChildrenPointers,CHILDCOUNT * sizeof(this));

#ifdef _DEBUG
	TreeNodeCount++;
#endif

}


FIXEDTREETEMPLATEFUNCTION
FixedTreeNode<CHILDCOUNT>::~FixedTreeNode()
{
	Destroy();

#ifdef _DEBUG
	TreeNodeCount--;
	/*
	if(TreeNodeCount <= 0)
	{
		TreeNodeCount = TreeNodeCount;
	}
	*/
	
#endif

}

FIXEDTREETEMPLATEFUNCTION
VOID FixedTreeNode<CHILDCOUNT>::Destroy()
{
	
	for(int i = 0; i < CHILDCOUNT; i++) 
	{
		if(lpChildrenPointers[i] != NULL)
		{
			delete lpChildrenPointers[i];
			lpChildrenPointers[i] = NULL;
		}
	}
	
}


FIXEDTREETEMPLATEFUNCTION
BOOL FixedTreeNode<CHILDCOUNT>::AddChild(FixedTreeNode* lpChild)
{
	if(lpChild == NULL || lpChild->lpParent != NULL)
	{
		return(FALSE);
	}

	for(int i = 0; i < CHILDCOUNT; i++) 
	{
		if(lpChildrenPointers[i] == NULL)
		{
			lpChildrenPointers[i] = lpChild;
			lpChild->lpParent = this;
			return(TRUE);
		}
	}

	
	return(FALSE);
}


FIXEDTREETEMPLATEFUNCTION
BOOL FixedTreeNode<CHILDCOUNT>::DestroyChild(FixedTreeNode* lpChild)
{
	if(lpChild == NULL || lpChild->lpParent != this)
	{
		return(FALSE);
	}

	
	for(int i = 0; i < CHILDCOUNT; i++) 
	{
		if(lpChildrenPointers[i] == lpChild)
		{
			delete lpChildrenPointers[i];
			lpChildrenPointers[i] = NULL;
			return(TRUE);
		}
	}

	
	return(FALSE);
}


FIXEDTREETEMPLATEFUNCTION
BOOL FixedTreeNode<CHILDCOUNT>::RemoveChild(FixedTreeNode* lpChild)
{
	WORD index;

	if(lpChild == NULL || lpChild->lpParent != this)
	{
		return(FALSE);
	}

	for(int i = 0; i < CHILDCOUNT; i++) 
	{
		if(lpChildrenPointers[i] == lpChild)
		{
			lpChildrenPointers[i] = NULL;
			return(TRUE);
		}
	}


	return(FALSE);
}

