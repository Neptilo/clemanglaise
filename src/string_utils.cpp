#include <QRegExp>
#include <QDebug>

#include "string_utils.h"

const QHash<QString, QString> maphash = mapping();

QString toTitleCase(const QString& str)
{
    return str.left(1).toUpper() + str.mid(1).toLower();
}

QString ampersand_escape(const QString &string) {
    QString encoded;
    for(int i=0;i<string.size();++i) {
        QChar ch = string.at(i);
        if(ch.unicode() > 127)
            encoded += QString("&#%1;").arg((int)ch.unicode());
        else
            encoded += ch;
    }
    return encoded;
}

QStringList ampersand_escape(const QStringList &list)
{
    QStringList ret;
    for(int i = 0; i < list.size(); ++i)
        ret << ampersand_escape(list.at(i));
    return ret;
}

/**
 * replace all: by : in IPA, because colon is a reserved caracter while
 * parsing a file
 * @par string: the string we want to handle
 */
QString colon_unescape(const QString &string){
    QRegExp rx0("[^:]*");
    QRegExp rx(":([^:]*)");
    QString res("");
    int pos = 0;
    if((pos = rx0.indexIn(string, pos)) != -1){
        pos += rx0.matchedLength();
        res += rx0.cap(0);
        while ((pos = rx.indexIn(string, pos)) != -1) {
            pos += rx.matchedLength();
            res += QString::fromUtf8("\u02D0")+rx.cap(1);
        }
    }
    return res;
}

/**
 * replace all &#number; by corresponding ascii letter
 * @par string: the string we want to handle
 */
QString ampersand_unescape(const QString &string){
    QRegExp rx0("[^&]*");
    QRegExp rx("&#(\\d*);([^&]*)");
    QString res;
    int pos = 0;
    if((pos = rx0.indexIn(string, pos)) != -1){
        pos += rx0.matchedLength();
        res += rx0.cap(0);
        while ((pos = rx.indexIn(string, pos)) != -1) {
            pos += rx.matchedLength();
            res += QChar(rx.cap(1).toInt())+rx.cap(2);
        }
    }
    return res;
}

QStringList ampersand_unescape(const QStringList &list)
{
    QStringList ret;
    for(int i = 0; i < list.size(); ++i)
        ret << ampersand_unescape(list.at(i));
    return ret;
}

QString diacritic_letters;
QStringList no_diacritic_letters;

QString remove_diacritics(const QString &string){
    if(diacritic_letters.isEmpty()){
        diacritic_letters = QString::fromUtf8(
                    "ÀÁÂÃÄÅĄÆÇĆÈÉÊËĘÌÍÎÏŁÑŃÒÓÔÕÖØŒŠŚÙÚÛÜÝŸŽŹŻ"
                    "àáâãäåąæçćèéêëęìíîïłñńòóôõöøœšśùúûüýÿžźż");
        no_diacritic_letters
                <<"A"<<"A"<<"A"<<"A"<<"A"<<"A"<<"A"<<"AE"<<"C"<<"C"<<"E"<<"E"<<
                  "E"<<"E"<<"E"<<"I"<<"I"<<"I"<<"I"<<"L"<<"N"<<"N"<<"O"<<"O"<<
                  "O"<<"O"<<"O"<<"O"<<"OE"<<"S"<<"S"<<"U"<<"U"<<"U"<<"U"<<"Y"<<
                  "Y"<<"Z"<<"Z"<<"Z"<<
                  "a"<<"a"<<"a"<<"a"<<"a"<<"a"<<"a"<<"ae"<<"c"<<"c"<<"e"<<"e"<<
                  "e"<<"e"<<"e"<<"i"<<"i"<<"i"<<"i"<<"l"<<"n"<<"n"<<"o"<<"o"<<
                  "o"<<"o"<<"o"<<"o"<<"oe"<<"s"<<"s"<<"u"<<"u"<<"u"<<"u"<<"y"<<
                  "y"<<"z"<<"z"<<"z";
    }

    QString ret = "";
    for(int i = 0; i < string.length(); i++){
        QChar c = string[i];
        int dIndex = diacritic_letters.indexOf(c);
        if (dIndex < 0) {
            ret.append(c);
        }else{
            QString replacement = no_diacritic_letters[dIndex];
            ret.append(replacement);
        }
    }

    return ret;
}

QStringList remove_diacritics(const QStringList &list)
{
    QStringList ret;
    for(int i = 0; i < list.size(); ++i)
        ret << remove_diacritics(list.at(i));
    return ret;
}

// Don't use with ampersand_escape. It would be redundant and generate bugs.
QString number_to_accent(const QString letter, int accent_number){
    if(letter == "a"){
        switch(accent_number){
            case 1:
                return "&#257;";
            case 2:
                return "&#225;";
            case 3:
                return "&#462;";
            case 4:
                return "&#224;";
            default:
                return "a";
        }
    }else if(letter == "e"){
        switch(accent_number){
            case 1:
                return "&#275;";
            case 2:
                return "&#233;";
            case 3:
                return "&#283;";
            case 4:
                return "&#232;";
            default:
                return "e";
        }
    }else if(letter == "i"){
        switch(accent_number){
            case 1:
                return "&#299;";
            case 2:
                return "&#237;";
            case 3:
                return "&#464;";
            case 4:
                return "&#236;";
            default:
                return "i";
        }
    }else if(letter == "o"){
        switch(accent_number){
            case 1:
                return "&#333;";
            case 2:
                return "&#243;";
            case 3:
                return "&#466;";
            case 4:
                return "&#242;";
            default:
                return "o";
        }
    }else if(letter == "u"){
        switch(accent_number){
            case 1:
                return "&#363;";
            case 2:
                return "&#250;";
            case 3:
                return "&#468;";
            case 4:
                return "&#249;";
            default:
                return "u";
        }
    }else if(letter == "v"){
        switch(accent_number){
            case 1:
                return "&#470;";
            case 2:
                return "&#472;";
            case 3:
                return "&#474;";
            case 4:
                return "&#476;";
            default:
                return "&#252;";
        }
    }else return ampersand_escape(letter);
}

QString numbers_to_accents(const QString &string, const QString &sep){

    // Capture syllables
    QRegExp syllable_rx("([bcdfgj-np-tw-z]?h?[iu]?)([\\x0101\\x0113\\x012B\\x014D\\x016B\\x01D6\\x00E1\\x00E9\\x00ED\\x00F3\\x00FA\\x01D8\\x01CE\\x011B\\x01D0\\x01D2\\x01D4\\x01DA\\x00E0\\x00E8\\x00EC\\x00F2\\x00F9\\x01DCaeiou\\x00FCvr])([iounr]?g?)(\\d?)(\\W*)", Qt::CaseInsensitive);
    QString res, separation;
    int pos = 0;
    while ((pos = syllable_rx.indexIn(string, pos)) != -1) {
        res += separation;
        pos += syllable_rx.matchedLength();

        // Generate new string
        QString nucleus = number_to_accent(syllable_rx.cap(2), syllable_rx.cap(4).toInt());
        separation = (sep.isEmpty()) ? syllable_rx.cap(5) : sep;
        res += syllable_rx.cap(1)+nucleus+syllable_rx.cap(3);
    }

    return res;
}

QString separate_pinyin(const QString &string, const QString &sep)
{
    // It is necessary to reverse the regular expression,
    // otherwise "ana" would be split as "an a" instead of "a na".

    // backward regular expression of:
    // "[bcdfgj-np-tw-z]?h?[iu]?(&#x?\\d+;|[aeiouv])[ioun]?g?|r"
    QRegExp backward_syllable_rx("g?[ioun]?(;\\d+x?#&|[aeiouv])[iu]?h?[bcdfgj-np-tw-z]?|r", Qt::CaseInsensitive);
    int pos = 0;
    QStringList syllables;
    QRegExp e_rx("(e|&#(275|233|283|232);)", Qt::CaseInsensitive);
    while ((pos = backward_syllable_rx.indexIn(reverse(string), pos)) != -1) {
        pos += backward_syllable_rx.matchedLength();
        QString syllable(reverse(backward_syllable_rx.cap(0)));
        if (
                !syllables.isEmpty()
                && syllables.at(0) == QString("r")
                && e_rx.exactMatch(syllable))
            syllables.replace(0, syllable+syllables.at(0));
        else
            syllables.prepend(syllable);
    }
    return syllables.join(sep);
}

/**
 * mapping Kirshenbaum -- IPA
 */
QHash<QString, QString> mapping()
{
    QHash<QString, QString> hash;
    hash.insert("a", QString::fromUtf8("\u0061"));
    hash.insert("b", QString::fromUtf8("\u0062"));
    hash.insert("c", QString::fromUtf8("\u0063"));
    hash.insert("d", QString::fromUtf8("\u0064"));
    hash.insert("e", QString::fromUtf8("\u0065"));
    hash.insert("f", QString::fromUtf8("\u0066"));
    hash.insert("g", QString::fromUtf8("\u0261"));
    hash.insert("h", QString::fromUtf8("\u0068"));
    hash.insert("i", QString::fromUtf8("\u0069"));
    hash.insert("j", QString::fromUtf8("\u006A"));
    hash.insert("k", QString::fromUtf8("\u006B"));
    hash.insert("l", QString::fromUtf8("\u006C"));
    hash.insert("m", QString::fromUtf8("\u006D"));
    hash.insert("n", QString::fromUtf8("\u006E"));
    hash.insert("o", QString::fromUtf8("\u006F"));
    hash.insert("p", QString::fromUtf8("\u0070"));
    hash.insert("q", QString::fromUtf8("\u0071"));
    hash.insert("r", QString::fromUtf8("\u0279"));
    hash.insert("s", QString::fromUtf8("\u0073"));
    hash.insert("t", QString::fromUtf8("\u0074"));
    hash.insert("u", QString::fromUtf8("\u0075"));
    hash.insert("v", QString::fromUtf8("\u0076"));
    hash.insert("w", QString::fromUtf8("\u0077"));
    hash.insert("x", QString::fromUtf8("\u0078"));
    hash.insert("y", QString::fromUtf8("\u0079"));
    hash.insert("z", QString::fromUtf8("\u007A"));
    hash.insert("A", QString::fromUtf8("\u0251"));
    hash.insert("B", QString::fromUtf8("\u03B2"));
    hash.insert("C", QString::fromUtf8("\u00E7"));
    hash.insert("D", QString::fromUtf8("\u00F0"));
    hash.insert("E", QString::fromUtf8("\u025B"));
    hash.insert("G", QString::fromUtf8("\u0262"));
    hash.insert("H", QString::fromUtf8("\u0127"));
    hash.insert("I", QString::fromUtf8("\u026A"));
    hash.insert("J", QString::fromUtf8("\u025F"));
    hash.insert("L", QString::fromUtf8("\u029F"));
    hash.insert("M", QString::fromUtf8("\u0271"));
    hash.insert("N", QString::fromUtf8("\u014B"));
    hash.insert("O", QString::fromUtf8("\u0254"));
    hash.insert("P", QString::fromUtf8("\u03A6"));
    hash.insert("Q", QString::fromUtf8("\u0263"));
    hash.insert("R", QString::fromUtf8("\u0280"));
    hash.insert("S", QString::fromUtf8("\u0283"));
    hash.insert("T", QString::fromUtf8("\u03B8"));
    hash.insert("U", QString::fromUtf8("\u028A"));
    hash.insert("V", QString::fromUtf8("\u028C"));
    hash.insert("W", QString::fromUtf8("\u0153"));
    hash.insert("X", QString::fromUtf8("\u03C7"));
    hash.insert("Y", QString::fromUtf8("\u0058"));
    hash.insert("Z", QString::fromUtf8("\u0292"));
    hash.insert("?", QString::fromUtf8("\u0294"));
    hash.insert("@", QString::fromUtf8("\u0259"));
    hash.insert("&", QString::fromUtf8("\u00E6"));
    hash.insert("*", QString::fromUtf8("\u027E"));
    hash.insert(":", QString::fromUtf8("\u02D0"));
    hash.insert(";", QString::fromUtf8("\u02B2"));
    hash.insert("`", QString::fromUtf8("\u02BC"));
    hash.insert("'", QString::fromUtf8("\u02C8"));
    hash.insert(",", QString::fromUtf8("\u02CC"));
    hash.insert("~", QString::fromUtf8("\u0303"));
    hash.insert("b<trl>", QString::fromUtf8("\u0299"));
    hash.insert("r<trl>", QString::fromUtf8("\u0280"));
    hash.insert("<h>", QString::fromUtf8("\u02B0"));
    hash.insert("<o>", QString::fromUtf8("\u02DA"));
    hash.insert("<r>", QString::fromUtf8("\u02B3"));
    hash.insert("<w>", QString::fromUtf8("\u02B7"));
    hash.insert("<?>", QString::fromUtf8("\u02B1"));
    hash.insert("p!", QString::fromUtf8("\u0298"));
    hash.insert("t!", QString::fromUtf8("\u0287"));
    hash.insert("c!", QString::fromUtf8("\u0297"));
    hash.insert("l!", QString::fromUtf8("\u0296"));
    hash.insert("k!", QString::fromUtf8("\u029E"));
    hash.insert("r<lbd>", QString::fromUtf8("\u028B"));
    hash.insert("b`", QString::fromUtf8("\u0253"));
    hash.insert("d`", QString::fromUtf8("\u0257"));
    hash.insert("J`", QString::fromUtf8("\u0284"));
    hash.insert("g`", QString::fromUtf8("\u0260"));
    hash.insert("q`", QString::fromUtf8("\u02A0"));
    hash.insert("G`", QString::fromUtf8("\u029B"));
    hash.insert("s<lat>", QString::fromUtf8("\u026C"));
    hash.insert("z<lat>", QString::fromUtf8("\u026E"));
    hash.insert("*<lat>", QString::fromUtf8("\u027A"));
    hash.insert("n.", QString::fromUtf8("\u0273"));
    hash.insert("A.", QString::fromUtf8("\u0252"));
    hash.insert("t.", QString::fromUtf8("\u0288"));
    hash.insert("d.", QString::fromUtf8("\u0256"));
    hash.insert("s.", QString::fromUtf8("\u0282"));
    hash.insert("z.", QString::fromUtf8("\u0290"));
    hash.insert("I.", QString::fromUtf8("\u026D"));
    hash.insert("&.", QString::fromUtf8("\u0276"));
    hash.insert("*.", QString::fromUtf8("\u027D"));
    hash.insert("@.", QString::fromUtf8("\u0275"));
    hash.insert("C<vcd>", QString::fromUtf8("\u029d"));
    hash.insert("H<vcd>", QString::fromUtf8("\u0295"));
    hash.insert("j<rnd>", QString::fromUtf8("\u0265"));
    hash.insert("I^", QString::fromUtf8("\u028E"));
    hash.insert("j<vel>`", QString::fromUtf8("\u0270"));
    hash.insert("w<vls>", QString::fromUtf8("\u028D"));
    hash.insert("n\"", QString::fromUtf8("\u0274"));
    hash.insert("g\"", QString::fromUtf8("\u0281"));
    hash.insert("r\"", QString::fromUtf8("\u0280"));
    hash.insert("i\"", QString::fromUtf8("\u0268"));
    hash.insert("u\"", QString::fromUtf8("\u0289"));
    hash.insert("V\"", QString::fromUtf8("\u025C"));
    hash.insert("O\"", QString::fromUtf8("\u025E"));
    hash.insert("h<?>", QString::fromUtf8("\u0266"));
    hash.insert("@<umd>\"", QString::fromUtf8("\u0258"));
    hash.insert("R<umd>\"", QString::fromUtf8("\u025D"));
    hash.insert("u-", QString::fromUtf8("\u026F"));
    hash.insert("o-", QString::fromUtf8("\u0264"));
    return hash;
}

QString X2IPA(const QString &string)
{
    QString res;
    if(maphash.contains(string))
    {
        res = maphash.value(string);
    } else  {
        res = string;
    }
    return res;
}

QString kirshenbaum2IPA(const QString &string){

    // Capture phonemes
    QRegExp rx("(([a-zA-Z@&*?',])(<[a-z?]{1,3}>)?([\";`!\\-.^~]?))(:?)");
    QString res="";
    int pos = 0;
    // Match rx in string from pos and return final position or -1 if match failed
    while (!rx.isEmpty() && (pos = rx.indexIn(string, pos)) != -1) {
        pos += rx.matchedLength();

        // Generate new string
        if (maphash.contains(rx.cap(1))) {
            res += X2IPA(rx.cap(1));
        } else if(maphash.contains(rx.cap(2)+rx.cap(3))) {
            res += X2IPA(rx.cap(2) + rx.cap(3)) + X2IPA(rx.cap(4));
        }else {
            res += X2IPA(rx.cap(2)) + X2IPA(rx.cap(3)) + X2IPA(rx.cap(4));
        }
        res += X2IPA(rx.cap(5));
    }

    return res;
}

bool isKirshenbaum(const QString& string){
    QRegExp rx("([a-zA-Z@&*?',]?(<[a-z?]{1,3}>)?[\":;`!\\-.^~]?)+");
    return rx.exactMatch(string);
}

QStringList trimmed(const QStringList &list)
{
    QStringList ret;
    for(int i = 0; i < list.size(); ++i)
        ret << list.at(i).trimmed();
    return ret;
}


QString reverse(const QString &str)
{
    QString ret(str);
    std::reverse(ret.begin(), ret.end());
    return ret;
}
