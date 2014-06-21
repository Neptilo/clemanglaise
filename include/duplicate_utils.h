#ifndef DUPLICATE_UTILS_H
#define DUPLICATE_UTILS_H

#include <QHash>
#include <QList>
#include <QStringList>

// returns the indice of the closest match to word_data in duplicate_values
int find_best_duplicate(
        const QHash<QString, QString> &word_data,
        const QStringList &duplicate_keys,
        const QList<QStringList> &duplicate_values
        );

QHash<QString, QString> merge_word(
        const QHash<QString, QString> &left_word_data,
        const QHash<QString, QString> &right_word_data
        );

QString merge_string(
        const QString &left_string,
        const QString &right_string,
        const QRegExp &split_sep,
        const QString &join_sep);

#endif // DUPLICATE_UTILS_H
