#ifndef NETWORKREPLYREADER_H
#define NETWORKREPLYREADER_H

#include <QObject>
#include <QtNetwork>

class NetworkReplyReader : public QObject
{
    Q_OBJECT

public:
    explicit NetworkReplyReader(QObject *parent = 0);
    static QNetworkCookieJar* cookie_jar;
    void print(QString s);

signals:
    
public slots:
    void read_reply(QNetworkReply* reply);
    
};

#endif // NETWORKREPLYREADER_H
