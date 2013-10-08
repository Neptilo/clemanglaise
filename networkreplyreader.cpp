#include <iostream>

#include <QObject>
#include <QtNetwork>

#include "networkreplyreader.h"
#include "homewindow.h"

NetworkReplyReader::NetworkReplyReader(QObject *parent) :
    QObject(parent)
{
}

QNetworkCookieJar* NetworkReplyReader::cookie_jar = new QNetworkCookieJar(0);

void NetworkReplyReader::read_reply(QNetworkReply* reply)
{
    reply->deleteLater();
    if(reply->readAll().toInt()){
        print(tr("Connection established"));
        HomeWindow* w = new HomeWindow(true);
        w->show();
    }else{
        print(tr("Wrong password"));
        qApp->exit();
    }
}

void NetworkReplyReader::print(QString s){
#if defined(Q_OS_WIN)
    std::cout << s.toLocal8Bit().constData() << std::endl;
#else
    std::cout << s.toUtf8().constData() << std::endl;
#endif
}
