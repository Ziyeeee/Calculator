#pragma once
#include<iostream>
using namespace std;
class Trie
{
public:
	~Trie();
	Trie();
	int search(string str);		//Ѱ��str��Ӧtire���Ľ��
private:
	int tree[100][26];			//�������������,���������������ӽڵ��a��z��26��
	int flag[100];				//�жϸý���Ƿ�Ϊ�������һ����ĸ
	int tot;					//��¼�������
	string str[26];				//�洢������ͺ�����
	void initial();				//��ʼ��������
	int insert(string str);		//���������
};


