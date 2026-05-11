#include "cal_func.h"

// 判断闰年
bool isLeapYear(int year)
{
    if((year%4==0 && year%100!=0) || (year%400==0))
        return true;
    return false;
}

// 获取某月天数
int getMonthDays(int year, int month)
{
    int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if(month == 2 && isLeapYear(year))
        return 29;
    return days[month-1];
}

// 计算1号是星期几 (0=周日,1=周一...6=周六)
int getWeekDay(int year, int month, int day)
{
    if(month < 3)
    {
        month += 12;
        year--;
    }
    int c = year / 100;
    int y = year % 100;
    int w = (c/4 - 2*c + y + y/4 + (13*(month+1)/5) + day - 1) % 7;
    if(w < 0) w += 7;
    return w;
}
