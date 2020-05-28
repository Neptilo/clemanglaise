#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QHash>

#include "Test.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    bool add_list(const QString &name, const QString &src, const QString &dst, const QString &flag, int &test_id);
    bool add_list(const QString &name, const QString &src, const QString &dst, const QString &flag);
    bool add_word(const QHash<QString, QString> &word_data);
    bool add_word(const QHash<QString, QString> &word_data, const QList<int> selected_tags);
    bool add_tag(const QString &tag);
    bool add_tag(const QString &tag, int &tag_id);
    bool create_list_table();
    bool create_word_table();
    bool create_tag_table();
    bool create_word_tag_table();
    bool delete_list(int test_id);
    bool delete_word(const int& id);

    // find word entry having the lowest score
    bool find_lowest(int test_id, QHash<QString, QString> &word_data, QList<int> tag_ids);
    bool find_tags(QStringList& reply_list);
    bool find_used_tags(int test_id, QStringList& reply_list);
    QList<Test> get_lists();
    bool open_db();
    QString pop_last_error();
    bool search(int test_id, const QString& expr, const QList<int> selected_tags, QStringList& reply_list);
    bool get_tags(int word_id, QStringList &word_tags);
    bool get_tags_id(int word_id, QStringList &word_tags_id);
    bool set_score(int id, const int& correct);
    bool update_word(const QHash<QString, QString> &word_data);
    bool update_word(const QHash<QString, QString> &word_data, const QList<int> selected_tags);
    bool find_duplicates(int test_id, const QString &word, QStringList &reply_keys, QList<QStringList> &reply_values);

    // sets res to the number of rows in the word list identified by test_id
    bool count(int test_id, int &res);

private:
    QString last_error;

signals:

public slots:

};

#endif // DATABASEMANAGER_H
