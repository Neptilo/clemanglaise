#include <QString>
#include <QRegExp>

#include "string_utils.h"

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

/**
 * replace all &#number; by corresponding ascii letter
 * @par string: the string we want to handle
 */
QString colon_unescape(const QString &string){
    QRegExp rx0("[^:]*");
    QRegExp rx(":([^:]*)");
    QString res;
    int pos = 0;
    if((pos = rx0.indexIn(string, pos)) != -1){
        pos += rx0.matchedLength();
        res += rx0.cap(0);
        while ((pos = rx.indexIn(string, pos)) != -1) {
            pos += rx.matchedLength();
            res += rx.cap(1) + rx.cap(2);
        }
    }
    return res;
}

/**
 * replace all: by nothing, because colon is a reserved caracter while
 * parsing a file
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

QString numbers_to_accents(const QString &string){

    // Capture syllables
    QRegExp syllable_rx("([bcdfgj-np-tw-z]?h?[iu]?)([\\x0101\\x0113\\x012B\\x014D\\x016B\\x01D6\\x00E1\\x00E9\\x00ED\\x00F3\\x00FA\\x01D8\\x01CE\\x011B\\x01D0\\x01D2\\x01D4\\x01DA\\x00E0\\x00E8\\x00EC\\x00F2\\x00F9\\x01DCaeiou\\x00FCv])([iounr]?g?)(\\d?)");
    QString res;
    int pos = 0;
    while ((pos = syllable_rx.indexIn(string, pos)) != -1) {
        pos += syllable_rx.matchedLength();

        // Generate new string
        QString nucleus = number_to_accent(syllable_rx.cap(2), syllable_rx.cap(4).toInt());
        res += (syllable_rx.cap(1))+nucleus+(syllable_rx.cap(3));
    }

    return res;
}

QString getImgPath(const QString& img) {
	QString path("");
#if defined(Q_WS_WIN) && !defined(__CYGWIN__)
	path = ":/img/win/"; 
#elif defined(Q_WS_MACX)
	path = ":/img/macx/";
#else
	path = ":/img/";
#endif
	path += img;
	return path;
}
