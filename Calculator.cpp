#include "Calculator.h"
#include "iostream"
#include "iomanip"
#include <string>



using namespace std;
typedef long long ll;

Calculator::Calculator(){
}
Calculator::Calculator(string s){
    run(s);
}
Calculator::~Calculator(){

}
void Calculator::run(string s){
    if(!judge(s)){
         cout<<endl<<"Wrong Input！"<<endl;
         s.clear();
    }
    else {
        string after;
        after=dealneg(s);
        str_to_inf(after);
        inf_to_suf();
        cout<<endl<<"The Result is:："<<result()<<endl;
    }

}

int Calculator::priority(char x){                       //给操作符赋予优先级
    if(x=='+'||x=='-')
        return 1;
    if(x=='*'||x=='/')
        return 2;
    if(x=='(')
        return 0;
    else
        return -1;
}
double Calculator::calcu(double d1,double d2,char t){   //四则运算
    if(t=='+')
        return (d1+d2);
    else if(t=='-')
        return (d1-d2);
    else if(t=='*')
        return (d1*d2);
    else
        return (d1/d2);
}

bool Calculator::judge(string s){           //判断表达式是否合法
    int n=s.length();
    bool f=false;
    stack<char> t;
    for(int i=0;i<n;i++){
           if(!(s[i]>='0'&&s[i]<='9')){
               if(s[i]!='+'&&s[i]!='-'&&s[i]!='*'&&s[i]!='/'&&s[i]!='.'&&s[i]!='('&&s[i]!=')'){
                   f=true;
                   break;
                }
           }               //有非法标识符，直接标记f为true并退出循环
           if(s.substr(i,2)=="/0"){
               f=true;     //每次截取长度为2的子串
               break;      //0不能做除数
           }
           if(t.empty()&&s[i]==')'){
              f=true;
              break;
           }
           if((s[n-1]<'0'||s[n-1]>'9')&&s[n-1]!=')'){
               f=true;        //表达式最后一位不是数字或者')'的不合法
               break;
           }
           if(s[i]=='('){
              t.push(s[i]);   //左括号入栈
           }
           if(s[i]==')'){     //每有一个右括号，弹出一个左括号
               t.pop();
           }
    }
    if(t.empty()&&!f){      //括号匹配(栈空)并且符合其他条件,表达式合法
       return true;
    }
    return false;
}

string Calculator::dealneg(string s){     //处理字符串里的负数 ，补0.
    for(int i=0;i<(int)s.length();i++){
        if(s[i]=='-'){
            if(i==0){
                s.insert(0,1,'0');      //表达式第一位，为'-'，如-3+(5*(3-7))这类的
            }                             //在每一个'-'前面补上一个'0'

            else if(s[i-1]=='('){       //表达式中间出现负数，该负数前面必然有左括号；
                s.insert(i,1,'0');      //在i处的前面加一个0 变成0-n;
            }                             //如：3+(-3*5) 或者 3+(-3*(-3))
        }
    }
    return s;
}

void Calculator::str_to_inf(string a){      //字符串转化为中缀表达式
    int n=a.length();
    for(int i=0;i<n;){
        if((a[i]>='0'&&a[i]<='9')||a[i]=='.'){
            string temp="";
            int l=i;
            while((a[l]>='0'&&a[l]<='9')||a[l]=='.'){  //实数用一个string保存，
                temp+=a[l];
                 //不直接用char数组，是因为操作的对象长度未知，char数组大小不好确定
                l++;
            }

            temp+='\0';
            int y=temp.length();        //string 转 字符数组
            char ss[y];
            for(int z=0;z<y;z++){
                ss[z]=temp[z];
            }
            infix[q].num =atof(ss);    //字符串转实数函数
            infix[q].sign=1;
            q++;
            i=l;                       //注意移动坐标i;
        }
        else{
            infix[q].op=a[i];    //运算符就直接存
            infix[q].sign=2;
            q++;
            i++;
        }
    }
}

void Calculator::inf_to_suf(){              //中缀表达式转后缀表达式
    int cut=0;
    for(int i=0;i<q;i++){    //预处理，为了方便进行运算符优先级的比较，先往栈里压入一个运算符
        if(infix[i].sign==1){
            suffix.push_back(infix[i]);   //数字直接存进后缀表达式
        }
        if(infix[i].sign ==2){
            tool.push(infix[i].op);
            cut=i+1;
            break;      //保证stack有东西，先存一个运算符
        }
    }

    nodes r;       //中间变量
    for(int i=cut;i<q;i++){   //从那个运算符之后再操作
        if(infix[i].sign==1){
            suffix.push_back(infix[i]);
        }
        else if(infix[i].sign==2){
                if(infix[i].op==')'){        //假如是右括号，就去往前匹配一个左括号
                while(tool.top()!='('){        //没匹配到之前，运算符存到后缀表达式
                    r.op =tool.top();
                    r.sign =2;
                    suffix.push_back(r);
                    tool.pop();         //弹出运算符
                }
                tool.pop(); //弹出左括号
                }
                else if((tool.empty())||
                priority(infix[i].op)>priority(tool.top())){   //这里要注意判断栈是不是空
                    tool.push(infix[i].op); //运算符优先级大于栈顶元素，入栈
                }
                else if(infix[i].op=='('){
                    tool.push('(');          //左括号入栈
                }
                else if((tool.empty())||priority(infix[i].op)<=priority(tool.top())){
                    int y=priority(infix[i].op);                 //运算符优先级小于等于栈顶元素
                    while((!tool.empty())&&priority(tool.top())>=y){
                        r.op =tool.top();
                        r.sign =2;
                        suffix.push_back(r);    //保存到后缀表达式并弹出
                        tool.pop();
                    }
                    tool.push(infix[i].op);   //将这个运算符入栈
            }
        }

    }

    while(!tool.empty()){
        r.op =tool.top();   //以上步骤进行完，栈里还有运算符，全部弹出，存到后缀表达式
        r.sign =2;
        suffix.push_back(r);
        tool.pop();
    }
}

double Calculator::result(){                //后缀表达式计算
    int o=suffix.size();
        while(suffix.size()!=1){
            int i=0;
           for(;i<=o-3;){
              if((i+2<(int)suffix.size())&&suffix[i].sign==1
                 &&suffix[i+1].sign==1&&suffix[i+2].sign==2){
    suffix[i].num=calcu(suffix[i].num,suffix[i+1].num,suffix[i+2].op);
            suffix.erase(suffix.begin()+i+1);  //这一个步骤执行之后，原来的i+2,变成了i+1;
            suffix.erase(suffix.begin()+i+1);  //所以这里再写i+2就是错的了
                  o-=2;  //总长度减2
              }
              else i++;
           }
    }
    return suffix[0].num;
}


