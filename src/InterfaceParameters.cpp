#include "InterfaceParameters.h"

#include <QApplication>
#include <QDebug>
#include <QWidget>

InterfaceParameters::InterfaceParameters()
{
    //update_widget_unit();
}

void InterfaceParameters::update_widget_unit(QWidget *window)
{
    widget_unit = window->height()/window_widget_ratio;
}

int InterfaceParameters::widget_unit;
