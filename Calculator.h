#ifndef C_H
#define C_H
#include<iostream>
#include<stack>
#include<string>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<algorithm>

using namespace std;

class Calculator
{
public:
    Calculator();
    ~Calculator();
    Calculator(string s);
    int priority(char x);
    double calcu(double d1,double d2,char t);

    bool judge(string s);         //判断表达式是否合法
    string dealneg(string s);     //处理字符串里的负数 ，补0.
    void str_to_inf(string str);  //字符串转化为中缀表达式
    void inf_to_suf();            //中缀表达式转后缀表达式
    double result();              //后缀表达式计算最终结果
    void run(string s);

private:
    struct nodes{
        char op;           //运算符
        double num;          //数字
        int sign;            //标记，存数字用num,标记为1，存运算符用Oper,标记为2
    };

    //string s;                     //读入字符串
    int q=0;                      //记录infix数组大小
    stack<char> tool;             //保存运算符
    nodes infix[10000000];        //储存中缀表达式
    vector<nodes> suffix;        //保存后缀表达式
};

#endif // C_H
