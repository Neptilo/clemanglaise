#ifndef ANDROIDSTYLE_H
#define ANDROIDSTYLE_H

#include <QProxyStyle>

class AndroidStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit AndroidStyle();

    int pixelMetric(PixelMetric which, const QStyleOption *option, const QWidget *widget) const;

signals:

public slots:

};

#endif // ANDROIDSTYLE_H
