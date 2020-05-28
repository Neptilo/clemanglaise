#ifndef NETWORKREPLYREADER_H
#define NETWORKREPLYREADER_H

#include <QObject>
#include <QtNetwork>

class NetworkReplyReader : public QObject
{
    Q_OBJECT

public:
    explicit NetworkReplyReader(QObject *parent = nullptr);
    static QNetworkCookieJar* cookie_jar;
    static QNetworkAccessManager* nam;
    void print(QString s);

signals:
    
public slots:
    void read_reply();
    
};

#endif // NETWORKREPLYREADER_H
