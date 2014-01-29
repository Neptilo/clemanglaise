#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QHash>

#include "Test.h"

const QString DBNAME="clemanglaise.db.sqlite";

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);
    bool add_list(const QString &name, const QString &src, const QString &dst);
    bool add_word(const QHash<QString, QString> &word_data);
    bool add_theme(const QString &theme);
    bool create_list_table();
    bool create_theme_table();
    bool delete_word(const QString& name, const int& id);
    bool find_lowest(const QString &name, QStringList &reply_list, int id_theme = -1);
	void find_themes(QStringList& reply_list);
    void find_used_themes(const QString& name, QStringList& reply_list);
    QList<Test> get_lists();
    bool open_db();
    QString pop_last_error();
    void search(const QString& name, const QString& expr, QStringList& reply_list);
    bool set_score(const QString& name, const QString& id, const int& correct);
    bool update_word(const QHash<QString, QString> &word_data);

private:
    QSqlDatabase db;
    QString last_error;

signals:

public slots:

};

#endif // DATABASEMANAGER_H
