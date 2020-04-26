#ifndef CALENDAR_H
#define CALENDAR_H


#include<iostream>
#include<iomanip>
#include <string>
#include <stdlib.h>

void printMonth(int year,int month);
void printMonthName(int year,int month);
void printCalBody(int year,int month);
int getStartDay(int year,int month);
int getTotalDays(int year,int month);
int getDaysinMonth(int year,int month);
bool isLeapYear(int year);
void printMonth(int year,int month);
void showYear(int year);

#endif // CALENDAR_H
