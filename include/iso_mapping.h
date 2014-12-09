#ifndef ISO_MAPPING_H
#define ISO_MAPPING_H

#include <QHash>
#include <QString>
#include <QStringList>

extern const QHash<QString, QString> COUNTRY_MAP;
extern const QHash<QString, QString> LANG_MAP;
extern const QStringList LANGUAGES;
QHash<QString, QString> create_countries_map();
QHash<QString, QString> create_lang_map();
QStringList get_languages(const QHash<QString, QString>& lang_map);
#endif  //ISO_MAPPING_H
