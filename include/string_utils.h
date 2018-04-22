#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <QHash>
#include <QString>
#include <QStringList>

extern const QHash<QString, QString> maphash;
QString toTitleCase(const QString &string);
QString ampersand_escape(const QString &string);
QStringList ampersand_escape(const QStringList &list);
QString colon_unescape(const QString &string);
QString ampersand_unescape(const QString &string);
QStringList ampersand_unescape(const QStringList &list);
QString remove_diacritics(const QString &string);
QStringList remove_diacritics(const QStringList &list);
QString number_to_accent(const QString letter, int accent_number);

// transcribes a pinyin pronunciation in number format or with accents into the pronunciation with accents only
// If `sep` is not empty, replaces all non-alphanumeric characters between syllables by `sep`
QString numbers_to_accents(const QString &string, const QString &sep = "");

// separates the syllables of a pinyin pronunciation
// replaces all non-alphanumeric characters between syllables by `sep`, a space by default
QString separate_pinyin(const QString &string, const QString &sep = " ");

QString getImgPath(const QString & img);
QHash<QString, QString> mapping();
QString X2IPA(const QString &string);
QString kirshenbaum2IPA(const QString &string);
bool isKirshenbaum(const QString& string);

// returns the list of trimmed element of the input list
QStringList trimmed(const QStringList &list);

// returns a reversed copy of the string
QString reverse(const QString &str);

#endif // STRING_UTILS_H
