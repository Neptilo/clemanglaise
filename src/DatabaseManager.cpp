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
    bool success = query.prepare("INSERT INTO lists "
                                 "(name, src, dst) "
                                 "VALUES(:name, :src, :dst)");
    query.bindValue(":name", name);
    query.bindValue(":src", src);
    query.bindValue(":dst", dst);
    success &= query.exec();
    if (!success){
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }

    // Get last inserted ID
    int test_id;
    query.exec("SELECT LAST_INSERT_ROWID()");
    if (query.next())
        test_id = query.value(0).toInt();
    else{
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }

    /* We call the list table "words_<id>" with <id> its ID in the list table,
     * instead of calling it the name given by the user. */
    success = query.exec(QString("CREATE TABLE IF NOT EXISTS words_%1("
                                 "id INTEGER PRIMARY KEY, "
                                 "word TEXT NOT NULL DEFAULT '', "
                                 "meaning TEXT NOT NULL DEFAULT '', "
                                 "pronunciation TEXT DEFAULT '', "
                                 "nature VARCHAR(5) DEFAULT '', "
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
        query.exec("ROLLBACK");
        return false;
    }
    query.exec("COMMIT");
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
        if(i.key() != "test_id" && i.key() != "id") {
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
    query.exec("BEGIN");
    bool success = query.exec(QString("DELETE FROM lists "
                                      "WHERE ID=%1").arg(test_id));
    if (!success){
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }
    if (query.numRowsAffected() != 1){
        last_error = tr("%1 list(s) seem to be affected by the deletion.").arg(query.numRowsAffected());
        query.exec("ROLLBACK");
        return false;
    }

    success = query.exec(QString("DROP TABLE words_%1").arg(test_id));
    if (!success){
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }

    query.exec("COMMIT");
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
    int nb_fields = query.record().count();
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
    int nb_fields = query.record().count();
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

#ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(".clemanglaise");
    // Create Clemanglaise folder if it doesn't exist
    if(!QDir(path).exists())
        QDir(path).mkpath(".");
    path.append(QDir::separator()).append("clemanglaise.sqlite");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
#else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    db.setDatabaseName("clemanglaise.sqlite");
#endif
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

void DatabaseManager::search(int test_id, const QString& expr, QStringList &reply_list){
    QSqlQuery query(
                QString ("SELECT words_%1.id, word, meaning, nature, comment, example, id_theme, pronunciation, score, name "
                         "FROM words_%1 "
                         "LEFT OUTER JOIN themes "
                         "ON themes.id = words_%1.id_theme "
                         "WHERE word LIKE '%%2%' or "
                         "meaning LIKE '%%2%' or "
                         "pronunciation LIKE '%%2%' or "
                         "name LIKE '%%2%'").arg(test_id).arg(expr));
    reply_list = QStringList();
    int nb_fields = query.record().count();
    while (query.next())
        for(int i = 0; i < nb_fields; ++i)
            reply_list << query.value(i).toString();
    // to be consistent with the online type of result which returns one empty line
    reply_list << "";
}

bool DatabaseManager::set_score(int test_id, int id, const int &correct) {
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
        if(i.key() != "test_id") {
            query.bindValue(":"+i.key(), i.value());
        }
    }
    success &= query.exec();
    if(!success)
        last_error = query.lastError().text();

    return success;

}

// Two sets of words are considered as possible duplicates of one another if
// more than half the words of the smaller set are in common with the larger set.
bool DatabaseManager::find_duplicates(int test_id, const QString &word, QStringList &reply_keys, QList<QStringList> &reply_values){
    QStringList word_list = word.split(",");
    for (int i = 0; i < word_list.size(); ++i)
        word_list.replace(i, word_list.at(i).trimmed());
    reply_keys.clear();
    reply_keys << "id" << "word" << "meaning" << "nature" << "comment" << "example" << "id_theme" << "pronunciation" << "score";
    QStringList cond;
    for (int i = 0; i < word_list.size(); ++i)
        cond << QString("word LIKE '%%1%'").arg(word_list[i]);
    cond.join(" OR ");
    QSqlQuery query(QString("SELECT %3 FROM words_%1 WHERE %2")
                    .arg(test_id)
                    .arg(cond.join(" OR "))
                    .arg(reply_keys.join(", ")));
    reply_values.clear();
    while (query.next()){
        QStringList entry;
        for(int i = 0; i < 9; ++i)
            entry << query.value(i).toString();

        // get the word list in this entry
        // at the same time count words in common with input word list
        QStringList reply_word_entry = entry.at(1).split(",");
        int nb_words_in_common = 0;
        for(int i = 0; i < reply_word_entry.size(); ++i){
            if(word_list.contains(reply_word_entry.at(i).trimmed(), Qt::CaseInsensitive))
                ++nb_words_in_common;
        }

        // insert entry as possible duplicate only if duplicate condition is respected
        if(2*nb_words_in_common > qMin(word_list.size(), reply_word_entry.size()))
            reply_values << entry;
    }

    return true;
}
