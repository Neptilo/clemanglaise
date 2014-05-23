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
    bool remote;
	QSet<QString> themes;

public:
    explicit Test(int id, const QString &name, const QString &src, const QString &dst, bool remote = true, QObject *parent = 0);
    explicit Test(const Test &other, QObject *parent = 0);
    int get_id() const;
    QString get_name() const;
    QString get_src() const;
    QString get_dst() const;
    QSet<QString> get_themes() const;
    bool is_remote() const;
    void set_remote(bool remote);
    void add_theme(const QString & theme);
    Test &operator= ( const Test &other );
};

#endif // TEST_H
