#ifndef NETWORKREPLYREADER_H
#define NETWORKREPLYREADER_H

#include <QObject>

class QNetworkCookieJar;
class QNetworkAccessManager;

class NetworkReplyReader : public QObject
{
    Q_OBJECT

public:
    explicit NetworkReplyReader(QObject *parent = nullptr);
    static QNetworkCookieJar* cookie_jar;
    static QNetworkAccessManager* nam;
    static const QString api_url;
    static void initialize();
    void print(QString s);

signals:
    
public slots:
    void read_reply();
    
};

#endif // NETWORKREPLYREADER_H
