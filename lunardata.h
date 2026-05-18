#ifndef LUNARCALENDAR_H
#define LUNARCALENDAR_H

#include <QString>

// 农历一年的信息
class LunarYearInfo {
public:
    int leapMonth;        // 闰几月？0 = 没有闰月
    int monthDays[13];    // 每个月多少天
    int totalMonths;      // 实际有多少个月
};

// 农历工具类（全是静态方法，不用创建对象，直接调用）
class LunarCalendar {
public:
    // 输入公历年月日，返回农历字符串
    static QString getLunarDate(int year, int month, int day);

private:
    // 解码十六进制农历数据
    static LunarYearInfo decodeLunarInfo(int year);
};

#endif // LUNARCALENDAR_H
