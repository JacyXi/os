#ifndef SCALENDAR_H
#define SCALENDAR_H


#include<iostream>
#include<iomanip>
#include <string>
#include <stdlib.h>
#include <map.h>
using namespace std;

class sCalendar{

public:
    sCalendar();
    ~sCalendar();
    void printMonth(int year,int month);
    void printMonthName(int year,int month);
    void printCalBody(int year,int month);
    int getStartDay(int year,int month);
    int getTotalDays(int year,int month);
    int getDaysinMonth(int year,int month);
    bool isLeapYear(int year);
    void showYear(int year);
    int get_this_month();

    void printCalBody_special(int year, int month);
    void printMonth_special(int year, int month);

    string showMonth(int year,int month);
    Map<string,int> month_map;

};




#endif // SCALENDAR_H
