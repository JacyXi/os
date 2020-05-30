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
    /* Default constructor for Calculator.*/
    Calculator();

    /* Default constructor for Calculator.*/
    ~Calculator();

    /* Constructor for Calculator with operation equation s.*/
    Calculator(string s);

    /*
     * Method: priority
     * Usage:priority(x);
     * ------------------
     * Helper function to figure out the operating priority.
     */
    int priority(char x);

    /*
     * Method: calcu
     * Usage: calcu(d1, d2, t);
     * ------------------
     * Helper function to calculate the equation.
     */
    double calcu(double d1, double d2, char t);

    /*
     * Method: judge
     * Usage: judge(s);
     * ------------------
     * Helper function to judge whether the equation is legal.
     */
    bool judge(string s);

    /*
     * Method: dealneg
     * Usage: dealneg(s);
     * ------------------
     * Helper function to deal with negative number.
     */
    string dealneg(string s);

    /*
     * Method: str_to_inf
     * Usage: str_to_inf(str);
     * ------------------
     * Intermediate helper function to change expression.
     */
    void str_to_inf(string str);

    /*
     * Method: inf_to_suf
     * Usage: inf_to_suf();
     * ------------------
     * Intermediate helper function to change expression.
     */
    void inf_to_suf();

    /*
     * Method: result
     * Usage: result();
     * ------------------
     * Calculate to the final result.
     */
    double result();

    /*
     * Method: run
     * Usage: run(s);
     * ------------------
     * Activate the calculator.
     */
    string run(string s);

private:

    /* Struct nodes: store the middle step elements.*/
    struct nodes {
        char op;
        double num;
        int sign;
    };
    /* int q*/
    int q = 0;
    /* stack tool*/
    stack<char> tool;
    /* node infix[]*/
    nodes infix[10000000];
    /* vector suffix*/
    vector<nodes> suffix;
};

#endif // C_H
