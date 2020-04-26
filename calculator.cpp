#include<iostream>
#include<vector>

#include <string>

#include "calculator.h"
#include <stdio.h>
#include<math.h>

using namespace std;

template<typename ElemType>

Calculator<ElemType>::Calculator()
{
    //栈的底point to空间's首地址
    s.base=new ElemType[STACK_INIT_SIZE];
    if(s.base==NULL){
        //出现error
        exit(0);
    }
    s.top=s.base;//top指向元素的下一个（栈顶），top中没有数据
    s.stackSize=STACK_INIT_SIZE;
}

//销毁栈，将内存空间释放
template<typename ElemType>
Calculator<ElemType>::~Calculator()
{
    delete []s.base;
}

template<typename ElemType>
void Calculator<ElemType>::Push(ElemType e)
{
    if(s.top-s.base>=s.stackSize){
        s.base=(ElemType *)realloc(s.base,(s.stackSize+STACKINCREMENT)*sizeof(ElemType));
        //        realloc申请一个新的空间，并将旧的内容copy到新的空间，也释放以前的空间

        if(s.base==NULL){
            // error

            exit(0);
        }
        else{
            s.top=s.base+s.stackSize;//因为重新分配了空间，所以重新设置栈顶
            s.stackSize=s.stackSize+STACKINCREMENT;//栈的最大容量变大了

        }


    }
    *(s.top)=e;
    s.top++;
}

template<typename ElemType>
bool Calculator<ElemType>::Pop(ElemType &e)
{
    if(s.top==s.base){
        return false;//空栈
    }
    else{
        e=*(--(s.top));

    }

    return 1;
}

//清空栈
template<typename ElemType>
void Calculator<ElemType>::clearStack()
{
    s.top=s.base;
}

//计算栈的元素数
template<typename ElemType>
int Calculator<ElemType>::StackLen()
{
    return s.top-s.base;
}


template<typename ElemType>
int Calculator<ElemType>:: Calculation(char Postfix[]){

    char str[MAXBUFFER];
    double a=0;
    double b=0;

    int i=0;
    int j=0;
    char c;

    for(j=0;Postfix[j]!='\0';j++)
    {
        c=Postfix[j];
        if(Postfix[j]!=' '){          //忽略空格
           str[j]=Postfix[j];
        }

        while ((Postfix[j]>=48 && Postfix[j]<=57)||Postfix[j]=='.') //输入的是数字
        {
            str[i]=Postfix[j];
            printf("str[%d]=%c\n",i,c);
            i++;
            str[i]='\0';
            if(i>=10)
            {
                printf("error ,the number is too large\n");
                return 0;
            }
            else{
                 scanf("%c",&c);

            }

                a=atof(str);
                printf("%f \n",a);
                Push(a);
                i=0;
            }

        }
        //如果不是数字
        switch (Postfix[j])
        {
        case '+':
            Pop(a);

            if(!Pop(b))//这是正负号,上一个不是数字
            {
                 Push(a);
                 break;
            }
            else{
                Pop(b);
                printf("%f+%f=%f\n",b,a,a+b);
                Push(a+b);
                break;
            }


        case '-':
            Pop(a);
            if(!Pop(b))
            {
                 Push(-a);
                 break;
            }
            else{
                printf("%f-%f=%f\n",b,a,b-a);
                Push(b-a);

            }
            break;


        case '*':
            Pop(a);
            if(!Pop(b))
            {
                 Push(pow(b,a));
                 break;
            }
            else{

                Pop(b);
                printf("%f*%f=%f\n",b,a,b*a);
                Push(b*a);
            }

            break;
        case '/':
            Pop(a);
            if(a==0)
            {
                printf("Divisor cannot be 0 \n");
                return -1;
            }
            else{
                Pop(b);
                Push(b/a);

            }


            break;
        default:
            cout<<"error, you entered non mathematical operators \n"<<endl;
            break;
        }
    Pop( a );
    return a;

}


template<typename ElemType>
bool Calculator<ElemType>::InfixtoPostfix(char Infix[],char Postfix[])
{
    Calculator<char> s;
    int i=0;
    int j=0;
    char e;

    while (Infix[j]!='\0')//忽略空格
    {
        if(Infix[j]>='0' && Infix[j]<='9')     //number
        {
            Postfix[i++]=Infix[j];
            j++;
        }
        else{// 非数字

                Postfix[i++]=' ';
            }
        }

        switch (Infix[j])
        {

        case ')':
            s.Pop(e);
            while ('('!=e)
            {
                Postfix[i++]=e;
                Postfix[i++]=' ';
                s.Pop(e);
            }
            break;

        case '+':
            if(0==s.StackLen())
                s.Push(Infix[j]);
            else
            {
                 s.Pop(e);
                 if('('==e)
                 {
                     s.Push(e);
                 }
                 else
                 {
                     Postfix[i++]=e;
                     Postfix[i++]=' ';
                 }
             }
            while (s.StackLen() && '('!=e){
                s.Push(Infix[j]);

         }
        break;

        case '-': // 和加法一样
            if(0==s.StackLen())
                s.Push(Infix[j]);
            else
            {
                 s.Pop(e);
                 if('('==e)
                 {
                     s.Push(e);
                 }
                 else
                 {
                     Postfix[i++]=e;
                     Postfix[i++]=' ';
                 }
             }

            while (s.StackLen() && '('!=e){
                s.Push(Infix[j]);

         }
        break;

        case '*':
            s.Push(Infix[j]);
            break;
        case '/':
            s.Push(Infix[j]);
            break;
        case '(':
            s.Push(Infix[j]);
            break;

        case '\0':
            break;

        default:
            printf("\n error, you entered non mathematical operators！\n");
            return false;
        }

        while (Infix[j]=='\0'){
            break;
        }
        j++;

        while ((Infix[j]='\0')){
            s.Pop(e);
            printf("%c ",e);
            Postfix[i++]=e;
            Postfix[i++]=' ';
     }

}

void Fraction::change(string doublenumber, int  fz, int  fm)//fz  分子，fm分母
{
    bool f = 0;              //flag
    char point;
    int n = doublenumber.length();

    for (int i = 0; i < n ; i++)
    {
        point = doublenumber[i];
        if (point == '.'){

            f = 1;
        }
        else
        {
            fz = fz * 10 + point - '0';
            if (f){

                fm *= 10;
        }
    }

    for (int i = 2; i <= fz; i++)				//化简
    {
        if (fz%i == 0 && fm%i == 0)
        {
            fz =fz/ i;
            fm = fm/i;
            i = 1;
        }
    }
    cout << fz << '/' << fm << endl;

}
}
int main()
{
    Calculator<double> calculator;
    char Infix[MAX_EXP_LEN];
    char Postfix[MAX_EXP_LEN];
    double sum;

    while(true){
        system("pause");
        system("cls");

        printf("\n\n\t\t\t please enter the number \n\n");
        gets(Infix);


        sum=calculator.Calculation(Postfix);
        cout<<"the result is ：";
        cout << sum <<endl;

        string doublenumber;
        cout << "Do you want to change the decimals of the output into fractions? 1 means yes, 2 means no" << endl;
        int answer;
        cin >> answer;
        switch(answer)
        {
        case 1:
            break;
        case 2:
            cout << "please enter your double number" << endl;
            cin >> doublenumber;
            while (!sum)
            {
                cin.clear();
                cout << "What you get is not a decimal" << endl;
                cout << "please enter a double number" << endl;
                cin >> doublenumber;

            }
            Fraction FenShu;
            int fz = 0, fm = 1;
            FenShu.change( doublenumber , fz, fm);
            break;

        }

    }
    return 0;

}
