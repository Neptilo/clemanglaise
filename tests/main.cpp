#include <QtTest>

#include "StringUtilsTests.h"

int main(int argc, char *argv[])
{
    StringUtilsTests sut;
    // Return the runner's result: discarding it makes every run "pass".
    return QTest::qExec(&sut, argc, argv);
}
