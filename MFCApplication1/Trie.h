#pragma once
#include<iostream>
using namespace std;
class Trie
{
public:
	~Trie();
	Trie();
	int search(string str);		//寻找str对应tire树的结点
private:
	int tree[100][26];			//横坐标代表结点编号,纵坐标代表连向的子节点从a到z共26个
	int flag[100];				//判断该结点是否为单词最后一个字母
	int tot;					//记录结点总数
	string str[26];				//存储常数表和函数表
	void initial();				//初始化建立树
	int insert(string str);		//插入树结点
};


