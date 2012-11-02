#include <QtGui/QApplication>

#include "testframe.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestFrame w;
    w.show();
    
    return a.exec();
}
