#include "sunorbit.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include <QTime>

SunOrbit::SunOrbit(QWidget *parent) : QWidget(parent)
{
    m_currentHour = 12.0f;  // 默认中午
    m_isDragging = false;
    setMinimumSize(400, 200);
    setMaximumSize(400, 200);

    // 自动回归定时器：松开鼠标后3秒触发
    m_returnTimer = new QTimer(this);
    m_returnTimer->setSingleShot(true);  // 只触发一次
    connect(m_returnTimer, &QTimer::timeout, this, &SunOrbit::onReturnTimer);
}

void SunOrbit::setTime(float hour)
{
    if (hour < 0) hour = 0;
    if (hour > 24) hour = 24;
    m_currentHour = hour;
    update();  // 重绘
    emit timeChanged(m_currentHour);
}

float SunOrbit::currentTime() const
{
    return m_currentHour;
}

// 正弦曲线：y = 高度/2 + 振幅 * sin(时间比例 * π)
QPoint SunOrbit::sunPosition(float hour) const
{
    float ratio = hour / 24.0f;  // 0~1

    // X坐标：均匀分布
    int x = m_orbitRect.left() + ratio * m_orbitRect.width();

    // Y坐标：完整正弦波（波浪起伏）
    float amplitude = m_orbitRect.height() * 0.4f;
    int centerY = m_orbitRect.top() + m_orbitRect.height() / 2;

    // 关键修改：完整波浪
    // ratio=0(0点): sin(-π/2)=-1, 太阳在最下方（深夜）
    // ratio=0.25(6点): sin(0)=0, 太阳在中间（日出）
    // ratio=0.5(12点): sin(π/2)=1, 太阳在最高（正午）
    // ratio=0.75(18点): sin(π)=0, 太阳在中间（日落）
    // ratio=1(24点): sin(3π/2)=-1, 太阳回到最下方
    float wave = qSin(ratio * 2 * M_PI - M_PI / 2);
    int y = centerY - amplitude * wave;  // 注意减号，让上方是Y小

    return QPoint(x, y);
}

// 反推：根据鼠标Y位置，找最近的时间
float SunOrbit::timeFromPosition(const QPoint &pos) const
{
    // 简化：主要根据X位置判断时间
    float ratio = (pos.x() - m_orbitRect.left()) / (float)m_orbitRect.width();
    if (ratio < 0) ratio = 0;
    if (ratio > 1) ratio = 1;
    return ratio * 24.0f;
}

void SunOrbit::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 轨道区域
    m_orbitRect = rect().adjusted(40, 20, -40, -20);

    // 画正弦曲线（日轨线）
    // 画正弦曲线（日轨线）- 完整波浪
    painter.setPen(QPen(Qt::white, 2, Qt::DashLine));
    QPainterPath path;

    for (float h = 0; h <= 24.0f; h += 0.2f) {  // 更密集的点，曲线更平滑
        QPoint p = sunPosition(h);
        if (h == 0) {
            path.moveTo(p);
        } else {
            path.lineTo(p);
        }
    }
    painter.drawPath(path);

    // 画太阳（白色圆）
    QPoint sunPos = sunPosition(m_currentHour);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(QColor(255, 223, 186), 3));  // 淡黄色描边，3像素粗
    painter.drawEllipse(sunPos, 15, 15);
    // 画时间文字
    painter.setPen(Qt::white);
    painter.drawText(rect().bottomLeft() + QPoint(10, -10),
                    QString("时间: %1:00").arg((int)m_currentHour));
}

void SunOrbit::mousePressEvent(QMouseEvent *event)
{
    QPoint sunPos = sunPosition(m_currentHour);
    // 点击太阳附近才能拖动
    if (QLineF(event->pos(), sunPos).length() < 30) {
        m_isDragging = true;
    }
    if (QLineF(event->pos(), sunPos).length() < 30) {
        m_isDragging = true;
        m_returnTimer->stop();  // ← 新增：开始拖动时停止回归倒计时
    }
}

void SunOrbit::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging) {
        float newTime = timeFromPosition(event->pos());
        setTime(newTime);
    }
}

void SunOrbit::mouseReleaseEvent(QMouseEvent *event)
{
    m_isDragging = false;
    m_returnTimer->start(3000);
}
void SunOrbit::onReturnTimer()
{
    // 获取当前真实时间（小时 + 分钟/60）
    float currentHour = QTime::currentTime().hour() + QTime::currentTime().minute() / 60.0f;

    // 如果位置和真实时间不一样，就回归
    if (qAbs(m_currentHour - currentHour) > 0.05f) {
        setTime(currentHour);
    }
}


