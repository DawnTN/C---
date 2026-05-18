#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lunardata.h"

#include<QMessageBox>
#include<QDebug>
#include<QColor>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ===== 新增：太阳轨道初始化 =====
        connect(ui->sunOrbitWidget, &SunOrbit::timeChanged, this, &MainWindow::onSunTimeChanged);
        float currentHour = QTime::currentTime().hour() + QTime::currentTime().minute() / 60.0f;
        ui
    ->sunOrbitWidget->setTime(currentHour);
        // ===== 太阳轨道结束 =====

    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::updateTime);
    updateTime();
    timer->start(1000);

    monthCheckTimer=new QTimer(this);
    connect(monthCheckTimer,&QTimer::timeout,this,&MainWindow::checkMonthChange);
    monthCheckTimer->start(10000);

    QDate today=QDate::currentDate();
    ui->yearEdit->setText(QString::number(today.year()));
    ui->monthEdit->setValue(today.month());
    currentDisplayYear=today.year();
    currentDisplayMonth=today.month();
    queryCalendar();
    connect(ui->querryBotton,&QPushButton::clicked,this,&MainWindow::queryCalendar);
    connect(ui->prevButton,&QPushButton::clicked,this,&MainWindow::prevMonth);
    connect(ui->nextButton,&QPushButton::clicked,this,&MainWindow::nextMonth);
}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
    delete monthCheckTimer;
}

void MainWindow::updateTime()//载入本地时间
{
    QDateTime now=QDateTime::currentDateTime();

    QString dateStr=now.toString("yyyy_M_dd");
    QString weekStr=now.toString("dddd");
    QString timeStr = now.toString("hh:mm:ss");

    // 秒针闪烁效果：奇数秒冒号变空格
    if (now.time().second() % 2 == 1) {
        timeStr.replace(":", " ");
    }

    ui->timeLabel->setText(timeStr);
    ui->datelabel->setText(dateStr);
    ui->weekLabel->setText(weekStr);

    QString lunarStr = LunarCalendar::getLunarDate(now.date().year(), now.date().month(), now.date().day());
    ui->lunarlabel->setText("农历："+lunarStr);

    // 同步背景色和文字颜色（按当前真实时间）
    float currentHour = now.time().hour() + now.time().minute() / 60.0f;
    onSunTimeChanged(currentHour);
}

void MainWindow::queryCalendar()//排序填表
{
    int year=ui->yearEdit->text().toInt();
    int month=ui->monthEdit->value();

    currentDisplayMonth=month;
    currentDisplayYear=year;

    ui->calendarTable->setColumnCount(7);
    QStringList headers;
    headers<<"日"<<"月"<<"火"<<"水"<<"木"<<"金"<<"土";
    ui->calendarTable->setHorizontalHeaderLabels(headers);

    ui->calendarTable->verticalHeader()->setVisible(false);
    ui->calendarTable->verticalHeader()->setDefaultSectionSize(30);
    for(int i=0;i<7;i++)
    {
        ui->calendarTable->setColumnWidth(i,30);
    }

    QDate firstDay(year,month,1);
    int startCol=firstDay.dayOfWeek()%7;
    int daysINMonth=firstDay.daysInMonth();

    ui->calendarTable->setRowCount(6);
    ui->calendarTable->clearContents();

    int row=0;
    int col=startCol;
    for(int day=1;day<=daysINMonth;day++)
    {
        QTableWidgetItem *item=new QTableWidgetItem(QString::number(day));
        item->setTextAlignment(Qt::AlignCenter);

        ui->calendarTable->setItem(row,col,item);
        QDate today=QDate::currentDate();
        if(year==today.year()&&month==today.month()&&day==today.day())
        {
            item->setBackground(QBrush(QColor(135,206,250)));
            item->setForeground(QBrush(QColor(255,255,255)));
        }
        col++;
        if(col>6)
        {
            col=0;
            row++;
        }
    }
}

void MainWindow::checkMonthChange()//自动翻页
{
    QDate today=QDate::currentDate();
    int realYear=today.year();
    int realMonth=today.month();

    if(realYear!=currentDisplayYear||realMonth!=currentDisplayMonth)
    {
        ui->yearEdit->setText(QString::number(realYear));
        ui->monthEdit->setValue(realMonth);
        queryCalendar();
    }
}
void MainWindow::on_querryBotton_clicked()
{

}
//手动翻页逻辑
void MainWindow::prevMonth()
{
    int year=ui->yearEdit->text().toInt();
    int month=ui->monthEdit->value();
    month--;
    if(month<1){month=12;year--;}
    ui->yearEdit->setText(QString::number(year));
    ui->monthEdit->setValue(month);
    queryCalendar();
}
void MainWindow::nextMonth()
{
    int year=ui->yearEdit->text().toInt();
    int month=ui->monthEdit->value();
    month++;
    if(month>12){month=1;year++;}
    ui->yearEdit->setText(QString::number(year));
    ui->monthEdit->setValue(month);
    queryCalendar();
}


// 颜色插值：在颜色a和颜色b之间平滑过渡（t=0是a，t=1是b）
QColor MainWindow::lerpColor(const QColor &a, const QColor &b, float t)
{
    int r = a.red()   + (b.red()   - a.red())   * t;
    int g = a.green() + (b.green() - a.green()) * t;
    int bl = a.blue()  + (b.blue()  - a.blue())  * t;
    return QColor(r, g, bl);
}

void MainWindow::onSunTimeChanged(float hour)
{


    // 局部颜色插值函数
    auto lerpColor = [](const QColor &a, const QColor &b, float t) -> QColor {
        int r = a.red()   + (b.red()   - a.red())   * t;
        int g = a.green() + (b.green() - a.green()) * t;
        int bl = a.blue()  + (b.blue()  - a.blue())  * t;
        return QColor(r, g, bl);
    };

    // 淡色系节点（马卡龙/奶油色）
    static const float nodeHours[] = {
        0.0f, 4.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f, 20.0f, 22.0f, 24.0f
    };
    static const QColor nodeColors[] = {
        QColor(176, 196, 222),   // 0点  深夜：淡钢蓝（明显一点）
        QColor(147, 112, 219),   // 4点  凌晨：淡紫（明显）
        QColor(255, 160, 122),   // 6点  日出：淡珊瑚（明显）
        QColor(135, 206, 235),   // 9点  上午：天蓝（明显）
        QColor(144, 238, 144),   // 12点 正午：淡绿（明显）
        QColor(173, 216, 230),   // 15点 下午：淡蓝（明显）
        QColor(255, 218, 185),   // 18点 黄昏：蜜桃（明显）
        QColor(221, 160, 221),   // 20点 日落：淡紫（明显）
        QColor(106, 90, 205),    // 22点 晚上：石板蓝（明显）
        QColor(176, 196, 222)    // 24点 深夜：淡钢蓝
    };
    const int nodeCount = 10;

    // 计算背景色
    QColor bgColor(232, 240, 250);
    for (int i = 0; i < nodeCount - 1; i++) {
        if (hour >= nodeHours[i] && hour <= nodeHours[i+1]) {
            float t = (hour - nodeHours[i]) / (nodeHours[i+1] - nodeHours[i]);
            bgColor = lerpColor(nodeColors[i], nodeColors[i+1], t);
            break;
        }
    }

    // 判断背景亮度，选文字颜色
    int brightness = (bgColor.red() * 299 + bgColor.green() * 587 + bgColor.blue() * 114) / 1000;
    QString textColor = (brightness > 180) ? "#5D6D7E" : "#FFFFFF";

    // ===== 终极背景色方案：直接操作 centralWidget =====
    QWidget *cw = this->centralWidget();  // Qt官方方法，不需要知道Designer里的名字
    if (cw) {
        cw->setStyleSheet(QString(
            "background-color: %1;"
        ).arg(bgColor.name()));
    }

    // 时间文字颜色
    ui->timeLabel->setStyleSheet(QString(
        "color: %1; font-weight: bold;"
    ).arg(textColor));
}
