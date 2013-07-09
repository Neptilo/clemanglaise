#ifndef HANDWRITINGAREA_H
#define HANDWRITINGAREA_H

#include <QWidget>
#include <QImage>

class HandwritingArea : public QWidget
{
    Q_OBJECT
public:
    explicit HandwritingArea(QWidget *parent = 0);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    bool scribbling;
    QImage image;
    QPoint lastPoint;

signals:
    
public slots:
    
};

#endif // HANDWRITINGAREA_H
