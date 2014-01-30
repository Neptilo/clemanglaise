#include "Test.h"

#include <QDebug>
#include <QObject>
#include <QStringList>

Test::Test(int id, const QString &name, const QString &src, const QString &dst, const bool remote, QObject *parent) :
    QObject(parent),
    id(id),
    name(name),
    src(src),
    dst(dst),
    remote(remote)
{
}

Test::Test(const Test& other) :
    QObject(other.parent()),
    id(other.id),
    name(other.name),
    src(other.src),
    dst(other.dst),
    remote(other.remote)
{
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

void Test::set_remote_work(bool remote) {
    this->remote = remote;
}

void Test::add_theme(const QString& theme) {
    this->themes.insert(theme);
}

Test &Test::operator=(const Test &other)
{
    id = other.id;
    name = other.name;
    src = other.src;
    dst = other.dst;
    remote = other.remote;
    setParent(other.parent());
    return *this;
}

QSet<QString> Test::get_themes() const{
	return this->themes;
}
