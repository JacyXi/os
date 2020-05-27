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


    for(i=0; i<startDay; i++)
    {
        outPut << "    ";
    }
    for(i=1, j =startDay +1; i<Date;i++,j++)   //startday to Date
    {
        if(i<10){
            outPut <<"   "<< i;
        }else{
            outPut << "  "<<i;
        }
        if (j%7 ==0)
            outPut <<endl;
    }
    if(year==2020 && month ==Month_int){     //at Date
        if(Date<10){
            outPut <<" *"<< Date<<"*";
        }else{
            outPut << " *"<<Date<<"*";
        }
        if (Date%7==0)
            outPut <<endl;
        if(Date+1 <10){
            outPut <<"  "<< Date+1;
        }else{
            outPut << " "<<Date+1;
        }
        if ((startDay+ Date)%7 ==0)
            outPut <<endl;
    } else{
        if(Date<10){
            outPut <<"   "<< Date;
        }else{
            outPut << "  "<<Date;
        }
        if ((startDay+Date)%7 ==0)
            outPut <<endl;
        if(Date+1<10){
            outPut <<"   "<< Date+1;
        }else{
            outPut << "  "<<Date+1;
        }
        if ((startDay+Date+1)%7 ==0)
            outPut <<endl;
    }

    for(i=Date+2, j =startDay +Date+2; i<=numberofDaysinMonth;i++,j++)  //Date to end
    {
        if(i<10){
            outPut <<"   "<< i;
        }else{
            outPut << "  "<<i;
        }
        if (j%7 ==0)
            outPut <<endl;
    }

    outPut<<"\n";
    outPut<<"-------------------------------------------"<<endl;

    string a = outPut.str();

    return outPut.str();
}

