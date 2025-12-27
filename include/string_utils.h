#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <QHash>
#include <QString>
#include <QStringList>

extern const QHash<QString, QString> kirshenbaum_IPA_hash;
QString toTitleCase(const QString &string);
QString ampersand_escape(const QString &string);
QStringList ampersand_escape(const QStringList &list);
QString colon_unescape(const QString &string);
QString ampersand_unescape(const QString &string);
QStringList ampersand_unescape(const QStringList &list);
QString remove_diacritics(const QString &string);
QStringList remove_diacritics(const QStringList &list);

// from a pinyin letter and the number of a Mandarin tone, return the corresponding accented Unicode character
QChar number_to_accent(const QChar& letter, int accent_number);

// transcribes a pinyin pronunciation in number format or with accents into the pronunciation with accents only
// If `sep` is not empty, replaces all non-alphanumeric characters between syllables by `sep`
QString numbers_to_accents(const QString &string, const QString &sep = "");

// separates the syllables of a pinyin pronunciation
// replaces all non-alphanumeric characters between syllables by `sep`, a space by default
QString separate_pinyin(const QString &string, const QString &sep = " ");

QHash<QString, QString> make_Kirshenbaum_IPA_hash();
QHash<QString, QString> make_ASCII_DIN_hash();

QString ASCII_to_DIN(const QString &string, bool keepPunctuation = true);

// converts ASCII + simple markers to Polish diacritics
// Supported markers:
//  - Acute: s' z' c' n' o' (and uppercase)
//  - Dot: z. (and Z.) -> ż/Ż
//  - Slash: l/ (and L/) -> ł/Ł
//  - Ogonek: a; e; (and A; E;) -> ą/Ą, ę/Ę
QString ASCII_to_Polish(const QString &string);

QString kirshenbaum2IPA(const QString &string);
bool isKirshenbaum(const QString& string);

// returns the list of trimmed element of the input list
QStringList trimmed(const QStringList &list);

// returns a reversed copy of the string
QString reverse(const QString &str);

#endif // STRING_UTILS_H
