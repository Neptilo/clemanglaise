#include <QMouseEvent>
#include <QPainter>

#include "handwritingarea.h"

HandwritingArea::HandwritingArea(QWidget *parent)
     : QWidget(parent)
 {
     scribbling = false;
     resizeImage(&image, QSize(100, 100));
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
        drawLineTo(event->pos());
}

void HandwritingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}

void HandwritingArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    int rad = (1 / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void HandwritingArea::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);
     QRect dirtyRect = event->rect();
     painter.drawImage(dirtyRect, image, dirtyRect);
 }

void HandwritingArea::resizeImage(QImage *image, const QSize &newSize)
 {
     if (image->size() == newSize)
         return;

     QImage newImage(newSize, QImage::Format_RGB32);
     newImage.fill(qRgb(255, 255, 255));
     QPainter painter(&newImage);
     painter.drawImage(QPoint(0, 0), *image);
     *image = newImage;
 }
