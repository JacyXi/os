#include "calendar.h"
#include <iostream>
#include <iomanip>
#include "calendar.h"
#include <string>
#include <stdlib.h>
#include "console.h"
#include <ctime>


using namespace std;

calendar::calendar(){

}
calendar::~calendar(){

}

void calendar::printMonthName(int year,int month)
{
    char chMonth[12][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

    cout<<setw(12)<<"year: "<<year<<"    "<< endl;
    cout<<chMonth[month-1]<<endl;
    cout<<"-------------------------------------------"<<endl;
    cout<<"  Sun Mon Tue Wed Thu Fri Sat"<<endl;
}

int calendar::getStartDay(int year,int month)   //确定第一天是星期几
{

    int getstartday;
    int totalDays;
    totalDays= getTotalDays(year,month);
    getstartday=(totalDays+3)%7;
    return getstartday;
}

int calendar::getTotalDays(int year,int month)
{
    int total = 0;
    for(int i=1800; i<year; i++) //从1800年开始
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
        total+=getDaysinMonth(year,i);// 从1800年到这个月1号有几天
    }
    return total;
}

int calendar::getDaysinMonth(int year,int month)
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

bool calendar::isLeapYear(int year) // 闰年
{
    return year%400==0||(year%4==0&&year%100!=0);
}


void calendar::printCalBody(int year,int month)
{
    int startDay = getStartDay(year,month);
    int numberofDaysinMonth = getDaysinMonth(year,month);
    int i = 0;
    int j = 1;

    for(i=0; i<startDay; i++)
    {
        cout <<setw(4)<<" ";
    }
    for(i=1, j =startDay +1; i<=numberofDaysinMonth;i++,j++)
    {
        cout <<setw(4)<< i;
        if (j%7 ==0)
            cout <<endl;
    }

    cout<<"\n";
    cout<<"-------------------------------------------"<<endl;
}

void calendar::printCalBody_special(int year,int month)
{
    int startDay = getStartDay(year,month);
    int numberofDaysinMonth = getDaysinMonth(year,month);
    int i = 0;
    int j = 1;

    time_t now = time(0);
    string dt =ctime(&now);
    string Date_now = dt.substr(8,2);
    Date_now.c_str();
    int Date = atoi(Date_now.c_str());

    for(i=0; i<startDay; i++)
    {
        cout <<setw(4)<<" ";
    }
    for(i=1, j =startDay +1; i<Date;i++,j++)
    {
        cout <<setw(4)<< i;
        if (j%7 ==0)
            cout <<endl;
    }

    cout<<setw(2)<<"*"<<Date<<"*";
    if (Date%7==0)
        cout <<endl;
    cout <<setw(3)<< Date+1;
    if (j%7 ==0)
        cout <<endl;
    for(i=Date+2, j =startDay +Date+2; i<=numberofDaysinMonth;i++,j++)
    {
        cout <<setw(4)<< i;
        if (j%7 ==0)
            cout <<endl;
    }

    cout<<"\n";
    cout<<"-------------------------------------------"<<endl;
}

void calendar::printMonth(int year,int month)
{

    if ((month < 1)||(month > 12)){
        cout << "error month";
    }
    printMonthName(year,month);
    printCalBody(year,month);
}

void calendar::printMonth_special(int year,int month)
{

    if ((month < 1)||(month > 12)){
        cout << "error month";
    }
    printMonthName(year,month);
    printCalBody_special(year,month);
}

void calendar::showYear(int year){
//    cout << year<<"year"<<endl;
    int month;

    time_t now = time(0);
    string dt =ctime(&now);
//    cout<<"now is "<<dt<<endl;
    string Month_now = dt.substr(4,3);
    string Date_now = dt.substr(8,2);
    int Month_int = 0;

    if (Month_now == "Jan"){
        Month_int = 1;
    } else if (Month_now =="Feb"){
        Month_int =2;
    } else if (Month_now =="Mar"){
        Month_int =3;
    } else if (Month_now =="Apr"){
        Month_int =4;
    } else if (Month_now =="May"){
        Month_int =5;
    } else if (Month_now =="Jun"){
        Month_int =6;
    } else if (Month_now =="Jul"){
        Month_int =7;
    } else if (Month_now =="Aug"){
        Month_int =8;
    } else if (Month_now =="Sep"){
        Month_int =9;
    } else if (Month_now =="Oct"){
        Month_int =10;
    } else if (Month_now =="Nov"){
        Month_int =11;
    } else if (Month_now =="Dec"){
        Month_int =12;
    }

    cout<<Month_now<<endl;
    cout<<Date_now<<endl;

    if (year !=2020){
        for  (month=1; month<=12; month++){
            printMonth(year,month);

        }
    }else{
        for (month=1; month<Month_int;month++){
            printMonth(year,month);
        }
        printMonth_special(2020,Month_int);
        for (month = Month_int+1;month<=12;month++){
            printMonth(year,month);
        }
    }
}


int main() {

    calendar* c = new calendar();
    string year;
    cout << "enter a year" << endl;
    getline(cin,year);

    c->showYear(atoi(year.c_str()));

    return 0;
}

