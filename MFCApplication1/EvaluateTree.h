#pragma once
#include "BinaryTreeNode.h"
#include <iostream>

using namespace std;

class EvaluateTree
{
public:
	EvaluateTree();
	~EvaluateTree();
	void get_suff(string suff);		//得到后缀表达式
	double output();				//输出计算结果
	int judge_mistake();			//判断是否有错误

private:
	char op_[100];					//存操作符
	double num_[100];				//存数字
	string suff_;					//输入的后缀表达式
	int root_, tr_;					//root_树根结点
	int stack_[100];				//栈用于生成树
	BinaryTreeNode node[100];		//二叉树节点类
	int total;						//操作符和数字的总数，主要用于处理单数字情况
	double result;					//计算结果
	void seperate_op_and_num();		//分割后缀表达式操作符和数字
	void create_tree();				//利用栈生成树
	void caculate();				//树计算
	int mistake;					//记录错误
};

