#include "NetworkReplyReader.h"

#include <iostream>

#include <QObject>
#include <QtNetwork>

#include "HomeView.h"

NetworkReplyReader::NetworkReplyReader(QObject *parent) :
    QObject(parent)
{}

QNetworkCookieJar* NetworkReplyReader::cookie_jar =
        new QNetworkCookieJar(nullptr);

void NetworkReplyReader::read_reply(QNetworkReply* reply)
{
    reply->deleteLater();
    if(reply->readAll().toInt()){
        print(tr("Connection established"));
        HomeView* w = new HomeView(true);
        w->show();
    }else{
        print(tr("Wrong password"));
        qApp->exit();
    }
}

void NetworkReplyReader::print(QString s){
#if defined(Q_OS_WIN) && !defined(__CYGWIN__)
    std::cout << s.toLocal8Bit().constData() << std::endl;
#else
    std::cout << s.toUtf8().constData() << std::endl;
#endif
}
