#include <QtGui/QApplication>

#include "homewindow.h"

int main(int argc, char *argv[])
{

	#if QT_VERSION < 0x050000
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
		QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	#endif
	srand(time(0));
    QApplication a(argc, argv);
    HomeWindow* w = new HomeWindow();
    w->show();
    
    return a.exec();
}
