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
	start = 0;									//函数表达式'('位置
	end = 0;									//函数表达式')'位置
	lengthOfOperator = 0;						//函数表达式长度
	memset(tempResult, 0, sizeof(char)*256);	//函数运算的暂时结果
	postfix = "";								//后缀表达式
	subInfix = "";								//函数内的中缀表达式
	doubleResult = 0;							//double结果值
	subDoubleResult = 0;						//子中缀表达式结果值
	stringResult = "";							//string结果值
	tempInfixL = "";							//函数前的子中缀表达式
	tempInfixR = "";							//函数后的子中缀表达式
	mistake = 0;								//是否存在除零错误
	stringreturn = "";							//最后返回的string值
}

/*输入为string的中缀表达式*/
void Calculation::InInfix(string inString)
{
	infix = inString;
}

/*递归处理部分：输出为double类型的结果，*/
double Calculation::OutAns()
{
	mistake = 0;
	/*中缀表达式转后缀表达式，对后缀表达式进行计算*/
	InfixToPostfixAndCalculate();
	return doubleResult;
}

/*计算，根据表达式是否正确分配结果*/
string Calculation::OutAnsStr()
{
	Initial();						//初始化
	InfixToPostfixAndCalculate();	//中缀表达式转后缀表达式，对后缀表达式进行计算
	switch (mistake)
	{
	case 0:
	{
		stringreturn = DoubleToString(doubleResult);	//计算正确
		break;
	}
	case 1:
	{
		stringreturn = "Divided by zero!";	//除零错误
		break;
	}
	case 2:
	{
		stringreturn = "The parameters of mod function are not integer!";	//对小数求余
		break;
	}
	case 3:
	{
		stringreturn = "Definition domain is not right!";	//定义域错误
		break;
	}
	case 4:
	{
		stringreturn = "Invalid expression!";	//表达式错误
		break;
	}
	default:
		break;
	}
	return stringreturn;
}

/*传递错误信息*/
int Calculation::Judge()
{
	return mistake;
}

/*中缀表达式转后缀表达式，对后缀表达式进行计算*/
void Calculation::InfixToPostfixAndCalculate()
{
	Calculation SubCalculate;
	int i = 0;
	char var;

	string tmp = "";

	MyStack Stack;

	/*设置二元运算符优先级*/
	priorityOfOperator['('] = 0;
	priorityOfOperator['+'] = priorityOfOperator['-'] = 1;
	priorityOfOperator['*'] = priorityOfOperator['/'] = priorityOfOperator['%'] = 2;
	priorityOfOperator['^'] = 3; 

	while (i < infix.length())
	{
		if (infix[0] == '-')			//表达式以'-opnum'开始，转化为0-opnum
		{
			infix = '0' + infix;
		}
		if (infix[i] == '(')			//遇到左括号：将其入栈
		{
			Stack.push(infix[i]);
		}
		else if (infix[i] == ')')		//遇到右括号：执行出栈操作，输出到后缀表达式，直到弹出的是左括号
		{
			if (Stack.isEmpty())		//栈为空，还未找到左括号，说明表达式错误
			{
				mistake = 4;
				return;
			}
			else						//执行出栈操作，输出到后缀表达式，直到弹出的是左括号
			{
				var = Stack.pop();
				while (var != '(')
				{
					if (!Stack.isEmpty())
					{
						postfix += var;
						var = Stack.pop();
					}
					else				//栈为空，还未找到左括号，说明表达式错误
					{
						mistake = 4;
						return;
					}
				}
			}
		}
		else if ((infix[i] >= '0' && infix[i] <= '9') || infix[i] == '.')	//遇到操作数：直接输出
		{
			postfix += infix[i];
			if (infix[i + 1] > '9' || (infix[i + 1] < '0' && infix[i + 1] != '.'))
			{
				postfix += '$';			//在数字末尾用'$'标记结束
			}
		}
		else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/' || infix[i] == '^' || infix[i] == '%')		//遇到操作符：弹出所有优先级大于或等于该运算符的栈顶元素，然后将该运算符入栈
		{
			if (infix[i - 1] == '(' && infix[i] == '-')		//遇到负数'(-opnum)'，转化为'(0-opnum)'
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
		else							//遇到一元函数
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
				lengthOfOperator = 3;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				if (isDegree)	//为角度制
				{
					subDoubleResult = subDoubleResult / 180 * 3.141592653589793;	//角度转化为弧度
				}
				doubleResult = sin(subDoubleResult);	//调用函数自身，计算出函数的值（double）
				mistake = SubCalculate.Judge();	//将是否发生错误一直传递到最上层
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 2:
			{
				lengthOfOperator = 3;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				if (isDegree)	//为角度制
				{
					subDoubleResult = subDoubleResult / 180 * 3.141592653589793;	//角度转化为弧度
				}
				doubleResult = cos(subDoubleResult);	//调用函数自身，计算出函数的值（double）
				mistake = SubCalculate.Judge();	//将是否发生错误一直传递到最上层
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 3:
			{
				lengthOfOperator = 3;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				if (isDegree)	//为角度制
				{
					subDoubleResult = subDoubleResult / 180 * 3.141592653589793;	//角度转化为弧度
				}
				if (cos(subDoubleResult) != 0)	//判断定义域，未超出定义域
				{
					doubleResult = tan(subDoubleResult);
				}
				else							//判断定义域，超出定义域
				{
					doubleResult = INFINITY;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)	//函数内部是否出错
				{
					mistake = SubCalculate.Judge();	//函数内部出错，错误优先级更高，覆盖原来的错误信息
				}
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 4:
			{
				lengthOfOperator = 3;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				if (isDegree)	//为角度制
				{
					subDoubleResult = subDoubleResult / 180 * 3.141592653589793;	//角度转化为弧度
				}
				if (cos(subDoubleResult) == 0)	//判断定义域，未超出定义域
				{
					doubleResult = 0;
				}
				else if (tan(subDoubleResult) != 0)	//判断定义域，未超出定义域
				{
					doubleResult = 1 / tan(subDoubleResult);
				}
				else								//判断定义域，超出定义域
				{
					doubleResult = INFINITY;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//函数内部是否出错
				{
					mistake = SubCalculate.Judge();	//函数内部出错，错误优先级更高，覆盖原来的错误信息
				}
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 5:
			{
				lengthOfOperator = 2;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				if (subDoubleResult <= 1 && subDoubleResult >= -1)	//判断定义域，未超出定义域
				{
					doubleResult = asin(subDoubleResult);
					if (isDegree)	//角度制
					{
						doubleResult = doubleResult / 3.141592653589793 * 180;	//弧度转化为角度
					}
				}
				else												//判断定义域，超出定义域
				{
					doubleResult = 0;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//函数内部是否出错
				{
					mistake = SubCalculate.Judge();	//函数内部出错，错误优先级更高，覆盖原来的错误信息
				}
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 6:
			{
				lengthOfOperator = 2;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				if (subDoubleResult <= 1 && subDoubleResult >= -1)	//判断定义域，未超出定义域
				{
					doubleResult = acos(subDoubleResult);
					if (isDegree)	//角度制
					{
						doubleResult = doubleResult / 3.141592653589793 * 180;	//弧度转化为角度
					}
				}
				else												//判断定义域，超出定义域
				{
					doubleResult = 0;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//函数内部是否出错
				{
					mistake = SubCalculate.Judge();	//函数内部出错，错误优先级更高，覆盖原来的错误信息
				}
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 7:
			{
				lengthOfOperator = 2;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				doubleResult = atan(subDoubleResult);
				if (isDegree)	//角度制
				{
					doubleResult = doubleResult / 3.141592653589793 * 180;	//弧度转化为角度
				}
				mistake = SubCalculate.Judge();	//将是否发生错误一直传递到最上层
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 8:
			{
				lengthOfOperator = 3;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				if (subDoubleResult > 0)					//判断定义域，未超出定义域
				{
					doubleResult = log10(subDoubleResult);
				}
				else										//判断定义域，超出定义域
				{
					doubleResult = 0;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//函数内部是否出错
				{
					mistake = SubCalculate.Judge();	//函数内部出错，错误优先级更高，覆盖原来的错误信息
				}
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 9:
			{
				lengthOfOperator = 2;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				doubleResult = sinh(subDoubleResult);	//调用函数自身，计算出函数的值（double）
				mistake = SubCalculate.Judge();	//将是否发生错误一直传递到最上层
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 10:
			{
				lengthOfOperator = 2;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				doubleResult = cosh(subDoubleResult);	//调用函数自身，计算出函数的值（double）
				mistake = SubCalculate.Judge();	//将是否发生错误一直传递到最上层
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 11:
			{
				lengthOfOperator = 2;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				doubleResult = tanh(subDoubleResult);	//调用函数自身，计算出函数的值（double）
				mistake = SubCalculate.Judge();	//将是否发生错误一直传递到最上层
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 12:
			{
				lengthOfOperator = 2;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				if (subDoubleResult > 0)					//判断定义域，未超出定义域
				{
					doubleResult = log(subDoubleResult);
				}
				else										//判断定义域，未超出定义域
				{
					doubleResult = 0;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//函数内部是否出错
				{
					mistake = SubCalculate.Judge();	//函数内部出错，错误优先级更高，覆盖原来的错误信息
				}
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 13:
			{
				lengthOfOperator = 4;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				if (subDoubleResult > 0)					//判断定义域，未超出定义域	
				{
					doubleResult = sqrt(subDoubleResult);
				}
				else										//判断定义域，超出定义域
				{
					doubleResult = 0;
					mistake = 3;
				}
				if (SubCalculate.Judge() != 0)		//函数内部是否出错
				{
					mistake = SubCalculate.Judge();	//函数内部出错，错误优先级更高，覆盖原来的错误信息
				}
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 14:
			{
				lengthOfOperator = 3;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				doubleResult = fabs(subDoubleResult);	//调用函数自身，计算出函数的值（double）
				mistake = SubCalculate.Judge();	//将是否发生错误一直传递到最上层
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 15:	
			{
				lengthOfOperator = 3;	//记录运算符长度，便于分割、替换字符串
				MatchBrackets(i);		//括号匹配
				if (mistake)			//遇到错误，停止进一步运算，返回
					return;
				SubCalculate.InInfix(subInfix);	//截取出函数内的部分（中缀表达式）
				subDoubleResult = SubCalculate.OutAns();	//计算函数内部分数值
				doubleResult = pow(10, subDoubleResult);	//调用函数自身，计算出函数的值（double）
				mistake = SubCalculate.Judge();	//将是否发生错误一直传递到最上层
				ReplaceExpression();	//将double的结果转化为string并插入到原中缀表达式中
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 16:					
			{
				lengthOfOperator = 1;
				stringResult = "3.1415926535898";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 17:					
			{
				lengthOfOperator = 2;
				stringResult = "9.80665";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 18:
			{
				lengthOfOperator = 2;
				stringResult = "8.988*10^9";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 19:
			{
				lengthOfOperator = 2;
				stringResult = "0.02241383";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符'
				break;
			}
			case 20:
			{
				lengthOfOperator = 2;
				stringResult = "1.380662*10^(-23)";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 21:
			{
				lengthOfOperator = 2;
				stringResult = "8.3144";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 22:
			{
				lengthOfOperator = 2;
				stringResult = "6.6720*10^(-11)";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 23:
			{
				lengthOfOperator = 2;
				stringResult = "6.022045*10^23";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 24:
			{
				lengthOfOperator = 2;
				stringResult = "2.99792458*10^8";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 25:
			{
				lengthOfOperator = 2;
				stringResult = "8.854187818*10^(-12)";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
				tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 26:
			{
				lengthOfOperator = 1;
				stringResult = "2.7182818284590452353";
				tempInfixL = infix.substr(0, i - lengthOfOperator);
		        tempInfixR = infix.substr(i);
				infix = tempInfixL + stringResult + tempInfixR;
				i = i - lengthOfOperator - 1;	//i的值恢复到函数前的操作符
				break;
			}
			case 0:							//字符串不再Trie树中非法表达式
			{
				mistake = 4;
				return;
				break;
			}

			default:
				break;
			/*常数替换*/
			}
		}
		i++;
		tmp = "";							//记录表达式中的函数或常数的tmp清空
	}
	while (!Stack.isEmpty())			//将栈中剩余内容依次弹出后缀表达式
	{
		if (Stack.topElement() != '(')	//栈中不存在左括号，将栈中剩余内容依次弹出后缀表达式
		{
			postfix += Stack.pop();
		}
		else							//栈中存在左括号，表达式错误
		{
			mistake = 4;
			return;
		}
	}
	/*后缀表达式求值*/
	if (!mistake)							//表达式无误
	{
		EvaluateTree Tree;
		Tree.get_suff(postfix);				//输入string后缀表达式的值
		doubleResult = Tree.output();		//输出double的结果
		mistake = Tree.judge_mistake();		//判断是否发生错误
	}
}

/*用栈实现括号匹配*/
void Calculation::MatchBrackets(int startPosition)
{
	char stack[100];		//栈
	int top = 0;			//栈顶指针
	int flag = 1;			//匹配结束标志
	start = startPosition;	//第一个'('的位置
	end = startPosition;	//初始从第一个'('开始，结束时指向最后一个')'的位置
	while (flag && end <= infix.length())
	{
		if (infix[end] == '(')		//遇到'('，'('入栈
		{
			stack[top] = '(';
			top++;
		}
		else if (infix[end] == ')')	//遇到')'，出栈
		{
			stack[top] = '0';
			top--;
		}
		if (top == 0)	//栈再次为空或遍历到中缀表达式末尾
		{
			flag = 0;				//匹配结束
		}
		else
		{
			end++;					//判断下一个元素
		}
	}
	if (top)		//栈中还有未匹配到的左括号，表达式错误
		mistake = 4;
	subInfix = infix.substr(startPosition + 1, end - startPosition - 1);	//截取出函数内的部分（中缀表达式）
}


/*将double的结果转化为string并插入到原中缀表达式中*/
void Calculation::ReplaceExpression()
{
	stringResult = DoubleToString(doubleResult);//string转double
	if (start != lengthOfOperator)				//表达式不以函数表开头
	{
		tempInfixL = infix.substr(0, start - lengthOfOperator);	//截取函数表达式之前的表达式
	}
	else										//表达式以函数开始
	{
		tempInfixL = "";						//函数之前的表达式为空
	}
	if (end != infix.length())					//表达式不以函数结尾
	{
		tempInfixR = infix.substr(end + 1);		//截取函数表达式之后的表达式
	}
	else										//表达式以函数结尾
	{
		tempInfixR = "";						//函数之后的表达式为空
	}
	infix = tempInfixL + '(' + stringResult + ')' + tempInfixR;	//拼接表达式
}

string Calculation::DoubleToString(double numDou)
{
	fstream file;

	stringstream StrToInt;
	string numStr;				//string结果值
	int powInt, lengthOfZero;	//科学记数法指数部分位数、添加0的位数
	unsigned int positionOfe;	//e出现的位置
	file.open("num.txt", ios::out | ios::in | ios::trunc);	//打开文件并清空
	file << setprecision(15) << numDou << endl;	//double结果存入文件，输出流保留15位有效数字
	file.seekg(0);				//文件指针指向文件首
	file >> numStr;				//以string形式读出结果
	file.close();				//关闭文件
	if (numDou > 999999999999999)	//超大数从科学计数法转化为正常显示
	{
		positionOfe = numStr.find('e', 0);	//定位e的位置
		StrToInt << numStr.substr(positionOfe + 1);	//科学记数法指数部分
		StrToInt >> powInt;
		if (numStr.find('.', 0) == 1)	//基数为小数
		{
			numStr = numStr.substr(0, 1) + numStr.substr(2, positionOfe - 2);	//截掉小数点
		}
		else							//基数为整数
		{
			numStr = numStr.substr(0, 1);
		}
		lengthOfZero = 15 - numStr.length() + powInt - 15 + 1;	//需填补0的位数
		numStr.append(lengthOfZero, '0');	//填补0
	}
	else if (numStr.find('e', 0) <= 16)	//超小数从科学计数法转化为正常显示
	{
		positionOfe = numStr.find('e', 0);	//定位e的位置
		StrToInt << numStr.substr(positionOfe + 2);	//科学记数法指数部分
		StrToInt >> powInt;
		if (numStr.find('.', 0) == 1)	//基数为小数
		{
			numStr = numStr.substr(0, 1) + numStr.substr(2, positionOfe - 2);	//截掉小数点
		}
		else							//基数为整数
		{
			numStr = numStr.substr(0, 1);
		}
		lengthOfZero = powInt - 1;		//需填补0的位数
		while (lengthOfZero != 0)		//填补0
		{
			numStr = '0' + numStr;
			lengthOfZero--;
		}
		numStr = "0." + numStr;			//填补0.
	}
	return numStr;
}
