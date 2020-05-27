#include "sCalendar.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include "console.h"
#include <ctime>


using namespace std;

sCalendar::sCalendar(){
    month_map.add("Jan",1);
    month_map.add("Feb",2);
    month_map.add("Mar",3);
    month_map.add("Apr",4);
    month_map.add("May",5);
    month_map.add("Jun",6);
    month_map.add("Jul",7);
    month_map.add("Aug",8);
    month_map.add("Sep",9);
    month_map.add("Oct",10);
    month_map.add("Nov",11);
    month_map.add("Dec",12);

}
sCalendar::~sCalendar(){

}

void sCalendar::printMonthName(int year,int month)
{
    char chMonth[12][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

    cout<<setw(12)<<"year: "<<year<<"    "<< endl;
    cout<<chMonth[month-1]<<endl;
    cout<<"-----------------------------"<<endl;
    cout<<"  Sun Mon Tue Wed Thu Fri Sat"<<endl;
}

int sCalendar::getStartDay(int year,int month)   //确定第一天是星期几
{

    int getstartday;
    int totalDays;
    totalDays= getTotalDays(year,month);
    getstartday=(totalDays+3)%7;
    return getstartday;
}

int sCalendar::getTotalDays(int year,int month)
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

int sCalendar::getDaysinMonth(int year,int month)
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

bool sCalendar::isLeapYear(int year) // 闰年
{
    return year%400==0||(year%4==0&&year%100!=0);
}


void sCalendar::printCalBody(int year,int month)
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
    cout<<"-----------------------------"<<endl;
}

void sCalendar::printCalBody_special(int year,int month)
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
    cout<<"-----------------------------"<<endl;
}

void sCalendar::printMonth(int year,int month)
{

    if ((month < 1)||(month > 12)){
        cout << "error month";
    }
    printMonthName(year,month);
    printCalBody(year,month);
}

void sCalendar::printMonth_special(int year,int month)
{

    if ((month < 1)||(month > 12)){
        cout << "error month";
    }
    printMonthName(year,month);
    printCalBody_special(year,month);
}

void sCalendar::showYear(int year){
//    cout << year<<"year"<<endl;
    int month;

    time_t now = time(0);
    string dt =ctime(&now);
//    cout<<"now is "<<dt<<endl;
    string Month_now = dt.substr(4,3);
    string Date_now = dt.substr(8,2);
    int Month_int = month_map.get(Month_now);

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

int sCalendar::get_this_month(){
    time_t now = time(0);
    string dt =ctime(&now);
    string Date_now = dt.substr(8,2);
    Date_now.c_str();
    string Month_now = dt.substr(4,3);
    int Month_int = month_map.get(Month_now);
    return Month_int;
}

string sCalendar::showMonth(int year, int month) {
    stringstream outPut ;
    char chMonth[12][10] = {"January", "February", "March", "April",
                            "May", "June", "July", "August", "September",
                            "October", "November", "December"};

    outPut<<setw(12)<<"year: "<<year<<"    "<< endl;
    outPut<<chMonth[month-1]<<endl;
    outPut<<"-----------------------------"<<endl;
    outPut<<"  Sun Mon Tue Wed Thu Fri Sat"<<endl;

    int startDay = getStartDay(year,month);
    int numberofDaysinMonth = getDaysinMonth(year,month);
    int i = 0;
    int j = 1;

    time_t now = time(0);
    string dt =ctime(&now);
    string Date_now = dt.substr(8,2);
    Date_now.c_str();
    int Date = atoi(Date_now.c_str());

    string Month_now = dt.substr(4,3);
    int Month_int = month_map.get(Month_now);



    for(i=0; i<startDay; i++)
    {
        outPut <<setw(4)<<" ";
    }
    for(i=1, j =startDay +1; i<Date;i++,j++)
    {
        outPut <<setw(4)<< i;
        if (j%7 ==0)
            outPut <<endl;
    }
    if(year==2020 && month ==Month_int){
        outPut<<setw(2)<<"*"<<Date<<"*";
        if (Date%7==0)
            outPut <<endl;
        outPut <<setw(3)<< Date+1;
        if (j%7 ==0)
            outPut <<endl;
    } else{
        outPut <<setw(4)<< i;
        if (j%7 ==0)
            outPut <<endl;
        outPut <<setw(4)<< Date+1;
        if (j%7 ==0)
            outPut <<endl;
    }

//    outPut <<setw(3)<< Date+1;
//    if (j%7 ==0)
//        outPut <<endl;
    for(i=Date+2, j =startDay +Date+2; i<=numberofDaysinMonth;i++,j++)
    {
        outPut <<setw(4)<< i;
        if (j%7 ==0)
            outPut <<endl;
    }

    outPut<<"\n";
    outPut<<"-----------------------------"<<endl;

    string a = outPut.str();

    return outPut.str();
}
/*
int main() {

    sCalendar* c = new sCalendar();
    string year;
    string month;
    cout << "enter a year" << endl;
    getline(cin,year);

//    c->showYear(atoi(year.c_str()));
    cout << c->showMonth(2020,5);

    return 0;
}
*/





