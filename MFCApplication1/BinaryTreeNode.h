#pragma once
class BinaryTreeNode
{
public:
	BinaryTreeNode();
	~BinaryTreeNode();
	char op_;								//�洢������,�����������Ĭ��Ϊ'#'
	double num_;							//��ȡ����
	int lch;								//����
	int rch;								//�ҽ��
	void set_node(char op, double num);		//����������㸳ֵ
	void allocate_rch(int r);				//�����Һ���
	void allocate_lch(int l);				//��������
private:

};


