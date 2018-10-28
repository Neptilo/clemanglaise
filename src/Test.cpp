#include "Test.h"

#include <QDebug>
#include <QObject>
#include <QStringList>

Test::Test(int id, const QString &name, const QString &src, const QString &dst, const QString &flag, const bool remote, QObject *parent) :
    QObject(parent),
    id(id),
    name(name),
    src(src),
    dst(dst),
    flag(flag),
    remote(remote)
{
}

Test::Test(const Test& other, QObject *parent) :
    QObject(parent),
    id(other.id),
    name(other.name),
    src(other.src),
    dst(other.dst),
    flag(other.flag),
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

QString Test::get_flag() const
{
    return flag;
}

bool Test::is_remote() const {
    return remote;
}

void Test::set_remote(bool remote) {
    this->remote = remote;
}

Test &Test::operator=(const Test &other)
{
    id = other.id;
    name = other.name;
    src = other.src;
    dst = other.dst;
    flag = other.flag;
    remote = other.remote;
    setParent(other.parent());
    return *this;
}
