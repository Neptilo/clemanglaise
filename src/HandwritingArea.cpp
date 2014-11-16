#include <QMouseEvent>
#include <QPainter>

#include "HandwritingArea.h"

HandwritingArea::HandwritingArea(QSize size, QWidget *parent)
    : QWidget(parent)
{
    scribbling = false;
    image = QImage(size, QImage::Format_Mono);
    image.fill(1);
}

void HandwritingArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

void HandwritingArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        draw_line_to(event->pos());
}

void HandwritingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        draw_line_to(event->pos());
        scribbling = false;
    }
}

void HandwritingArea::draw_line_to(const QPoint &endPoint)
{
    QPainter painter(&image);
    int width = 8;
    painter.setPen(QPen(Qt::black, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    int rad = width/2+2;
    update(QRect(lastPoint, endPoint).adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void HandwritingArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}
