#include <QObject>
#include <QStringList>

#include "test.h"

Test::Test(int id, const QString& name, const QString& src, const QString& dst, const bool remote) {
    this->id = id;
    this->name = name;
    this->src = src;
    this->dst = dst;
    this->remote = remote;

    // List of languages for which we asked for the pronunciation
    QStringList list;
    list << "ja" << "zh";
    this->asked_pronunciation = list.contains(this->dst);
}

int Test::getId() const
{
    return id;
}

const QString& Test::getName() const{
    return name;
}

const QString &Test::getSrc(){
    return src;
}

const QString &Test::getDst(){
    return dst;
}

bool Test::isRemoteWork() const {
	return remote;
}

void Test::setRemoteWork(bool where) {
    this->remote = where;
}

void Test::addTheme(const QString& theme) {
	this->themes.insert(theme);
}

QSet<QString>& Test::getThemes() {
	return this->themes;
}
