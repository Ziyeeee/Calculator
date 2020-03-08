#pragma once
class BinaryTreeNode
{
public:
	BinaryTreeNode();
	~BinaryTreeNode();
	char op_;								//存储操作符,如果是数字则默认为'#'
	double num_;							//存取数字
	int lch;								//左结点
	int rch;								//右结点
	void set_node(char op, double num);		//给二叉树结点赋值
	void allocate_rch(int r);				//分配右孩子
	void allocate_lch(int l);				//分配左孩子
private:

};


