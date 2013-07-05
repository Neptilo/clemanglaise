#ifndef TEST_H
#define TEST_H

#include<QString>

class Test{

private:
     QString name;
     QString src;
     QString dst;
	 bool remote;

public:
    explicit Test(const QString &name, const QString &src, const QString &dst, bool remote=true);
    const QString& getName() const;
    const QString& getSrc();
    const QString& getDst();
	bool isRemoteWork() const;
	void setRemoteWork(bool where);
};

#endif // TEST_H
