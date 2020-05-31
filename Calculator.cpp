/*
 * File: Calculator.cpp
 * ---------------
 * This file implements the Calculator class.
 */
#include "Calculator.h"
#include "iostream"
#include "iomanip"
#include <string>



using namespace std;
typedef long long ll;

/* Constructor: Calculator
 * Usage: Calculator cal;
 * -------------------
 * Initial with default empty calculator.*/
Calculator::Calculator() {
}

/* Constructor: Calculator
 * Usage: Calculator(s);
 * -------------------
 * Initial with default size of the memory(4GB).*/
Calculator::Calculator(string s) {
    run(s);
}

Calculator::~Calculator() {

}
string Calculator::run(string s) {
    if(!judge(s)) {
         return "Error";
    } else {
        string after;
        after=dealneg(s);
        str_to_inf(after);
        inf_to_suf();
        double x = result();
        string result = to_string(x);
        return result;
    }
}

/*
 * Method: priority
 * Usage:priority(x);
 * ------------------
 * Helper function to figure out the operating priority.
 */
int Calculator::priority(char x) {
    if(x=='+'||x=='-')
        return 1;
    if(x=='*'||x=='/')
        return 2;
    if(x=='(')
        return 0;
    else
        return -1;
}

/*
 * Method: calcu
 * Usage: calcu(d1, d2, t);
 * ------------------
 * Helper function to calculate the equation.
 */
double Calculator::calcu(double d1, double d2, char t) {
    if (t == '+') {
        return (d1 + d2);
    } else if (t == '-') {
        return (d1-d2);
    } else if (t == '*') {
        return (d1 * d2);
    } else {
        return (d1 / d2);
    }
}

bool Calculator::judge(string s) {
    int n = s.length();
    bool f = false;
    stack<char> t;
    for (int i = 0; i < n; i++) {
       if(!(s[i] >= '0' && s[i] <= '9')) {
           if(s[i] != '+' && s[i] != '-' && s[i] != '*'
                   && s[i] != '/' && s[i] != '.'
                   && s[i] != '(' && s[i] != ')') {
               f = true;
               break;
            }
       }
       if(s.substr(i, 2) == "/0") {
           f = true;
           break;
       }
       if(t.empty() && s[i] == ')') {
          f = true;
          break;
       }
       if((s[n-1] < '0' || s[n - 1] > '9') && s[n - 1] != ')') {
           f = true;
           break;
       }
       if(s[i] == '(') {
          t.push(s[i]);
       }
       if(s[i] == ')') {
           t.pop();
       }
    }
    if(t.empty() && !f) {
       return true;
    }
    return false;
}

string Calculator::dealneg(string s) {
    for(int i = 0; i < (int) s.length(); i++) {
        if(s[i] == '-') {
            if (i == 0) {
                s.insert(0, 1, '0');
            } else if (s[i - 1] == '(') {
                s.insert(i,1,'0');
            }
        }
    }
    return s;
}

void Calculator::str_to_inf(string a) {
    int n = a.length();
    for(int i = 0; i < n;) {
        if((a[i] >= '0' && a[i] <= '9') || a[i] == '.') {
            string temp = "";
            int l = i;
            while((a[l] >= '0' && a[l] <= '9') || a[l] == '.') {
                temp += a[l];
                l++;
            }
            temp += '\0';
            int y = temp.length();
            char ss[y];
            for(int z = 0; z < y; z++) {
                ss[z] = temp[z];
            }
            infix[q].num = atof(ss);
            infix[q].sign = 1;
            q++;
            i=l;
        } else {
            infix[q].op = a[i];
            infix[q].sign = 2;
            q++;
            i++;
        }
    }
}

void Calculator::inf_to_suf() {              //中缀表达式转后缀表达式
    int cut = 0;
    for(int i = 0; i < q; i++) {
        if(infix[i].sign == 1) {
            suffix.push_back(infix[i]);
        }
        if(infix[i].sign == 2) {
            tool.push(infix[i].op);
            cut = i + 1;
            break;
        }
    }
    nodes r;
    for(int i = cut; i < q; i++) {
        if(infix[i].sign == 1) {
            suffix.push_back(infix[i]);
        } else if (infix[i].sign == 2) {
            if(infix[i].op == ')') {
                while(tool.top() != '(') {
                    r.op = tool.top();
                    r.sign = 2;
                    suffix.push_back(r);
                    tool.pop();
                }
                tool.pop();
             } else if((tool.empty()) ||
                priority(infix[i].op) > priority(tool.top())) {
                    tool.push(infix[i].op);
             } else if(infix[i].op == '(') {
                tool.push('(');
             } else if((tool.empty()) || priority(infix[i].op) <= priority(tool.top())) {
                    int y = priority(infix[i].op);
                    while((!tool.empty()) && priority(tool.top()) >= y) {
                        r.op = tool.top();
                        r.sign = 2;
                        suffix.push_back(r);
                        tool.pop();
                    }
                    tool.push(infix[i].op);
             }
        }

    }
    while(!tool.empty()) {
        r.op = tool.top();
        r.sign = 2;
        suffix.push_back(r);
        tool.pop();
    }
}

double Calculator::result() {
    int o = suffix.size();
        while(suffix.size() != 1) {
            int i = 0;
            for(; i <= o - 3;) {
               if((i + 2 < (int) suffix.size()) && suffix[i].sign == 1
                 && suffix[i + 1].sign == 1 && suffix[i + 2].sign == 2) {
                    suffix[i].num = calcu(suffix[i].num, suffix[i + 1].num,suffix[i + 2].op);
                    suffix.erase(suffix.begin() + i + 1);
                    suffix.erase(suffix.begin() + i + 1);
                    o -= 2;
                }
              else i++;
           }
    }
    return suffix[0].num;
}


