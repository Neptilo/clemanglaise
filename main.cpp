#include <QtGui/QApplication>

#include "homewindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeWindow* w = new HomeWindow();
    w->show();
    
    return a.exec();
}
