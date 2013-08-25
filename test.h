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
    bool withThemes;
	QSet<QString> themes;

public:
    bool asked_pronunciation;

    explicit Test(int id, const QString &name, const QString &src, const QString &dst, bool remote=true, bool withThemes=false);
    int getId() const;
    const QString& getName() const;
    const QString& getSrc();
    const QString& getDst();
	bool isRemoteWork() const;
	void setRemoteWork(bool where);
    bool hasThemes() const;
	void addTheme(const QString& theme);
	QSet<QString>& getThemes();
};

#endif // TEST_H
