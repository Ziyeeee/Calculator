#pragma once
#include <iostream>
#include "Trie.h"
using namespace std;

class Calculation
{
public:
	Calculation();
	~Calculation();
	void InInfix(string inString);	//������׺���ʽ
	double OutAns();				//���double���ֵ
	string OutAnsStr();				//���string���ֵ
	int Judge();					//�Ƿ��ֳ������
	bool isDegree;					//�ԽǶȽ��м���
	Trie tr;						//trie��3
private:
	int start, end;					//�������ʽ'('��')'λ��
	int lengthOfOperator;			//�������ʽ����
	char tempResult[256];			//�����������ʱ���
	string infix;					//��׺���ʽ
	string postfix;					//��׺���ʽ
	string subInfix;				//�����ڵ���׺���ʽ
	double doubleResult;			//double���ֵ
	double subDoubleResult;			//����׺���ʽ���ֵ
	string stringResult;			//string���ֵ
	string tempInfixL, tempInfixR;	//����ǰ�������׺���ʽ
	int mistake;					//�Ƿ���ڳ������
	string stringreturn;			//��󷵻ص�stringֵ
	
	void Initial();
	void InfixToPostfixAndCalculate();		//��׺���ʽת��׺���ʽ���Ժ�׺���ʽ���м���
	void MatchBrackets(int startPosition);	//����ƥ��
	void ReplaceExpression();				//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
	string DoubleToString(double numDou);
};

