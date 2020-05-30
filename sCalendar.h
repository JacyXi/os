#ifndef SCALENDAR_H
#define SCALENDAR_H

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <map.h>

using namespace std;

class sCalendar {
public:
    /*
     * Constructor: sCalendar
     * Usage: sCalendar calendar;
     * ---------------------------------
     * Initialize a new calendar.
     */
    sCalendar();

    /*
     * Destructor: ~sCalendar
     * Usage: (Usually implicit)
     * ----------------------------------
     * Deallocate storage associated.
     */
    ~sCalendar();

    /*
     * Method: get_this_month
     * Usage: get_this_month();
     * ----------------------------------
     * first use time(0) to get a string of current time,
     * and cut the string to get the current month.
     */
    int get_this_month();

    /*
     * Method: showMonth
     * Usage: showMonth(year, month);
     * ----------------------------------
     * use stringstream to get the output of the calendar (as a string).
     * first write in the date from 1st to current date,
     * then current date and the next date, then to the end of the month.
     */
    string showMonth(int year,int month);

private:
    /*
     * Method: getStartDay
     * Usage: getStartDay(year, month);
     * ----------------------------------
     * returns the weekday for the first day of a certain month in certain year.
     * i.e the 1st May 2020 is Friday, then returns 5.
     */
    int getStartDay(int year,int month);

    /*
     * Method: getTotalDays
     * Usage: getTotalDays(year, month);
     * ----------------------------------
     * count and return how many days from 1st Jan 1800 to the first
     * day of a certain month in certain year.
     */
    int getTotalDays(int year,int month);

    /*
     * Method: getDaysinMonth
     * Usage: getDaysinMonth(year, month);
     * ----------------------------------
     * count and return how many days from 1st Jan 1800 to the first
     * day of a certain month in certain year.
     */
    int getDaysinMonth(int year,int month);

    /*
     * Method: isLeapYear
     * Usage: isLeapYear(year);
     * ----------------------------------
     * return true if a given year is leap year.
     */
    bool isLeapYear(int year);

    /* Map month_map*/
    Map<string,int> month_map;

};




#endif // SCALENDAR_H
