#pragma once
#include "BinaryTreeNode.h"
#include <iostream>

using namespace std;

class EvaluateTree
{
public:
	EvaluateTree();
	~EvaluateTree();
	void get_suff(string suff);		//�õ���׺���ʽ
	double output();				//���������
	int judge_mistake();			//�ж��Ƿ��д���

private:
	char op_[100];					//�������
	double num_[100];				//������
	string suff_;					//����ĺ�׺���ʽ
	int root_, tr_;					//root_�������
	int stack_[100];				//ջ����������
	BinaryTreeNode node[100];		//�������ڵ���
	int total;						//�����������ֵ���������Ҫ���ڴ����������
	double result;					//������
	void seperate_op_and_num();		//�ָ��׺���ʽ������������
	void create_tree();				//����ջ������
	void caculate();				//������
	int mistake;					//��¼����
};

