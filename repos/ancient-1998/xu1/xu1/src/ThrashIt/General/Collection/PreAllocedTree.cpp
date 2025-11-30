#include "PreAllocedTree.h"


#ifdef _DEBUG
	LONG PreAllocedTreeNode::TreeNodeCount = 0;
#endif



PreAllocedTreeNode::PreAllocedTreeNode()
{
	//pParent = NULL;
	pChildrenPointers = NULL;
	Size = 0;

#ifdef _DEBUG
	TreeNodeCount++;
#endif

}


PreAllocedTreeNode::~PreAllocedTreeNode()
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

VOID PreAllocedTreeNode::Destroy()
{
	
	for(int i = 0; i < Size; i++) 
	{
		if(pChildrenPointers[i] != NULL)
		{
			delete pChildrenPointers[i];
			pChildrenPointers[i] = NULL;
		}
	}

	Size = 0;
	
}


BOOL PreAllocedTreeNode::CreateChildArray(BYTE size)
{
	if(pChildrenPointers)
	{
		return(NULL);
	}

	Size = size;
	pChildrenPointers = new PreAllocedTreeNode*[Size];

	memset(pChildrenPointers,0,sizeof(PreAllocedTreeNode*) * Size);
	return(TRUE);
}


BOOL PreAllocedTreeNode::AddChild(PreAllocedTreeNode* lpChild)
{
	if(lpChild == NULL /*|| lpChild->pParent != NULL*/)
	{
		return(FALSE);
	}

	for(int i = 0; i < Size; i++) 
	{
		if(pChildrenPointers[i] == NULL)
		{
			pChildrenPointers[i] = lpChild;
			//lpChild->pParent = this;
			return(TRUE);
		}
	}

	
	return(FALSE);
}



BOOL PreAllocedTreeNode::DestroyChild(PreAllocedTreeNode* lpChild)
{
	if(lpChild == NULL /*|| lpChild->pParent != this*/)
	{
		return(FALSE);
	}

	
	for(int i = 0; i < Size; i++) 
	{
		if(pChildrenPointers[i] == lpChild)
		{
			delete pChildrenPointers[i];
			pChildrenPointers[i] = NULL;
			return(TRUE);
		}
	}

	
	return(FALSE);
}



BOOL PreAllocedTreeNode::RemoveChild(PreAllocedTreeNode* lpChild)
{
	WORD index;

	if(lpChild == NULL /*|| lpChild->pParent != this*/)
	{
		return(FALSE);
	}

	for(int i = 0; i < Size; i++) 
	{
		if(pChildrenPointers[i] == lpChild)
		{
			pChildrenPointers[i] = NULL;
			return(TRUE);
		}
	}


	return(FALSE);
}

