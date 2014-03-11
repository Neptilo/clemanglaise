#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <QHash>
#include <QString>
#include <QStringList>

extern const QHash<QString, QString> maphash;
QString ampersand_escape(const QString &string);
QString colon_unescape(const QString &string);
QString ampersand_unescape(const QString &string);
QString number_to_accent(const QString letter, int accent_number);
QString numbers_to_accents(const QString &string);
QString getImgPath(const QString & img);
QHash<QString, QString> mapping();
QString X2IPA(const QString &string);
QString kirshenbaum2IPA(const QString &string);
bool isKirshenbaum(const QString& string);

// returns the list of trimmed element of the input list
QStringList trimmed(const QStringList &list);

#endif // STRING_UTILS_H
