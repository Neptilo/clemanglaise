#ifndef TEST_H
#define TEST_H

#include<QString>
#include <QSet>

class Test{

private:
    int id;
    QString name;
    QString src;
    QString dst;
    bool remote;
	QSet<QString> themes;

public:
    explicit Test(int id, const QString &name, const QString &src, const QString &dst, bool remote=true);
    int get_id() const;
    QString get_name() const;
    QString get_src() const;
    QString get_dst() const;
    QSet<QString> get_themes() const;
    bool is_remote_work() const;
    void set_remote_work(bool where);
    void add_theme(const QString& theme);
};

#endif // TEST_H
