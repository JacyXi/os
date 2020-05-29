#ifndef SCALENDAR_H
#define SCALENDAR_H


#include<iostream>
#include<iomanip>
#include <string>
#include <stdlib.h>
#include <map.h>
using namespace std;

class sCalendar {

public:
    sCalendar();
    ~sCalendar();
    int get_this_month();
    string showMonth(int year,int month);

private:
    int getStartDay(int year,int month);
    int getTotalDays(int year,int month);
    int getDaysinMonth(int year,int month);
    bool isLeapYear(int year);

    Map<string,int> month_map;

};




#endif // SCALENDAR_H
