#include "lunardata.h"
#include <QDate>

// 农历1900-2100年的压缩数据（200个）
static const unsigned int lunarInfo[] = {
    0x04bd8,0x04ae0,0x0a570,0x054d5,0x0d260,0x0d950,0x16554,0x056a0,0x09ad0,0x055d2,
    0x04ae0,0x0a5b6,0x0a4d0,0x0d250,0x1d255,0x0b540,0x0d6a0,0x0ada2,0x095b0,0x14977,
    0x04970,0x0a4b0,0x0b4b5,0x06a50,0x06d40,0x1ab54,0x02b60,0x09570,0x052f2,0x04970,
    0x06566,0x0d4a0,0x0ea50,0x06e95,0x05ad0,0x02b60,0x186e3,0x092e0,0x1c8d7,0x0c950,
    0x0d4a0,0x1d8a6,0x0b550,0x056a0,0x1a5b4,0x025d0,0x092d0,0x0d2b2,0x0a950,0x0b557,
    0x06ca0,0x0b550,0x15355,0x04da0,0x0a5d0,0x14573,0x052d0,0x0a9a8,0x0e950,0x06aa0,
    0x0aea6,0x0ab50,0x04b60,0x0aae4,0x0a570,0x05260,0x0f263,0x0d950,0x05b57,0x056a0,
    0x096d0,0x04dd5,0x04ad0,0x0a4d0,0x0d4d4,0x0d250,0x0d558,0x0b540,0x0b5a0,0x195a6,
    0x095b0,0x049b0,0x0a974,0x0a4b0,0x0b27a,0x06a50,0x06d40,0x0af46,0x0ab60,0x09570,
    0x04af5,0x04970,0x064b0,0x074a3,0x0ea50,0x06b58,0x055c0,0x0ab60,0x096d5,0x092e0,
    0x0c960,0x0d954,0x0d4a0,0x0da50,0x07552,0x056a0,0x0abb7,0x025d0,0x092d0,0x0cab5,
    0x0a950,0x0b4a0,0x0baa4,0x0ad50,0x055d9,0x04ba0,0x0a5b0,0x15176,0x052b0,0x0a930,
    0x07954,0x06aa0,0x0ad50,0x05b52,0x04b60,0x0a6e6,0x0a4e0,0x0d260,0x0ea65,0x0d530,
    0x05aa0,0x076a3,0x096d0,0x04bd7,0x04ad0,0x0a4d0,0x1d0b6,0x0d250,0x0d520,0x0dd45,
    0x0b5a0,0x056d0,0x055b2,0x049b0,0x0a577,0x0a4b0,0x0aa50,0x1b255,0x06d20,0x0ada0,
    0x04b63,0x0937f,0x049f8,0x04970,0x064b0,0x068a6,0x0ea5f,0x06b20,0x0a6c4,0x0aaef,
    0x092e0,0x0d2e3,0x0c960,0x0d557,0x0d4a0,0x0da50,0x05d55,0x056a0,0x0a6d0,0x055d4,
    0x052d0,0x0a9b8,0x0a950,0x0b4a0,0x0b6a6,0x0ad50,0x055a0,0x0aba4,0x0a5b0,0x052b0,
    0x0b273,0x06930,0x07337,0x06aa0,0x0ad50,0x14b55,0x04b60,0x0a570,0x054e4,0x0d160,
    0x0e968,0x0d520,0x0daa0,0x16aa6,0x056d0,0x04ae0,0x0a9d4,0x0a2d0,0x0d150,0x0f252,
    0x0d520
};

// 解码函数
LunarYearInfo LunarCalendar::decodeLunarInfo(int year)
{
    LunarYearInfo info;
    unsigned int hexCode = lunarInfo[year - 1900];

    // 低4位 = 闰几月
    info.leapMonth = hexCode & 0xf;
    if (info.leapMonth > 12) info.leapMonth = 0;

    info.totalMonths = 12 + (info.leapMonth ? 1 : 0);

    // 从第4位开始读12个月（不是第16位！）
    for (int i = 0; i < 12; i++) {
        bool isBigMonth = (hexCode >> (4 + i)) & 1;
        info.monthDays[i] = isBigMonth ? 30 : 29;
    }

    // 闰月天数（如果有）
    if (info.leapMonth) {
        bool isBigLeap = (hexCode >> 16) & 1;  // 闰月大小在第16位
        info.monthDays[12] = isBigLeap ? 30 : 29;
    }

    return info;
}
// 公历转农历
QString LunarCalendar::getLunarDate(int year, int month, int day)
{
    static int baseYear = 1900;
    static int baseMonth = 1;
    static int baseDay = 31;

    static const char *gan[] = {"甲","乙","丙","丁","戊","己","庚","辛","壬","癸"};
    static const char *zhi[] = {"子","丑","寅","卯","辰","巳","午","未","申","酉","戌","亥"};
    static const char *animals[] = {"鼠","牛","虎","兔","龙","蛇","马","羊","猴","鸡","狗","猪"};
    static const char *lunarMonths[] = {"正","二","三","四","五","六","七","八","九","十","冬","腊"};
    static const char *lunarDays[] = {
        "初一","初二","初三","初四","初五","初六","初七","初八","初九","初十",
        "十一","十二","十三","十四","十五","十六","十七","十八","十九","二十",
        "廿一","廿二","廿三","廿四","廿五","廿六","廿七","廿八","廿九","三十"
    };

    QDate baseDate(baseYear, baseMonth, baseDay);
    QDate targetDate(year, month, day);
    int totalDays = baseDate.daysTo(targetDate);

    int lunarYear = baseYear;
    int yearDays;
    while (totalDays >= 0) {
        LunarYearInfo info = decodeLunarInfo(lunarYear);
        yearDays = 0;
        for (int i = 0; i < info.totalMonths; i++) {
            yearDays += info.monthDays[i];
        }
        if (totalDays < yearDays) break;
        totalDays -= yearDays;
        lunarYear++;
    }

    LunarYearInfo info = decodeLunarInfo(lunarYear);
    int lunarMonth = 1;
    int lunarDay = 1;
    bool isLeapMonth = false;

    for (int i = 0; i < info.totalMonths; i++) {
        int monthDays = info.monthDays[i];
        if (totalDays < monthDays) {
            lunarDay = totalDays + 1;
            if (info.leapMonth && i == info.leapMonth) {
                isLeapMonth = true;
                lunarMonth = info.leapMonth;
            } else {
                isLeapMonth = false;
                if (info.leapMonth && i > info.leapMonth) lunarMonth = i;
                else lunarMonth = i + 1;
            }
            break;
        }
        totalDays -= monthDays;
    }

    int ganIdx = (lunarYear - 4) % 10;
    int zhiIdx = (lunarYear - 4) % 12;
    QString yearStr = QString("%1%2年").arg(gan[ganIdx]).arg(zhi[zhiIdx]);
    QString animalStr = QString("【%1】").arg(animals[zhiIdx]);
    QString monthStr = isLeapMonth ? QString("闰%1月").arg(lunarMonths[lunarMonth-1])
                                   : QString("%1月").arg(lunarMonths[lunarMonth-1]);
    QString dayStr = lunarDays[lunarDay - 1];

    return yearStr + animalStr + monthStr + dayStr;
}
