#include <iostream>
#include "cal_func.h"
#include "ui.h"

using namespace std;

int main()
{
    int choice;
    while(true)
    {
        showMenu();
        cin >> choice;

        if(choice == 1)
        {
            int y, m;
            cout << "请输入年份：";
            cin >> y;
            cout << "请输入月份：";
            cin >> m;
            printMonth(y, m);
        }
        else if(choice == 2)
        {
            int y;
            cout << "请输入年份：";
            cin >> y;
            printYear(y);
        }
        else if(choice == 3)
        {
            cout << "感谢使用！" << endl;
            break;
        }
        else
        {
            cout << "输入错误，请重试！" << endl;
        }
    }
    return 0;
}
