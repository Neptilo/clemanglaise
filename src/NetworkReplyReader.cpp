#include "NetworkReplyReader.h"

#include <iostream>

#include <QObject>
#include <QNetworkCookieJar>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QApplication> // For qApp

#include "HomeView.h"

NetworkReplyReader::NetworkReplyReader(QObject *parent) :
    QObject(parent)
{
    // By default, nam takes ownership of the cookie jar.
    nam->setCookieJar(cookie_jar);
}

QNetworkCookieJar* NetworkReplyReader::cookie_jar = nullptr;

QNetworkAccessManager* NetworkReplyReader::nam = nullptr;

#ifdef QT_DEBUG
const QString NetworkReplyReader::api_url = "https://localhost/php/clemanglaise/";
#else
const QString NetworkReplyReader::api_url = "https://neptilo.com/php/clemanglaise/";
#endif

void NetworkReplyReader::initialize()
{
    if (!cookie_jar)
        cookie_jar = new QNetworkCookieJar();
    if (!nam)
        nam = new QNetworkAccessManager();
}

void NetworkReplyReader::read_reply()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
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
