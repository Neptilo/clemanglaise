#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <QString>

QString ampersand_escape(const QString &string);
QString ampersand_unescape(const QString &string);
QString number_to_accent(const QString letter, int accent_number);
QString numbers_to_accents(const QString &string);

#endif // STRING_UTILS_H
