#ifndef _TREE_H__
#define _TREE_H__

#include "CFastPtrArrayKeepTogether.h"
#include "CFastPrimitiveArrayKeepTogether.h"


class TreeNode;
struct TreeNodeFlatNodeInfo
{
	WORD *pChildIndexes;
	TreeNode *pNode;
	
	TreeNodeFlatNodeInfo();
	VOID Create(TreeNode* lpNode);
	~TreeNodeFlatNodeInfo();
};

class TreeNode
{

public:

	static LONG TreeNodeCount;


	TreeNode*								lpParent;
	CFastPtrArrayKeepTogether<TreeNode>*	lpChildrenPointers;
	

	TreeNode();
	TreeNode(int ExpectedChildrenCount);
	virtual ~TreeNode();

	BOOL AddChild(TreeNode* lpChild);
	BOOL DestroyChild(TreeNode* lpChild);
	BOOL RemoveChild(TreeNode* lpChild);
	VOID Destroy();

	VOID ToFlatNodePtrArrayByLevel(CFastPrimitiveArrayKeepTogether<TreeNodeFlatNodeInfo>& flatArray);
};



#endif