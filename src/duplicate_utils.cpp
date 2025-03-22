#include "duplicate_utils.h"

#include <QDebug>
#include <QSet>
#include <QRegularExpression>

#include "string_utils.h"

int find_best_duplicate(
        const QHash<QString, QString> &word_data,
        const QStringList &duplicate_keys,
        const QList<QStringList> &duplicate_values
        ){
    QStringList relevant_keys; // keys of relevant fields to compute similarity between imported entry and possible duplicate
    relevant_keys << "word" << "meaning";
    int best_similarity = 0; // Similarity is defined as the number of words and meanings in common between two entries.
    int best_match_ind(0);
    for(int i = 0; i < duplicate_values.size(); ++i){
        int similarity = 0;
        for(int j = 0; j < relevant_keys.size(); ++j){
            // get word list of imported entry
            QStringList import_word_list = trimmed(word_data[relevant_keys.at(j)].split(","));
            int key_ind = duplicate_keys.indexOf(relevant_keys.at(j));
            // get the word list and count elements in common with input word list
            QStringList dup_word_list = duplicate_values.at(i).at(key_ind).split(",");
            for(int k = 0; k < dup_word_list.size(); ++k){
                if(import_word_list.contains(dup_word_list.at(k).trimmed(), Qt::CaseInsensitive))
                    ++similarity;
            }
        }
        if(similarity > best_similarity){
            best_similarity = similarity;
            best_match_ind = i;
        }
    }
    return best_match_ind;
}

QHash<QString, QString> merge_word(
        const QHash<QString, QString> &left_word_data,
        const QHash<QString, QString> &right_word_data
        )
{
    QHash<QString, QString> merged_data;
    QHash<QString, QString>::const_iterator i;
    for (i = left_word_data.begin(); i != left_word_data.end(); ++i){
        if(i.key() != "id"){
            QRegularExpression split_sep;
            QString join_sep;
            if(i.key() == "comment" || i.key() == "example"){
                split_sep = QRegularExpression("<br ?/>", QRegularExpression::CaseInsensitiveOption);
                join_sep = "<br />";
            }else{
                split_sep = QRegularExpression(",");
                join_sep = ", ";
            }
            merged_data[i.key()] = merge_string(i.value(), right_word_data[i.key()], split_sep, join_sep);
        }
    }
    return merged_data;
}


QString merge_string(
    const QString &left_string,
    const QString &right_string,
    const QRegularExpression &split_sep,
    const QString &join_sep)
{
    const QStringList left_list = ampersand_escape(trimmed(left_string.split(
        split_sep, Qt::SkipEmptyParts)));
    auto left_set = QSet<QString>(left_list.begin(), left_list.end());
    const QStringList right_list = ampersand_escape(trimmed(right_string.split(
        split_sep, Qt::SkipEmptyParts)));
    auto right_set = QSet<QString>(right_list.begin(), right_list.end());
    left_set.unite(right_set);
    return QStringList(left_set.values()).join(join_sep);
}
