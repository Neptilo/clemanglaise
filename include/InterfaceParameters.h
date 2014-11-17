#ifndef INTERFACEPARAMETERS_H
#define INTERFACEPARAMETERS_H

#include <QObject>

class InterfaceParameters : public QObject
{
    Q_OBJECT
public:
    InterfaceParameters();

    // reference width or height for widgets
    // proportional to window height
    static int widget_unit;

    static void update_widget_unit(QWidget *window);

private:
    // ratio between the height of the window and the size of a reference widget
    static const int window_widget_ratio = 10;
};

#endif // INTERFACEPARAMETERS_H
