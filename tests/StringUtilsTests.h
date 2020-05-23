#ifndef STRINGUTILSTESTS_H
#define STRINGUTILSTESTS_H

#include <QObject>

class StringUtilsTests : public QObject
{
    Q_OBJECT
public:
    explicit StringUtilsTests(QObject *parent = 0);

signals:

private slots:
    void testNumbersToAccents();
    void testSeparatePinyin();
};

#endif // STRINGUTILSTESTS_H
