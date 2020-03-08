#include "Trie.h"
#include<iostream>

using namespace std;

Trie::~Trie()
{

}

Trie::Trie()
{
	tot = 0;
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 26; j++)
			tree[i][j] = 0;
	}
	for (int i = 0; i < 50; i++)
		flag[i] = 0;

	str[0] = "sin";
	str[1] = "cos";
	str[2] = "tan";
	str[3] = "cot";
	str[4] = "as";
	str[5] = "ac";
	str[6] = "at";
	str[7] = "log";
	str[8] = "sh";
	str[9] = "ch";
	str[10] = "th";
	str[11] = "ln";
	str[12] = "sqrt";
	str[13] = "abs";
	str[14] = "exp";
	str[15] = "p";
	str[16] = "gg";
	str[17] = "ef";
	str[18] = "vm";
	str[19] = "bc";
	str[20] = "mg";
	str[21] = "gr";
	str[22] = "na";
	str[23] = "ls";
	str[24] = "dc";
	str[25] = "e";

	//���ϲ���Ϊ��ʼ��str��Ӧ�ĺ������ͳ���,�Լ���ʼ����tree��flag�ڵ㸳ֵΪ0

	initial();
}

int Trie::search(string str)
{
	int k;
	int root = 0;
	for (int i = 0; i < str.length(); i++)
	{
		k = str[i] - 'a';
		if (!tree[root][k])						//���ַ����������Ҳ���,����0��������ַ����Ƿ�
			return 0;
		root = tree[root][k];
	}
	return flag[root];							//��������ַ�����Ӧtrie�����ַ������90
}


void Trie::initial()
{
	for (int i = 0; i < 26; i++)					//�����ͺ����ܹ�26��
	{
		int root = insert(str[i]);					//�����в����ַ���,�����ַ������һ����ĸ��Ӧ�Ľڵ���
		flag[root] = i + 1;							//�Ըýڵ���Ϊ��i+1����trie���еĵ���
	}
}

int Trie::insert(string str)						//���뺯��,�����ַ������һ����ĸ��Ӧ�Ľڵ���
{
	int root = 0;
	int k;
	for (int i = 0; i < str.length(); i++)
	{
		k = str[i] - 'a';
		if (!tree[root][k])							//���Ŀǰ�ڵ�δ����չ,����trie��
			tree[root][k] = ++tot;
		root = tree[root][k];						//rootΪĩβ��ĸ�ڵ���
	}
	return root;
}
