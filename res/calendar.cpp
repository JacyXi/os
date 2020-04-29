#include "calendar.h"
#include<iostream>
#include<iomanip>
#include "calendar.h"
#include <string>
#include <stdlib.h>
using namespace std;


void printMonthName(int year,int month)
{
    char chMonth[12][10] = {"January", "February", "March", "April", "May", "June", "August", "September", "October", "November", "December"};

    cout<<setw(12)<<year<<"year"<<"    "<< endl;
    cout<<chMonth[month-1]<<endl;
    cout<<"-------------------------------------------"<<endl;
    cout<<"  Sun Mon Tue Wed Thu Fri Sat"<<endl;
}

int getStartDay(int year,int month)
{

    int getstartday;
    int totalDays;
    totalDays= getTotalDays(year,month);
    getstartday=(totalDays+3)%7;
    return getstartday;
}

int getTotalDays(int year,int month)
{
    int total = 0;
    for(int i=1800; i<year; i++)
    {
        if(isLeapYear(i))
        {
            total+=366;
        }
        else
        {
            total+=365;
        }
    }
    for(int i=1; i<month; i++)
    {
        total+=getDaysinMonth(year,i);// 这一年的天数+从这一年开始到这个月的天数
    }
    return total;
}

int getDaysinMonth(int year,int month)
{
    if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)
    {
        return 31;
    }
    else if(month==4||month==6||month==9||month==11)
    {
        return 30;
    }
    else
    {
        return isLeapYear(year)?29:28;
    }
    return 0;
}

bool isLeapYear(int year) // 闰年
{
    return year%400==0||(year%4==0&&year%100!=0);
}


void printCalBody(int year,int month)
{
    int startDay = getStartDay(year,month);
    int numberofDaysinMonth = getDaysinMonth(year,month);
    int i = 0;
    for(i=0; i<startDay; i++)
    {
        cout<<"    ";
    }
    for(i=1; i<=numberofDaysinMonth; i++)
    {
        cout<<setw(7);
        if((i+startDay)%7==0)  // 周天
        {
            cout<<endl;
        }
    }
    cout<<endl;
    cout<<"-------------------------------------------"<<endl;
}


void printMonth(int year,int month)
{

    if ((month < 1)||(month > 12)){
        cout << "error month";
    }
    printMonthName(year,month);
    printCalBody(year,month);
}

void showYear(int year){
    cout << year<<"year"<<endl;
    int month;
    for  (month=1; month<=12; month++){
        printMonth(year,month);

    }
}
