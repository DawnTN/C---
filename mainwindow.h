#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include<QDate>
#include "lunardata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void updateTime();
    void queryCalendar();
    void checkMonthChange();
    void on_querryBotton_clicked();
    void prevMonth();
    void nextMonth();



private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *monthCheckTimer;

    int currentDisplayYear;
    int currentDisplayMonth;


};
#endif // MAINWINDOW_H
