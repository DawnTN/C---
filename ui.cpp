#include <iostream>
#include <iomanip>
#include "ui.h"
#include "cal_func.h"

using namespace std;

void showMenu()
{
    cout << "======================" << endl;
    cout << "      万年历系统" << endl;
    cout << "======================" << endl;
    cout << "1. 查询指定月份" << endl;
    cout << "2. 查询指定年份" << endl;
    cout << "3. 退出" << endl;
    cout << "======================" << endl;
    cout << "请输入选项：";
}

void printMonth(int year, int month)
{
    cout << "\n==========" << year << "年" << month << "月==========" << endl;
    cout << "日 一 二 三 四 五 六" << endl;

    int firstWeek = getWeekDay(year, month, 1);
    int days = getMonthDays(year, month);

    for(int i=0; i<firstWeek; i++)
        cout << "   ";

    for(int d=1; d<=days; d++)
    {
        cout << setw(2) << d << " ";
        if((d+firstWeek) % 7 == 0)
            cout << endl;
    }
    cout << endl << endl;
}

void printYear(int year)
{
    cout << "\n==============" << year << "年 万年历==============\n" << endl;
    for(int m=1; m<=12; m++)
    {
        printMonth(year, m);
    }
}
