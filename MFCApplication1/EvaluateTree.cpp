#include "pch.h"
#include "EvaluateTree.h"
#include <iostream>
#include "BinaryTreeNode.h"
#include <stdlib.h>
#include <math.h>
const int NUL = -1;

using namespace std;

EvaluateTree::EvaluateTree()
{
	mistake = 0;		//Ĭ���޴���
}


EvaluateTree::~EvaluateTree()
{
}

void EvaluateTree::get_suff(string suff)
{
	suff_ = suff;
}

void EvaluateTree::seperate_op_and_num()
{
	unsigned int i;
	int j, k, op_num = 0;
	double num_tmp, num_int, num_decimal;
	for (i = 0, k = 0; i < suff_.length(); i++)
	{
		num_tmp = 0;
		num_int = 0;		//��������
		num_decimal = 0;	//С������
		while (suff_[i] >= '0' && suff_[i] <= '9')
		{
			num_int = num_int * 10 + (double)(suff_[i] - 48);
			i++;
		}
		if (suff_[i] == '.')
		{
			i++;
			j = 0;
			while (suff_[i] >= '0' && suff_[i] <= '9')
			{
				num_decimal += pow(10, --j) * (double)(suff_[i] - 48);
				i++;
			}
		}
		if (suff_[i] == '$')
		{
			num_tmp = num_decimal + num_int;
			num_[k] = num_tmp;
			op_[k++] = '#';						//���ֲ��ֵĲ���������Ϊ��#��
			continue;
		}
		if (strchr("+-*/^%", suff_[i]) != NULL)
		{
			op_[k++] = suff_[i];
			op_num++;
		}
	}
	total = k;
	if (k != op_num * 2 + 1)
	{
		mistake = 4;
	}
}

void EvaluateTree::create_tree()
{
	if (mistake)
		return;
	int top = 0;
	for (int i = 0; i < total; i++)
	{
		if (total == 1)				//������������Ĵ���ʽ
		{
			node[i].set_node(op_[i], num_[i]);
			root_ = i;
			break;
		}
		if (strchr("+-*/^%", op_[i]) != NULL)		//Ŀǰ���жϵ�Ϊ������
		{
			node[i].set_node(op_[i], num_[i]);
			tr_ = i;
			node[tr_].allocate_rch(stack_[--top]);
			node[tr_].allocate_lch(stack_[--top]);
			stack_[top++] = tr_;					//������֮ǰ����ջ��Ԫ�س�Ϊ�����������Һ��ӣ���󽫲�������ջ
		}
		else
		{
			node[i].set_node(op_[i], num_[i]);
			tr_ = i;
			stack_[top++] = tr_;					//���ֽ�����������㲢��ջ
		}
	}
	if (total != 1)
		root_ = stack_[--top];						//���ø����
}

void EvaluateTree::caculate()
{
	if (mistake)
		return;
	while (node[root_].op_ != '#')
	{
		int tr = root_;
		while (node[node[tr].lch].op_ != '#' || node[node[tr].rch].op_ != '#')
		{
			if (node[node[tr].lch].op_ != '#')
				tr = node[tr].lch;
			else
				tr = node[tr].rch;
		}
		switch (node[tr].op_)
		{
		case '+':
		{
			node[tr].num_ = node[node[tr].lch].num_ + node[node[tr].rch].num_;
			node[tr].lch = NUL;
			node[tr].rch = NUL;
			node[tr].op_ = '#';
			break;
		}
		case '-':
		{
			node[tr].num_ = node[node[tr].lch].num_ - node[node[tr].rch].num_;
			node[tr].lch = NUL;
			node[tr].rch = NUL;
			node[tr].op_ = '#';
			break;
		}
		case '*':
		{
			node[tr].num_ = node[node[tr].lch].num_ * node[node[tr].rch].num_;
			node[tr].lch = NUL;
			node[tr].rch = NUL;
			node[tr].op_ = '#';
			break;
		}
		case '/':
		{
			if (!node[node[tr].rch].num_)
				mistake = 1;					//���ֳ������
			node[tr].num_ = node[node[tr].lch].num_ / node[node[tr].rch].num_;
			node[tr].lch = NUL;
			node[tr].rch = NUL;
			node[tr].op_ = '#';
			break;
		}
		case '^':
		{
			node[tr].num_ = pow(node[node[tr].lch].num_, node[node[tr].rch].num_);
			node[tr].lch = NUL;
			node[tr].rch = NUL;
			node[tr].op_ = '#';
			break;
		}
		case '%':
		{
			if ((int)node[node[tr].lch].num_ != node[node[tr].lch].num_ || (int)node[node[tr].rch].num_ != node[node[tr].rch].num_)
				mistake = 2;
			node[tr].num_ = (int)node[node[tr].lch].num_ % (int)node[node[tr].rch].num_;
			node[tr].lch = NUL;
			node[tr].rch = NUL;
			node[tr].op_ = '#';
			break;
		}
		default:
			break;
		}
	}
	result = node[root_].num_;
}

double EvaluateTree::output()
{
	seperate_op_and_num();
	create_tree();
	caculate();
	if (mistake)
		return 0;
	return result;
}

int EvaluateTree::judge_mistake()
{
	return mistake;
}

