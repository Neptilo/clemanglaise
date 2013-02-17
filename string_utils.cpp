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
            return "&#465;";
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
    }else return letter;
}

QString numbers_to_accents(const QString &string){

    // Capture syllables
    QRegExp syllable_rx("([bcdfghj-np-tw-z]h?[iu]?)([āēīōūǖáéíóúǘǎěǐǒǔǚàèìòùǜaeiouüv])([iounr]?g?)(\\d?)");
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
