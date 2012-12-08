#ifndef TEST_H
#define TEST_H

#include<QString>

class Test{

private:
     QString name;
     QString src;
     QString dst;

public:
    explicit Test(const QString &name, const QString &src, const QString &dst);
    const QString& getName() const;
    const QString& getSrc();
    const QString& getDst();
};

#endif // TEST_H
