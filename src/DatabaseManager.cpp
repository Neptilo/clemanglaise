#include "DatabaseManager.h"

#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMapIterator>

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
    if(!open_db()) {
        // "Error while opening the database";
        return;
    }
    create_list_table();
    create_theme_table();
}

bool DatabaseManager::add_list(const QString &name, const QString &src, const QString &dst) {
    QSqlQuery query;
    query.exec("BEGIN");
    bool commit_success;
    bool success = query.prepare("INSERT INTO lists "
                            "(name, src, dst) "
                            "VALUES(:name, :src, :dst)");
    query.bindValue(":name", name);
    query.bindValue(":src", src);
    query.bindValue(":dst", dst);
    success &= query.exec();
    if (!success){
        last_error = query.lastError().text();
        commit_success = query.exec("COMMIT");
        if (!commit_success){
            last_error = query.lastError().text();
        }
        return success&&commit_success;
    }

    // Get last inserted ID
    int test_id;
    query.exec("SELECT ID FROM lists ORDER BY ID DESC LIMIT 1");
    if (query.next())
        test_id = query.value(0).toInt();
    else{
        last_error = query.lastError().text();
        commit_success = query.exec("COMMIT");
        if (!commit_success){
            last_error = query.lastError().text();
        }
        return false;
    }

    /* We call the list table "words_<id>" with <id> its ID in the list table,
     * instead of calling it the name given by the user. */
    success = query.exec(QString("CREATE TABLE IF NOT EXISTS words_%1("
                 "id INTEGER PRIMARY KEY, "
                 "word VARCHAR(50) NOT NULL DEFAULT '', "
                 "meaning VARCHAR(100) NOT NULL DEFAULT '', "
                 "pronunciation VARCHAR(100) DEFAULT '', "
                 "nature VARCHAR(5) NOT NULL DEFAULT '', "
                 "comment TEXT DEFAULT '', "
                 "example TEXT DEFAULT '', "
                 "correctly_answered INTEGER NOT NULL DEFAULT 0, "
                 "asked INTEGER NOT NULL DEFAULT 0, "
                 "score DECIMAL(2,2) DEFAULT 0, "
                 "id_theme INTEGER NULL, "
                 "FOREIGN KEY(id_theme) REFERENCES themes(id) ON DELETE SET NULL)"
                 ).arg(test_id));
    if (!success){
        last_error = query.lastError().text();
        commit_success = query.exec("COMMIT");
        if (!commit_success){
            last_error = query.lastError().text();
        }
        return success&&commit_success;
    }
    success = query.exec("COMMIT");
    if (!success){
        last_error = query.lastError().text();
    }
    return success;
}

bool DatabaseManager::add_theme(const QString &theme) {
    QString str = "INSERT INTO themes "
            "(name) "
            "VALUES(:name)";
    QSqlQuery query;
    bool success = query.prepare(str);
    query.bindValue(":name", theme);

    success &= query.exec();
    if (!success)
        last_error = query.lastError().text();
    return success;
}

bool DatabaseManager::add_word(const QHash<QString, QString> &word_data)
{

    QSqlQuery query;
    QString test_id(word_data["test_id"]);
    bool success = query.prepare(QString("INSERT INTO words_%1(word, meaning, nature, pronunciation, comment, example, id_theme) "
                                         "VALUES(:word, :meaning, :nature, :pronunciation, :comment, :example, :theme)").arg(test_id));

    for(QHash<QString, QString>::const_iterator i = word_data.begin(); i != word_data.end(); ++i) {
        if(i.key() != "lang" && i.key() != "id") {
            query.bindValue(":"+i.key(), i.value());
        }
    }

    success &= query.exec();

    if(!success)
        last_error = query.lastError().text();


    return success;
}

bool DatabaseManager::create_list_table()
{
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS lists("
                              "ID INTEGER PRIMARY KEY, "
                              "name VARCHAR(32) UNIQUE NOT NULL, "
                              "src VARCHAR(3) NOT NULL, "
                              "dst VARCHAR(3) NOT NULL"
                              ")");
    if(!success)
        last_error = query.lastError().text();
    return success;
}

bool DatabaseManager::create_theme_table()
{
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS themes("
                              "ID INTEGER PRIMARY KEY, "
                              "name VARCHAR(32) UNIQUE NOT NULL"
                              ")");
    if(!success)
        last_error = query.lastError().text();
    return success;
}

bool DatabaseManager::delete_list(int test_id)
{
    QSqlQuery query;
    bool commit_success;
    query.exec("BEGIN");
    bool success = query.exec(QString("DELETE FROM lists "
                                      "WHERE ID=%1").arg(test_id));
    if (!success){
        last_error = query.lastError().text();
        commit_success = query.exec("COMMIT");
        if (!commit_success){
            last_error = query.lastError().text();
        }
        return success && commit_success;
    }
    if (query.numRowsAffected() != 1){
        last_error = tr("%1 list(s) seem to be affected by the deletion.").arg(query.numRowsAffected());
        commit_success = query.exec("COMMIT");
        if (!commit_success){
            last_error = query.lastError().text();
        }
        return false;
    }

    success = query.exec("COMMIT");
    if (!success){
        last_error = query.lastError().text();
    }
    return success;
}

bool DatabaseManager::delete_word(int test_id, const int& id) {
    QSqlQuery query;
    bool success = query.prepare(QString("DELETE FROM words_%1 WHERE id = :id").arg(test_id));
    query.bindValue(":id", id);

    success &= query.exec();
    if (!success)
        last_error = query.lastError().text();
    return success;
}

bool DatabaseManager::find_lowest(int test_id, QStringList &reply_list, int id_theme)
{
    // id_theme = 0 if no theme was selected
    QString cond = (id_theme > 0)? QString("id_theme = %1").arg(id_theme): "1";

    QSqlQuery query(QString("SELECT words_%1.ID, word, meaning, nature, comment, example, id_theme, pronunciation, score, name "
                                        "FROM words_%1 "
                                        "LEFT OUTER JOIN themes "
                                        "ON themes.ID = words_%1.id_theme "
                                        "WHERE score=(SELECT MIN(score) FROM words_%1 WHERE %2) AND %2 "
                                        "ORDER BY RANDOM() LIMIT 1"
                                        ).arg(test_id).arg(cond));
    if (query.next())
    {
        reply_list = QStringList();
        for(int i = 0; i < 10; ++i){
            reply_list << query.value(i).toString();
        }
        return true;
    }else{
        last_error = query.lastError().text();
        return false;
    }
}

void DatabaseManager::find_themes(QStringList& reply_list) { 
    QSqlQuery query("SELECT * FROM themes ORDER BY name ASC");
    reply_list = QStringList();
    int nb_fields = query.boundValues().size();
    while (query.next())
        for(int i = 0; i < nb_fields; ++i)
            reply_list << query.value(i).toString();

}

void DatabaseManager::find_used_themes(int test_id, QStringList& reply_list) {
    QSqlQuery query(
                QString("SELECT DISTINCT (themes.id), name "
                        "FROM words_%1 "
                        "INNER JOIN themes "
                        "ON themes.id = words_%1.id_theme "
                        "ORDER BY name ASC").arg(test_id)
                );
    reply_list = QStringList();
    int nb_fields = query.boundValues().size();
    while (query.next())
        for(int i = 0; i < nb_fields; ++i)
            reply_list << query.value(i).toString();
}

QList<Test> DatabaseManager::get_lists()
{
    QSqlQuery query("SELECT ID, name, src, dst FROM lists");
    QList<Test> test_list;
    while (query.next())
        test_list << Test(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), false, this);
    return test_list;
}

bool DatabaseManager::open_db()
{
    // Find QSLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(DBNAME);
    db.setConnectOptions("foreign_keys = ON");

    // Open databasee
    return db.open();
}

QString DatabaseManager::pop_last_error()
{
    QString err = last_error.trimmed(); // "Because it can sometimes be "" or " ".
    last_error = " ";
    return err;
}

void DatabaseManager::search(int test_id, const QString& expr, QStringList& reply_list){
    QSqlQuery query(
                QString ("SELECT words_%1.id, word, meaning, nature, comment, example, id_theme, pronunciation, score, name "
                         "FROM words_%1 "
                         "LEFT OUTER JOIN themes "
                         "ON themes.id = words_%1.id_theme "
                         "WHERE word LIKE '%%2%' or "
                         "meaning LIKE '%%2%' or "
                         "pronunciation LIKE '%%2%'").arg(test_id).arg(expr));
    if (query.size()>0)
        reply_list = QStringList();
    while (query.next())
        for(int i = 0; i < 10; ++i)
            reply_list << query.value(i).toString();

}

bool DatabaseManager::set_score(int test_id, int id, const int& correct) {
    QSqlQuery query;
    bool success = query.prepare(
                QString("UPDATE words_%1 "
                        "SET correctly_answered = correctly_answered + :correct, "
                        "asked = asked + 1, "
                        "score = ROUND((correctly_answered + :cor) * 1.0 /(asked+2), 2) "
                        "WHERE id = :id").arg(test_id));
    query.bindValue(":id", id);
    query.bindValue(":correct", correct);
    query.bindValue(":cor", correct);
    success &= query.exec();
    if(!success)
        last_error = query.lastError().text();

    return success;
}

bool DatabaseManager::update_word(const QHash<QString, QString> &word_data)
{
    QString test_id(word_data["test_id"]);
    QSqlQuery query;

    bool success = query.prepare(QString("UPDATE words_%1 "
                                         "SET word=:word, "
                                         "meaning=:meaning, "
                                         "nature=:nature, "
                                         "comment=:comment, "
                                         "example=:example, "
                                         "id_theme=:theme, "
                                         "pronunciation=:pronunciation "
                                         "WHERE id=:id").arg(test_id));
    for(QHash<QString, QString>::const_iterator i = word_data.begin(); i != word_data.end(); ++i) {
        if(i.key() != "lang") {
            query.bindValue(":"+i.key(), i.value());
        }
    }
    success &= query.exec();
    if(!success)
        last_error = query.lastError().text();

    return success;

}
