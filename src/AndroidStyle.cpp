#include "include/AndroidStyle.h"

AndroidStyle::AndroidStyle() :
    QProxyStyle()
{
}

int AndroidStyle::pixelMetric(PixelMetric which,
                             const QStyleOption *option,
                             const QWidget *widget) const
{
    int metric = QProxyStyle::pixelMetric(which, option, widget);
    switch (which) {
    case PM_IndicatorWidth:
    case PM_IndicatorHeight:
        return 3*metric;
    default:
        return metric;
    }
}
