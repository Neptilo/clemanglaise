#include "InterfaceParameters.h"

#include <QApplication>
#include <QDebug>
#include <QWidget>

InterfaceParameters::InterfaceParameters()
{
}

void InterfaceParameters::update_widget_unit(QWidget *window)
{
    widget_unit = window->height()/window_widget_ratio;
}

int InterfaceParameters::widget_unit;

Qt::ScreenOrientation InterfaceParameters::orientation = Qt::PortraitOrientation;
