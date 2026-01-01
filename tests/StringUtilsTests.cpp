#include "StringUtilsTests.h"

#include <QtTest>

#include "../include/string_utils.h"

StringUtilsTests::StringUtilsTests(QObject *parent) :
    QObject(parent)
{
}

void StringUtilsTests::testNumbersToAccents()
{
    QCOMPARE(numbers_to_accents("kong4r"), QString("k&#242;ngr"));
    QCOMPARE(numbers_to_accents("kong4rkong4r"), QString("k&#242;ngrk&#242;ngr"));
    QCOMPARE(numbers_to_accents("er2zi"), QString("&#233;rzi"));
    QCOMPARE(numbers_to_accents("dangran"), QString("dangran"));
    QCOMPARE(numbers_to_accents("dang'ran"), QString("dang'ran"));
    QCOMPARE(numbers_to_accents("xian1"), QString("xi&#257;n"));
    QCOMPARE(numbers_to_accents("xi'an1"), QString("xi'&#257;n"));
    QCOMPARE(numbers_to_accents("r4"), QString("r"));
    QCOMPARE(numbers_to_accents("xyz1"), QString(""));
}

void StringUtilsTests::testSeparatePinyin()
{
    QCOMPARE(separate_pinyin("k&#242;ngr"), QString("k&#242;ng r"));
    QCOMPARE(separate_pinyin("k&#242;ngrk&#242;ngr"), QString("k&#242;ng r k&#242;ng r"));
    QCOMPARE(separate_pinyin("&#233;rzi"), QString("&#233;r zi"));
    QCOMPARE(separate_pinyin("dangran"), QString("dang ran"));
    QCOMPARE(separate_pinyin("dang'ran"), QString("dang ran"));
    QCOMPARE(separate_pinyin("xi&#257;n"), QString("xi&#257;n"));
    QCOMPARE(separate_pinyin("xi'&#257;n"), QString("xi &#257;n"));
    QCOMPARE(separate_pinyin("&#224;iren"), QString("&#224;i ren"));
    QCOMPARE(separate_pinyin("ji&#257;n&#225;"), QString("ji&#257; n&#225;"));
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
