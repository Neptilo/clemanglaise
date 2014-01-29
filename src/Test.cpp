#include <QObject>
#include <QStringList>

#include "Test.h"

Test::Test(int id, const QString& name, const QString& src, const QString& dst, const bool remote) {
    this->id = id;
    this->name = name;
    this->src = src;
    this->dst = dst;
    this->remote = remote;
}

int Test::get_id() const
{
    return id;
}

QString Test::get_name() const{
    return name;
}

QString Test::get_src() const{
    return src;
}

QString Test::get_dst() const{
    return dst;
}

bool Test::is_remote_work() const {
	return remote;
}

void Test::set_remote_work(bool where) {
    this->remote = where;
}

void Test::add_theme(const QString& theme) {
	this->themes.insert(theme);
}

QSet<QString> Test::get_themes() const{
	return this->themes;
}
