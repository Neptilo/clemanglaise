#include "StringUtilsTests.h"

#include <QtTest>

#include "../include/string_utils.h"

StringUtilsTests::StringUtilsTests(QObject *parent) :
    QObject(parent)
{
}

void StringUtilsTests::testNumbersToAccents()
{
    QCOMPARE(numbers_to_accents("kong4r"), QString("kòngr"));
    QCOMPARE(numbers_to_accents("kong4rkong4r"), QString("kòngrkòngr"));
    QCOMPARE(numbers_to_accents("er2zi"), QString("érzi"));
    QCOMPARE(numbers_to_accents("dangran"), QString("dangran"));
    QCOMPARE(numbers_to_accents("dang'ran"), QString("dang'ran"));
    QCOMPARE(numbers_to_accents("xian1"), QString("xiān"));
    QCOMPARE(numbers_to_accents("xi'an1"), QString("xi'ān"));
}

void StringUtilsTests::testSeparatePinyin()
{
    QCOMPARE(separate_pinyin("kòngr"), QString("kòng r"));
    QCOMPARE(separate_pinyin("kòngrkòngr"), QString("kòng r kòng r"));
    QCOMPARE(separate_pinyin("érzi"), QString("ér zi"));
    QCOMPARE(separate_pinyin("dangran"), QString("dang ran"));
    QCOMPARE(separate_pinyin("dang'ran"), QString("dang ran"));
    QCOMPARE(separate_pinyin("xiān"), QString("xiān"));
    QCOMPARE(separate_pinyin("xi'ān"), QString("xi ān"));
    QCOMPARE(separate_pinyin("àiren"), QString("ài ren"));
    QCOMPARE(separate_pinyin("jiāná"), QString("jiā ná"));
    QCOMPARE(separate_pinyin("huir"), QString("hui r"));
    QCOMPARE(separate_pinyin("zher"), QString("zhe r"));
}

void StringUtilsTests::testAsciiToDin()
{
    QCOMPARE(ASCII_to_DIN("ab it u2thj7khddhr,zsshSDTZ3ghfqklmnhw al-y 2ah aaiiuu."),
             QString("ʾab ʾit ʾuʾṯǧḥḫdḏr,zsšṣḍṭẓʿġfqklmnhw ʾal-y ʾah ʾāīū."));
    QCOMPARE(ASCII_to_DIN("/r,l a-*y.", false),
             QString("rl ay"));
    // A string already in DIN must not change
    QCOMPARE(ASCII_to_DIN("ʿh.(, ġī;"), QString("ʿh.(, ġī;"));
}

void StringUtilsTests::testAsciiToPolish()
{
    // Single comprehensive test: mix markers, plain words, spaces, lowercase and uppercase
    QCOMPARE(ASCII_to_Polish("s'pan S'ten z.L/energia a;e;O'C'N''x ąę"),
             QString::fromUtf8("śpan Śten żŁenergia ąęÓĆŃ'x ąę"));
}
