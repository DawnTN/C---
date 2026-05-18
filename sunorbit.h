#ifndef SUNORBIT_H
#define SUNORBIT_H

#include <QWidget>
#include <QPoint>
#include <QTimer>

class SunOrbit : public QWidget
{
    Q_OBJECT
public:
    explicit SunOrbit(QWidget *parent = nullptr);

    // 设置当前时间（0-24小时），自动移动太阳到对应位置
    void setTime(float hour);

    // 获取当前太阳位置对应的时间
    float currentTime() const;

    void syncToCurrentTime();


signals:
    // 太阳位置变化时发出信号，通知主窗口改背景色
    void timeChanged(float hour);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // 计算正弦曲线上某时间对应的坐标
    QPoint sunPosition(float hour) const;

    // 根据鼠标位置，反推最近的时间
    float timeFromPosition(const QPoint &pos) const;

    float m_currentHour;    // 当前时间（0-24）
    bool m_isDragging;      // 是否正在拖动
    QRect m_orbitRect;      // 轨道区域
    QTimer *m_returnTimer;//自动回归定时器
private slots:
    void onReturnTimer();
};

#endif // SUNORBIT_H
