#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QString>
#include <QSet>

class Test: public QObject
{
private:
    int id;
    QString name;
    QString src;
    QString dst;
    QString flag;
    bool remote;

public:
    explicit Test(int id, const QString &name, const QString &src, const QString &dst, const QString &flag, bool remote = true, QObject *parent = nullptr);
    explicit Test(const Test &other, QObject *parent = nullptr);
    int get_id() const;
    QString get_name() const;
    QString get_src() const;
    QString get_dst() const;
    QString get_flag() const;
    bool is_remote() const;
    void set_remote(bool remote);
    Test &operator= ( const Test &other );
};

#endif // TEST_H
