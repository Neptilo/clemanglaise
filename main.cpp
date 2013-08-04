#if QT_VERSION < 0x050000
#include <QtGui/QApplication>
#else
#include <QtWidgets/QApplication>
#endif
#include <ctime>
#include <QPushButton>
#include "homewindow.h"
int main(int argc, char *argv[])
{

	#if QT_VERSION < 0x050000
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
		QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	#endif
	
	srand(time(0));
    QApplication a(argc, argv);
    HomeWindow* w = new HomeWindow();
    w->show();

	return a.exec();
}
