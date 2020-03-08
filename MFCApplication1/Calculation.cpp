#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <sstream>
#include <math.h>
#include <map>
#include "Calculation.h"
#include "MyStack.h"
#include "EvaluateTree.h"

using namespace std;
map<char, int> priorityOfOperator;

Calculation::Calculation()
{
}


Calculation::~Calculation()
{
}

void Calculation::Initial()
{
	start = 0;									//�������ʽ'('λ��
	end = 0;									//�������ʽ')'λ��
	lengthOfOperator = 0;						//�������ʽ����
	memset(tempResult, 0, sizeof(char)*256);	//�����������ʱ���
	postfix = "";								//��׺���ʽ
	subInfix = "";								//�����ڵ���׺���ʽ
	doubleResult = 0;							//double���ֵ
	subDoubleResult = 0;						//����׺���ʽ���ֵ
	stringResult = "";							//string���ֵ
	tempInfixL = "";							//����ǰ������׺���ʽ
	tempInfixR = "";							//�����������׺���ʽ
	mistake = 0;								//�Ƿ���ڳ������
	stringreturn = "";							//��󷵻ص�stringֵ
}

/*����Ϊstring����׺���ʽ*/
void Calculation::InInfix(string inString)
{
	infix = inString;
}

/*�ݹ鴦���֣����Ϊdouble���͵Ľ����*/
double Calculation::OutAns()
{
	mistake = 0;
	/*��׺���ʽת��׺���ʽ���Ժ�׺���ʽ���м���*/
	InfixToPostfixAndCalculate();
	return doubleResult;
}

/*���㣬���ݱ��ʽ�Ƿ���ȷ������*/
string Calculation::OutAnsStr()
{
	Initial();						//��ʼ��
	InfixToPostfixAndCalculate();	//��׺���ʽת��׺���ʽ���Ժ�׺���ʽ���м���
	switch (mistake)
	{
	case 0:
	{
		stringreturn = DoubleToString(doubleResult);	//������ȷ
		break;
	}
	case 1:
	{
		stringreturn = "Divided by zero!";	//�������
		break;
	}
	case 2:
	{
		stringreturn = "The parameters of mod function are not integer!";	//��С������
		break;
	}
	case 3:
	{
		stringreturn = "Definition domain is not right!";	//���������
		break;
	}
	case 4:
	{
		stringreturn = "Invalid expression!";	//���ʽ����
		break;
	}
	default:
		break;
	}
	return stringreturn;
}

/*���ݴ�����Ϣ*/
int Calculation::Judge()
{
	return mistake;
}

/*��׺���ʽת��׺���ʽ���Ժ�׺���ʽ���м���*/
void Calculation::InfixToPostfixAndCalculate()
{
	Calculation SubCalculate;
	int i = 0;
	char var;

	string tmp = "";

	MyStack Stack;

	/*���ö�Ԫ��������ȼ�*/
	priorityOfOperator['('] = 0;
	priorityOfOperator['+'] = priorityOfOperator['-'] = 1;
	priorityOfOperator['*'] = priorityOfOperator['/'] = priorityOfOperator['%'] = 2;
	priorityOfOperator['^'] = 3; 

	while (i < infix.length())
	{
		if (infix[0] == '-')			//���ʽ��'-opnum'��ʼ��ת��Ϊ0-opnum
		{
			infix = '0' + infix;
		}
		if (infix[i] == '(')			//���������ţ�������ջ
		{
			Stack.push(infix[i]);
		}
		else if (infix[i] == ')')		//���������ţ�ִ�г�ջ�������������׺���ʽ��ֱ����������������
		{
			if (Stack.isEmpty())		//ջΪ�գ���δ�ҵ������ţ�˵�����ʽ����
			{
				mistake = 4;
				return;
			}
			else						//ִ�г�ջ�������������׺���ʽ��ֱ����������������
			{
				var = Stack.pop();
				while (var != '(')
				{
					if (!Stack.isEmpty())
					{
						postfix += var;
						var = Stack.pop();
					}
					else				//ջΪ�գ���δ�ҵ������ţ�˵�����ʽ����
					{
						mistake = 4;
						return;
					}
				}
			}
		}
		else if ((infix[i] >= '0' && infix[i] <= '9') || infix[i] == '.')	//������������ֱ�����
		{
			postfix += infix[i];
			if (infix[i + 1] > '9' || (infix[i + 1] < '0' && infix[i + 1] != '.'))
			{
				postfix += '$';			//������ĩβ��'$'��ǽ���
			}
		}
		else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/' || infix[i] == '^' || infix[i] == '%')		//�����������������������ȼ����ڻ���ڸ��������ջ��Ԫ�أ�Ȼ�󽫸��������ջ
		{
			if (infix[i - 1] == '(' && infix[i] == '-')		//��������'(-opnum)'��ת��Ϊ'(0-opnum)'
			{
				postfix += "0$";
			}
			while (!Stack.isEmpty() && priorityOfOperator[infix[i]] <= priorityOfOperator[Stack.topElement()])
			{
				postfix += Stack.pop();
			}
			Stack.push(infix[i]);
		}
		else if (infix[i] == ' ')
		{
			i++;
			continue;
		}
		else							//����һԪ����
		{
			while (infix[i] != '(' && infix[i] != '+' && infix[i] != '-' && infix[i] != '*' && infix[i] != '/' && infix[i] != '^' && infix[i] != '%' && infix[i] != ')' && i < infix.length() )
			{
				tmp = tmp + infix[i];
				i++;
			}
			switch (tr.search(tmp))
			{
			case 1:
			{
				lengthOfOperator = 3;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				if (isDegree)	//Ϊ�Ƕ���
				{
					subDoubleResult = subDoubleResult / 180 * 3.141592653589793;	//�Ƕ�ת��Ϊ����
				}
				doubleResult = sin(subDoubleResult);	//���ú������������������ֵ��double��
				mistake = SubCalculate.Judge();	//���Ƿ�������һֱ���ݵ����ϲ�
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 2:
			{
				lengthOfOperator = 3;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				if (isDegree)	//Ϊ�Ƕ���
				{
					subDoubleResult = subDoubleResult / 180 * 3.141592653589793;	//�Ƕ�ת��Ϊ����
				}
				doubleResult = cos(subDoubleResult);	//���ú������������������ֵ��double��
				mistake = SubCalculate.Judge();	//���Ƿ�������һֱ���ݵ����ϲ�
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 3:
			{
				lengthOfOperator = 3;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				if (isDegree)	//Ϊ�Ƕ���
				{
					subDoubleResult = subDoubleResult / 180 * 3.141592653589793;	//�Ƕ�ת��Ϊ����
				}
				if (cos(subDoubleResult) != 0)	//�ж϶�����δ����������
				{
					doubleResult = tan(subDoubleResult);
				}
				else							//�ж϶����򣬳���������
				{
					doubleResult = INFINITY;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)	//�����ڲ��Ƿ����
				{
					mistake = SubCalculate.Judge();	//�����ڲ������������ȼ����ߣ�����ԭ���Ĵ�����Ϣ
				}
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 4:
			{
				lengthOfOperator = 3;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				if (isDegree)	//Ϊ�Ƕ���
				{
					subDoubleResult = subDoubleResult / 180 * 3.141592653589793;	//�Ƕ�ת��Ϊ����
				}
				if (cos(subDoubleResult) == 0)	//�ж϶�����δ����������
				{
					doubleResult = 0;
				}
				else if (tan(subDoubleResult) != 0)	//�ж϶�����δ����������
				{
					doubleResult = 1 / tan(subDoubleResult);
				}
				else								//�ж϶����򣬳���������
				{
					doubleResult = INFINITY;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//�����ڲ��Ƿ����
				{
					mistake = SubCalculate.Judge();	//�����ڲ������������ȼ����ߣ�����ԭ���Ĵ�����Ϣ
				}
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 5:
			{
				lengthOfOperator = 2;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				if (subDoubleResult <= 1 && subDoubleResult >= -1)	//�ж϶�����δ����������
				{
					doubleResult = asin(subDoubleResult);
					if (isDegree)	//�Ƕ���
					{
						doubleResult = doubleResult / 3.141592653589793 * 180;	//����ת��Ϊ�Ƕ�
					}
				}
				else												//�ж϶����򣬳���������
				{
					doubleResult = 0;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//�����ڲ��Ƿ����
				{
					mistake = SubCalculate.Judge();	//�����ڲ������������ȼ����ߣ�����ԭ���Ĵ�����Ϣ
				}
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 6:
			{
				lengthOfOperator = 2;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				if (subDoubleResult <= 1 && subDoubleResult >= -1)	//�ж϶�����δ����������
				{
					doubleResult = acos(subDoubleResult);
					if (isDegree)	//�Ƕ���
					{
						doubleResult = doubleResult / 3.141592653589793 * 180;	//����ת��Ϊ�Ƕ�
					}
				}
				else												//�ж϶����򣬳���������
				{
					doubleResult = 0;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//�����ڲ��Ƿ����
				{
					mistake = SubCalculate.Judge();	//�����ڲ������������ȼ����ߣ�����ԭ���Ĵ�����Ϣ
				}
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 7:
			{
				lengthOfOperator = 2;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				doubleResult = atan(subDoubleResult);
				if (isDegree)	//�Ƕ���
				{
					doubleResult = doubleResult / 3.141592653589793 * 180;	//����ת��Ϊ�Ƕ�
				}
				mistake = SubCalculate.Judge();	//���Ƿ�������һֱ���ݵ����ϲ�
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 8:
			{
				lengthOfOperator = 3;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				if (subDoubleResult > 0)					//�ж϶�����δ����������
				{
					doubleResult = log10(subDoubleResult);
				}
				else										//�ж϶����򣬳���������
				{
					doubleResult = 0;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//�����ڲ��Ƿ����
				{
					mistake = SubCalculate.Judge();	//�����ڲ������������ȼ����ߣ�����ԭ���Ĵ�����Ϣ
				}
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 9:
			{
				lengthOfOperator = 2;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				doubleResult = sinh(subDoubleResult);	//���ú������������������ֵ��double��
				mistake = SubCalculate.Judge();	//���Ƿ�������һֱ���ݵ����ϲ�
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 10:
			{
				lengthOfOperator = 2;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				doubleResult = cosh(subDoubleResult);	//���ú������������������ֵ��double��
				mistake = SubCalculate.Judge();	//���Ƿ�������һֱ���ݵ����ϲ�
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 11:
			{
				lengthOfOperator = 2;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				doubleResult = tanh(subDoubleResult);	//���ú������������������ֵ��double��
				mistake = SubCalculate.Judge();	//���Ƿ�������һֱ���ݵ����ϲ�
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 12:
			{
				lengthOfOperator = 2;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				if (subDoubleResult > 0)					//�ж϶�����δ����������
				{
					doubleResult = log(subDoubleResult);
				}
				else										//�ж϶�����δ����������
				{
					doubleResult = 0;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//�����ڲ��Ƿ����
				{
					mistake = SubCalculate.Judge();	//�����ڲ������������ȼ����ߣ�����ԭ���Ĵ�����Ϣ
				}
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 13:
			{
				lengthOfOperator = 4;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				if (subDoubleResult > 0)					//�ж϶�����δ����������	
				{
					doubleResult = sqrt(subDoubleResult);
				}
				else										//�ж϶����򣬳���������
				{
					doubleResult = 0;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//�����ڲ��Ƿ����
				{
					mistake = SubCalculate.Judge();	//�����ڲ������������ȼ����ߣ�����ԭ���Ĵ�����Ϣ
				}
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 14:
			{
				lengthOfOperator = 3;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				doubleResult = fabs(subDoubleResult);	//���ú������������������ֵ��double��
				mistake = SubCalculate.Judge();	//���Ƿ�������һֱ���ݵ����ϲ�
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 15:	
			{
				lengthOfOperator = 3;	//��¼��������ȣ����ڷָ�滻�ַ���
				MatchBrackets(i);		//����ƥ��
				if (mistake)			//��������ֹͣ��һ�����㣬����
					return;
				SubCalculate.InInfix(subInfix);	//��ȡ�������ڵĲ��֣���׺���ʽ��
				subDoubleResult = SubCalculate.OutAns();	//���㺯���ڲ�����ֵ
				doubleResult = pow(10, subDoubleResult);	//���ú������������������ֵ��double��
				mistake = SubCalculate.Judge();	//���Ƿ�������һֱ���ݵ����ϲ�
				ReplaceExpression();	//��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 16:					
			{
				lengthOfOperator = 1;
				stringResult = "3.1415926535898";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 17:					
			{
				lengthOfOperator = 2;
				stringResult = "9.80665";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 18:
			{
				lengthOfOperator = 2;
				stringResult = "8.988*10^9";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 19:
			{
				lengthOfOperator = 2;
				stringResult = "0.02241383";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����'
				break;
			}
			case 20:
			{
				lengthOfOperator = 2;
				stringResult = "1.380662*10^(-23)";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 21:
			{
				lengthOfOperator = 2;
				stringResult = "8.3144";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 22:
			{
				lengthOfOperator = 2;
				stringResult = "6.6720*10^(-11)";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 23:
			{
				lengthOfOperator = 2;
				stringResult = "6.022045*10^23";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 24:
			{
				lengthOfOperator = 2;
				stringResult = "2.99792458*10^8";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 25:
			{
				lengthOfOperator = 2;
				stringResult = "8.854187818*10^(-12)";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 26:
			{
				lengthOfOperator = 1;
				stringResult = "2.7182818284590452353";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
		        tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i��ֵ�ָ�������ǰ�Ĳ�����
				break;
			}
			case 0:							//�ַ�������Trie���зǷ����ʽ
			{
				mistake = 4;
				return;
				break;
			}

			default:
				break;
			/*�����滻*/
			}
		}
		i++;
		tmp = "";							//��¼���ʽ�еĺ���������tmp���
	}
	while (!Stack.isEmpty())			//��ջ��ʣ���������ε�����׺���ʽ
	{
		if (Stack.topElement() != '(')	//ջ�в����������ţ���ջ��ʣ���������ε�����׺���ʽ
		{
			postfix += Stack.pop();
		}
		else							//ջ�д��������ţ����ʽ����
		{
			mistake = 4;
			return;
		}
	}
	/*��׺���ʽ��ֵ*/
	if (!mistake)							//���ʽ����
	{
		EvaluateTree Tree;
		Tree.get_suff(postfix);				//����string��׺���ʽ��ֵ
		doubleResult = Tree.output();		//���double�Ľ��
		mistake = Tree.judge_mistake();		//�ж��Ƿ�������
	}
}

/*��ջʵ������ƥ��*/
void Calculation::MatchBrackets(int startPosition)
{
	char stack[100];		//ջ
	int top = 0;			//ջ��ָ��
	int flag = 1;			//ƥ�������־
	start = startPosition;	//��һ��'('��λ��
	end = startPosition;	//��ʼ�ӵ�һ��'('��ʼ������ʱָ�����һ��')'��λ��
	while (flag && end <= infix.length())
	{
		if (infix[end] == '(')		//����'('��'('��ջ
		{
			stack[top] = '(';
			top++;
		}
		else if (infix[end] == ')')	//����')'����ջ
		{
			stack[top] = '0';
			top--;
		}
		if (top == 0)	//ջ�ٴ�Ϊ�ջ��������׺���ʽĩβ
		{
			flag = 0;				//ƥ�����
		}
		else
		{
			end++;					//�ж���һ��Ԫ��
		}
	}
	if (top)		//ջ�л���δƥ�䵽�������ţ����ʽ����
		mistake = 4;
	subInfix = infix.substr(startPosition + 1, end - startPosition - 1);	//��ȡ�������ڵĲ��֣���׺���ʽ��
}


/*��double�Ľ��ת��Ϊstring�����뵽ԭ��׺���ʽ��*/
void Calculation::ReplaceExpression()
{
	stringResult = DoubleToString(doubleResult);//stringתdouble
	if (start != lengthOfOperator)				//���ʽ���Ժ�����ͷ
	{
		tempInfixL = infix.substr(0, start - lengthOfOperator);	//��ȡ�������ʽ֮ǰ�ı��ʽ
	}
	else										//���ʽ�Ժ�����ʼ
	{
		tempInfixL = "";						//����֮ǰ�ı��ʽΪ��
	}
	if (end != infix.length())					//���ʽ���Ժ�����β
	{
		tempInfixR = infix.substr(end + 1);		//��ȡ�������ʽ֮��ı��ʽ
	}
	else										//���ʽ�Ժ�����β
	{
		tempInfixR = "";						//����֮��ı��ʽΪ��
	}
	infix = tempInfixL + '(' + stringResult + ')' + tempInfixR;	//ƴ�ӱ��ʽ
}

string Calculation::DoubleToString(double numDou)
{
	fstream file;

	stringstream StrToInt;
	string numStr;				//string���ֵ
	int powInt, lengthOfZero;	//��ѧ������ָ������λ�������0��λ��
	unsigned int positionOfe;	//e���ֵ�λ��
	file.open("num.txt", ios::out | ios::in | ios::trunc);	//���ļ������
	file << setprecision(15) << numDou << endl;	//double��������ļ������������15λ��Ч����
	file.seekg(0);				//�ļ�ָ��ָ���ļ���
	file >> numStr;				//��string��ʽ�������
	file.close();				//�ر��ļ�
	if (numDou > 999999999999999)	//�������ӿ�ѧ������ת��Ϊ������ʾ
	{
		positionOfe = numStr.find('e', 0);	//��λe��λ��
		StrToInt << numStr.substr(positionOfe + 1);	//��ѧ������ָ������
		StrToInt >> powInt;
		if (numStr.find('.', 0) == 1)	//����ΪС��
		{
			numStr = numStr.substr(0, 1) + numStr.substr(2, positionOfe - 2);	//�ص�С����
		}
		else							//����Ϊ����
		{
			numStr = numStr.substr(0, 1);
		}
		lengthOfZero = 15 - numStr.length() + powInt - 15 + 1;	//���0��λ��
		numStr.append(lengthOfZero, '0');	//�0
	}
	else if (numStr.find('e', 0) <= 16)	//��С���ӿ�ѧ������ת��Ϊ������ʾ
	{
		positionOfe = numStr.find('e', 0);	//��λe��λ��
		StrToInt << numStr.substr(positionOfe + 2);	//��ѧ������ָ������
		StrToInt >> powInt;
		if (numStr.find('.', 0) == 1)	//����ΪС��
		{
			numStr = numStr.substr(0, 1) + numStr.substr(2, positionOfe - 2);	//�ص�С����
		}
		else							//����Ϊ����
		{
			numStr = numStr.substr(0, 1);
		}
		lengthOfZero = powInt - 1;		//���0��λ��
		while (lengthOfZero != 0)		//�0
		{
			numStr = '0' + numStr;
			lengthOfZero--;
		}
		numStr = "0." + numStr;			//�0.
	}
	return numStr;
}
