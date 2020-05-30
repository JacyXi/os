#include "sCalendar.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <ctime>
using namespace std;

/*
 * Constructor: sCalendar
 * Usage: sCalendar calendar;
 * ---------------------------------
 * Initialize a new calendar.
 */
sCalendar::sCalendar() {
    month_map.add("Jan", 1);
    month_map.add("Feb", 2);
    month_map.add("Mar", 3);
    month_map.add("Apr", 4);
    month_map.add("May", 5);
    month_map.add("Jun", 6);
    month_map.add("Jul", 7);
    month_map.add("Aug", 8);
    month_map.add("Sep", 9);
    month_map.add("Oct", 10);
    month_map.add("Nov", 11);
    month_map.add("Dec", 12);
}

/*
 * Destructor: ~sCalendar
 * Usage: (Usually implicit)
 * ----------------------------------
 * Deallocate storage associated.
 */
sCalendar::~sCalendar() {
    month_map.clear();
}

/*
 * Method: getStartDay
 * Usage: getStartDay(year, month);
 * ----------------------------------
 * returns the weekday for the first day of a certain month in certain year.
 * i.e the 1st May 2020 is Friday, then returns 5.
 */
int sCalendar::getStartDay(int year, int month) {

    int getstartday;
    int totalDays;
    totalDays = getTotalDays(year, month);
    getstartday = (totalDays + 3) % 7;
    return getstartday;
}

/*
 * Method: getTotalDays
 * Usage: getTotalDays(year, month);
 * ----------------------------------
 * count and return how many days from 1st Jan 1800 to the first
 * day of a certain month in certain year.
 */
int sCalendar::getTotalDays(int year, int month) {
    int total = 0;
    for (int i = 1800; i < year; i++) {
        if(isLeapYear(i)) {
            total += 366;
        } else {
            total+=365;
        }
    }
    for (int i = 1; i < month; i++) {
        total += getDaysinMonth(year, i);
    }
    return total;
}

/*
 * Method: getDaysinMonth
 * Usage: getDaysinMonth(year, month);
 * ----------------------------------
 * count and return how many days from 1st Jan 1800 to the first
 * day of a certain month in certain year.
 */
int sCalendar::getDaysinMonth(int year,int month) {
    if(month == 1 || month == 3 || month == 5 || month == 7
            || month == 8 || month == 10 || month == 12) {
        return 31;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return isLeapYear(year) ? 29 : 28;
    }
    return 0;
}

/*
 * Method: isLeapYear
 * Usage: isLeapYear(year);
 * ----------------------------------
 * return true if a given year is leap year.
 */
bool sCalendar::isLeapYear(int year) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

/*
 * Method: get_this_month
 * Usage: get_this_month();
 * ----------------------------------
 * first use time(0) to get a string of current time,
 * and cut the string to get the current month.
 */
int sCalendar::get_this_month() {
    time_t now = time(0);
    string dt = ctime(&now);
    string Date_now = dt.substr(8, 2);
    Date_now.c_str();
    string Month_now = dt.substr(4, 3);
    int Month_int = month_map.get(Month_now);
    return Month_int;
}

/*
 * Method: showMonth
 * Usage: showMonth(year, month);
 * ----------------------------------
 * use stringstream to get the output of the calendar (as a string).
 * first write in the date from 1st to current date,
 * then current date and the next date, then to the end of the month.
 */
string sCalendar::showMonth(int year, int month) {
    stringstream outPut;
    char chMonth[12][10] = {"January", "February", "March", "April",
                            "May", "June", "July", "August", "September",
                            "October", "November", "December"};

    outPut << setw(12) << "year: " << year << "    " << endl;
    outPut << chMonth[month - 1] << endl;
    outPut << "-----------------------------" << endl;
    outPut << "  Sun Mon Tue Wed Thu Fri Sat" << endl;

    int startDay = getStartDay(year, month);
    int numberofDaysinMonth = getDaysinMonth(year, month);
    int i = 0;
    int j = 1;

    time_t now = time(0);
    string dt = ctime(&now);
    string Date_now = dt.substr(8, 2);
    Date_now.c_str();
    int Date = atoi(Date_now.c_str());
    string Month_now = dt.substr(4, 3);
    int Month_int = 0;
    Month_int = month_map.get(Month_now);
    for (i = 0; i < startDay; i++) {
        outPut << "    ";
    }
    for (i = 1, j = startDay + 1; i < Date; i++, j++) {
        if (i < 10) {
            outPut << "   " << i;
        } else {
            outPut << "  " << i;
        }
        if (j % 7 == 0) {
            outPut << endl;
        }
    }
    if (year == 2020 && month == Month_int) {
        if (Date < 10) {
            outPut << " *" << Date << "*";
        } else {
            outPut << " *" << Date << "*";
        }
        if ((Date + startDay) % 7 == 0) {
            outPut << endl;
            outPut << " ";
        }
        if (Date + 1 < 10) {
            outPut << "  " << Date + 1;
        } else {
            outPut << " " << Date + 1;
        }
        if ((startDay + Date + 1) % 7 ==0) {
            outPut << endl;
        }
    } else{
        if(Date < 10) {
            outPut << "   " << Date;
        } else {
            outPut << "  " << Date;
        }
        if ((startDay + Date) % 7 == 0) {
            outPut << endl;
        }
        if (Date + 1 < 10) {
            outPut << "   " << Date + 1;
        } else {
            outPut << "  " << Date + 1;
        }
        if ((startDay + Date + 1) % 7 == 0) {
            outPut <<endl;
        }
    }

    for(i = Date + 2, j = startDay + Date + 2; i <= numberofDaysinMonth; i++, j++) {
        if (i < 10) {
            outPut << "   " << i;
        } else {
            outPut << "  " << i;
        }
        if (j % 7 == 0) {
            outPut << endl;
        }
    }
    outPut << "\n";
    outPut << "-------------------------------------------" << endl;
    return outPut.str();
}
