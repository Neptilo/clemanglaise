#include <QApplication>
#include <QSplashScreen>
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
	QPixmap pixmap("img/clemanglaise.png");
	QSplashScreen splash(pixmap);
	splash.show();
	a.processEvents();
	HomeWindow* w = new HomeWindow();
	w->show();
	splash.finish(w);

	return a.exec();
}
