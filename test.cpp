#include <QObject>

#include "test.h"

Test::Test(const QString& name, const QString& src, const QString& dst){
    this->name = name;
    this->src = src;
    this->dst = dst;
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
