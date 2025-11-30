#include "Tree.h"

#include "CFastPtrArrayKeepTogether.cpp"
#include "CFastPrimitiveArrayKeepTogether.cpp"

LONG TreeNode::TreeNodeCount = 0;


TreeNode::TreeNode()
{
	lpParent = NULL;
	lpChildrenPointers = NULL;

	TreeNodeCount++;

}

TreeNode::TreeNode(int ExpectedChildrenCount)
{
	lpParent = NULL;
	lpChildrenPointers = new CFastPtrArrayKeepTogether<TreeNode>;
	lpChildrenPointers->Create(ExpectedChildrenCount);
}

TreeNode::~TreeNode()
{
	Destroy();

	TreeNodeCount--;
	/*
	if(TreeNodeCount <= 0)
	{
		TreeNodeCount = TreeNodeCount;
	}
	*/
	

}

VOID TreeNode::Destroy()
{
	if(lpChildrenPointers != NULL)
	{
		delete lpChildrenPointers;
		lpChildrenPointers = NULL;
	}
}

BOOL TreeNode::AddChild(TreeNode* lpChild)
{
	if(lpChild == NULL || lpChild->lpParent != NULL)
	{
		return(FALSE);
	}

	if(lpChildrenPointers == NULL)
	{
		lpChildrenPointers = new CFastPtrArrayKeepTogether<TreeNode>;
		lpChildrenPointers->Create(1);
	}

	*lpChildrenPointers->New() = lpChild;
	lpChild->lpParent = this;

	return(TRUE);
}

BOOL TreeNode::DestroyChild(TreeNode* lpChild)
{
	if(lpChild == NULL || lpChild->lpParent != this)
	{
		return(FALSE);
	}

	lpChildrenPointers->DeletePtr(lpChild);

	if(lpChildrenPointers->UsedCount == 0)
	{
		delete lpChildrenPointers;
		lpChildrenPointers = NULL;
	}

	return(TRUE);
}

BOOL TreeNode::RemoveChild(TreeNode* lpChild)
{
	WORD index;

	if(lpChild == NULL || lpChild->lpParent != this)
	{
		return(FALSE);
	}

	lpChild->lpParent = NULL;
	index = lpChildrenPointers->PtrIndex(lpChild);
	lpChildrenPointers->Delete(index);

	if(lpChildrenPointers->UsedCount == 0)
	{
		delete lpChildrenPointers;
		lpChildrenPointers = NULL;
	}

	return(TRUE);
}


VOID TreeNode::ToFlatNodePtrArrayByLevel(CFastPrimitiveArrayKeepTogether<TreeNodeFlatNodeInfo>& flatArray)
{
	TreeNodeFlatNodeInfo* pTemp;
	CFastPtrArrayKeepTogether<TreeNode> Level1;
	CFastPtrArrayKeepTogether<TreeNode> Level2;
	CFastPtrArrayKeepTogether<TreeNode>* pLevel1; 
	CFastPtrArrayKeepTogether<TreeNode>* pLevel2;
	CFastPtrArrayKeepTogether<TreeNode>* pTempLevel;

	flatArray.Destroy();
	flatArray.Create(TreeNodeCount);

	Level1.Create(TreeNodeCount);
	Level2.Create(TreeNodeCount);
	Level1.AutoDelete(FALSE);
	Level2.AutoDelete(FALSE);

	*Level1.New() = this;
	pLevel1 = &Level1;
	pLevel2 = &Level2;

	while(pLevel1->UsedCount)
	{
		
		int levelIndex = flatArray.UsedCount + pLevel1->UsedCount;
		
		pLevel2->DeleteAll();
		for(int i = 0;i < pLevel1->UsedCount; i++)
		{
			TreeNode* pNode = pLevel1->Array[i];
			if(pNode->lpChildrenPointers)
			{
				for(int j = 0;j < pNode->lpChildrenPointers->UsedCount; j++)
				{
					*Level2.New() = pNode->lpChildrenPointers->Array[j];
				}
			}
		}


		for(i = 0;i < pLevel1->UsedCount; i++)
		{
			pTemp = flatArray.New();
			pTemp->Create(pLevel1->Array[i]);

			if(pTemp->pChildIndexes)
			{
				if(pTemp->pNode->lpChildrenPointers)
				{
					for(int i = 0; i < this->lpChildrenPointers->UsedCount; i++)
					{
						pTemp->pChildIndexes[i] = levelIndex++;
					}
				}
			}
			
		}


		pTempLevel = pLevel1;
		pLevel1 = pLevel2;
		pLevel2 = pTempLevel;
	}
	
}


TreeNodeFlatNodeInfo::TreeNodeFlatNodeInfo()
{
	pChildIndexes = NULL;
	pNode = NULL;
}

VOID TreeNodeFlatNodeInfo::Create(TreeNode* lpNode)
{
	pNode = lpNode;
	if(pNode->lpChildrenPointers && pNode->lpChildrenPointers->UsedCount)
	{
		pChildIndexes = new WORD[pNode->lpChildrenPointers->UsedCount];
	}
}

TreeNodeFlatNodeInfo::~TreeNodeFlatNodeInfo()
{
	if(pChildIndexes)
	{
		delete[] pChildIndexes;
		pChildIndexes = NULL;
	}
	pNode = NULL;
}
