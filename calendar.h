#ifndef CALENDAR_H
#define CALENDAR_H


#include<iostream>
#include<iomanip>
#include <string>
#include <stdlib.h>
using namespace std;

class calendar{

public:
    calendar();
    ~calendar();
    void printMonth(int year,int month);

private:
    void printMonthName(int year,int month);
    void printCalBody(int year,int month);
    int getStartDay(int year,int month);
    int getTotalDays(int year,int month);
    int getDaysinMonth(int year,int month);
    bool isLeapYear(int year);
    void showYear(int year);

    void printCalBody_special(int year, int month);
    void printMonth_special(int year, int month);
};



#endif // CALENDAR_H
