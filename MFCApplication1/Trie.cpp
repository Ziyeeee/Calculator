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

	//以上部分为初始化str对应的函数名和常数,以及初始所有tree和flag节点赋值为0

	initial();
}

int Trie::search(string str)
{
	int k;
	int root = 0;
	for (int i = 0; i < str.length(); i++)
	{
		k = str[i] - 'a';
		if (!tree[root][k])						//该字符串在树中找不到,返回0代表这个字符串非法
			return 0;
		root = tree[root][k];
	}
	return flag[root];							//返回这个字符串对应trie树中字符串编号90
}


void Trie::initial()
{
	for (int i = 0; i < 26; i++)					//常数和函数总共26个
	{
		int root = insert(str[i]);					//对树中插入字符串,返回字符串最后一个字母对应的节点编号
		flag[root] = i + 1;							//对该节点标记为第i+1个在trie树中的单词
	}
}

int Trie::insert(string str)						//插入函数,返回字符串最后一个字母对应的节点编号
{
	int root = 0;
	int k;
	for (int i = 0; i < str.length(); i++)
	{
		k = str[i] - 'a';
		if (!tree[root][k])							//如果目前节点未被拓展,加入trie树
			tree[root][k] = ++tot;
		root = tree[root][k];						//root为末尾字母节点编号
	}
	return root;
}
