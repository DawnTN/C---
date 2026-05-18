#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lunardata.h"
#include<QMessageBox>
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    QString timeStr=now.toString("hh:mm:ss");
    QString dateStr=now.toString("yyyy_M_dd");
    QString weekStr=now.toString("dddd");
    ui->timeLabel->setText(timeStr);
    ui->datelabel->setText(dateStr);
    ui->weekLabel->setText(weekStr);
    QString lunarStr
    = LunarCalendar::getLunarDate(now.date().year(), now.date().month(), now.date().day());

    ui->lunarlabel->setText("农历："+lunarStr);
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





