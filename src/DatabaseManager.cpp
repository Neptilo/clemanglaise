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
    create_word_table();
    create_tag_table();
    create_word_tag_table();
}

// test_id is an output: the inserted ID.
bool DatabaseManager::add_list(const QString &name, const QString &src, const QString &dst, int &test_id) {
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
    query.exec("SELECT LAST_INSERT_ROWID()");
    if (query.next())
        test_id = query.value(0).toInt();
    else{
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }

    query.exec("COMMIT");
    return success;
}

bool DatabaseManager::add_list(const QString &name, const QString &src, const QString &dst) {
    int test_id;
    return add_list(name, src, dst, test_id);
}

bool DatabaseManager::add_tag(const QString &tag) {
    QString str = "INSERT INTO tags "
            "(name) "
            "VALUES(:name)";
    QSqlQuery query;
    bool success = query.prepare(str);
    query.bindValue(":name", tag);

    success &= query.exec();
    if (!success)
        last_error = query.lastError().text();
    return success;
}



bool DatabaseManager::add_word(const QHash<QString, QString> &word_data, const QList<int> selected_tags)
{
    QSqlQuery query;
    query.exec("BEGIN");
    bool success = query.prepare(QString("INSERT INTO words(list_id, word, meaning, nature, pronunciation, comment, example) "
                                         "VALUES(:list_id, :word, :meaning, :nature, :pronunciation, :comment, :example)"));
    for(QHash<QString, QString>::const_iterator i = word_data.begin(); i != word_data.end(); ++i) {
        if(i.key() != "id") {
            query.bindValue(":"+i.key(), i.value());
        }
    }
    success &= query.exec();
    if (!success){
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }
    // Get last inserted ID
    int word_id = 0;
    query.exec("SELECT LAST_INSERT_ROWID()");
    if (query.next())
        word_id = query.value(0).toInt();
    else{
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }

    // Add themes to many-to-many tabe
    for (int i =0,l= selected_tags.size(); i<l; ++i)
    {
        success &= query.prepare(QString("INSERT INTO words_tags(word_id, tag_id) "
                                         "VALUES (:word_id, :tag_id)"
                                         ));
        query.bindValue(":word_id", word_id);
        query.bindValue(":tag_id", selected_tags.at(i));
        success &= query.exec();
    }

    if(!success) {
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }
    query.exec("COMMIT");
    return success;
}

bool DatabaseManager::add_word(const QHash<QString, QString> &word_data)
{
    QSqlQuery query;
    bool success = query.prepare(QString("INSERT INTO words(list_id, word, meaning, nature, pronunciation, comment, example) "
                                         "VALUES(:list_id, :word, :meaning, :nature, :pronunciation, :comment, :example)"));
    for(QHash<QString, QString>::const_iterator i = word_data.begin(); i != word_data.end(); ++i) {
        if(i.key() != "id") {
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

bool DatabaseManager::create_word_table()
{
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS words("
                              "id INTEGER PRIMARY KEY, " 
                              "list_id INTEGER NOT NULL, "
                              "word TEXT NOT NULL DEFAULT '', " 
                              "meaning TEXT NOT NULL DEFAULT '', " 
                              "pronunciation TEXT DEFAULT '', "
                              "nature VARCHAR(5) DEFAULT '', "
                              "comment TEXT DEFAULT '', "
                              "example TEXT DEFAULT '', "
                              "correctly_answered INTEGER NOT NULL DEFAULT 0, "
                              "asked INTEGER NOT NULL DEFAULT 0, "
                              "score DECIMAL(2,2) DEFAULT 0, "
                              "FOREIGN KEY(list_id) REFERENCES lists(ID) "
                              "ON UPDATE CASCADE "
                              "ON DELETE CASCADE"
                              ")");
    if(!success)
        last_error = query.lastError().text();
    return success;
}


bool DatabaseManager::create_tag_table()
{
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS tags("
                              "ID INTEGER PRIMARY KEY, "
                              "name VARCHAR(32) UNIQUE NOT NULL"
                              ")");
    if(!success)
        last_error = query.lastError().text();
    return success;
}


bool DatabaseManager::create_word_tag_table()
{
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS words_tags("
                              "word_id INTEGER NOT NULL, "
                              "tag_id INTEGER NOT NULL, "
                              "FOREIGN KEY (word_id) REFERENCES words(id) "
                              "ON UPDATE CASCADE "
                              "ON DELETE CASCADE, "
                              "FOREIGN KEY (tag_id) REFERENCES tags(id) "
                              "ON UPDATE CASCADE "
                              "ON DELETE CASCADE "
                              "UNIQUE (word_id, tag_id))");
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
    query.exec("COMMIT");
    return success;
}

bool DatabaseManager::delete_word(const int& id) {
    QSqlQuery query;
    bool success = query.prepare(QString("DELETE FROM words WHERE id = :id"));
    query.bindValue(":id", id);

    success &= query.exec();
    if (!success)
        last_error = query.lastError().text();
    return success;
}

bool DatabaseManager::find_lowest(int test_id, QHash<QString, QString> &word_data, int id_theme)
{
    // id_theme = 0 if no theme was selected
    QString cond = (id_theme > 0)? QString("id_theme = %1").arg(id_theme): "1";

    QStringList reply_keys;
    reply_keys << "word" << "meaning" << "nature" << "comment" << "example" << "id_theme" << "pronunciation" << "score" << "name";

    QSqlQuery query(QString("SELECT words_%1.ID, %3 "
                            "FROM words_%1 "
                            "LEFT OUTER JOIN themes "
                            "ON themes.ID = words_%1.id_theme "
                            "WHERE score=(SELECT MIN(score) FROM words_%1 WHERE %2) AND %2 "
                            "ORDER BY RANDOM() LIMIT 1")
                    .arg(test_id)
                    .arg(cond)
                    .arg(reply_keys.join(", ")));
    reply_keys.prepend("id"); // to account for the ID added in the SELECT query
    if (query.next())
    {
        word_data.clear();
        for(int i = 0; i < reply_keys.size(); ++i)
            word_data.insert(reply_keys.at(i), query.value(i).toString());
        return true;
    }else{
        last_error = query.lastError().text();
        return false;
    }
}

void DatabaseManager::find_tags(QStringList& reply_list) { 
    QSqlQuery query("SELECT * FROM tags ORDER BY name ASC");
    reply_list = QStringList();
    int nb_fields = query.record().count();
    while (query.next())
        for(int i = 0; i < nb_fields; ++i)
            reply_list << query.value(i).toString();

}

void DatabaseManager::find_used_tags(int test_id, QStringList& reply_list) {
    QSqlQuery query;
    bool success = query.prepare(QString("SELECT DISTINCT (tags.id), name "
                                        "FROM words_tags "
                                        "INNER JOIN tags "
                                        "ON tags.id = words_tags.tag_id "
                                        "INNER JOIN words "
                                        "ON words.id  = words_tags.word_id "
                                        "WHERE list_id = :list_id "
                                        "ORDER BY name ASC"));
    query.bindValue(":list_id", test_id);
    success &= query.exec(); 
    if (!success) {
        last_error = query.lastError().text();
        return;
    }
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
        test_list << Test(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), false);
    return test_list;
}

bool DatabaseManager::open_db()
{
    // Find QSLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

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
                QString ("SELECT words_%1.id, word, meaning, pronunciation, nature, comment, example, id_theme, score, name "
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

/*
void DatabaseManager::search(int test_id, const QString& expr, QStringList &reply_list){
    QSqlQuery query(
                QString ("SELECT id, word, meaning, pronunciation, nature, comment, example, score "
                         "FROM words "
                         "WHERE list_id = %1 "
                         "AND word LIKE '%%2%' OR "
                         "meaning LIKE '%%2%' OR "
                         "pronunciation LIKE '%%2%'").arg(test_id).arg(expr));
    reply_list = QStringList();
    int nb_fields = query.record().count();
    while (query.next())
        for(int i = 0; i < nb_fields; ++i)
            reply_list << query.value(i).toString();
    // to be consistent with the online type of result which returns one empty line
    reply_list << "" << "" << "";
}
*/

void DatabaseManager::search_by_tags(int test_id, const QString& expr, const QList<int> selected_tags, QStringList &reply_list){
    if (selected_tags.size()==0) {
        search(test_id, expr, reply_list);
        return;
    }

    // selected_tags contains at list one item

    QStringList selected_tags_str;
    for (int i = 0, l=selected_tags.size(); i < l; ++i)
        if (selected_tags.at(i) != 0)
            selected_tags_str << QString::number(selected_tags.at(i));

    QString tags_selected = selected_tags_str.join(", ");

    QString tags_cond;
    if (selected_tags.contains(0))
        tags_cond = QString("tag_id IN (%1) OR tag_id is NULL").arg(tags_selected);
    else
        tags_cond = QString("tag_id IN (%1)").arg(tags_selected);

    QSqlQuery query(
                QString ("SELECT DISTINCT (words.id), word, meaning, pronunciation, nature, comment, example, score "
                         "FROM words "
                         "LEFT OUTER JOIN words_tags "
                         "ON words_tags.word_id = words.id "
                         "WHERE list_id = %1 "
                         "AND %2 "
                         "AND  word LIKE '%%3%' OR "
                         "meaning LIKE '%%3%' OR "
                         "pronunciation LIKE '%%3%'"
                         ).arg(test_id).arg(tags_cond).arg(expr));
    reply_list = QStringList();
    int nb_fields = query.record().count();
    while (query.next())
        for(int i = 0; i < nb_fields; ++i)
            reply_list << query.value(i).toString();
    // to be consistent with the online type of result which returns one empty line
    reply_list << "" << "" << "";
}

void DatabaseManager::get_tags(int word_id, QStringList &word_tags) {
    QSqlQuery query;
    query.prepare(QString("SELECT name from tags "
                          "INNER JOIN words_tags "
                          "ON tags.id = words_tags.tag_id "
                          "WHERE word_id = :id "
                          "ORDER BY name ASC"));
    query.bindValue(":id", word_id);
    query.exec();
    while(query.next())
        word_tags << query.value(0).toString();
}

bool DatabaseManager::set_score(int id, const int &correct) {
    QSqlQuery query;
    bool success = query.prepare(
                QString("UPDATE words "
                        "SET correctly_answered = correctly_answered + :correct, "
                        "asked = asked + 1, "
                        "score = ROUND((correctly_answered + :cor) * 1.0 /(asked+2), 2) "
                        "WHERE id = :id"));
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
    QSqlQuery query;

    bool success = query.prepare(QString("UPDATE words SET "
                                         "word=:word, "
                                         "meaning=:meaning, "
                                         "nature=:nature, "
                                         "comment=:comment, "
                                         "example=:example, "
                                         "pronunciation=:pronunciation, "
                                         "WHERE id=:id "
                                         "AND list_id=:list_id"));
    QHash<QString, QString>::const_iterator i;
    for(i = word_data.begin(); i != word_data.end(); ++i) {
        if(i.key() != "score") {
            query.bindValue(":"+i.key(), i.value());
        }
    }

    success &= query.exec();
    if(!success)
        last_error = query.lastError().text();

    return success;
}

bool DatabaseManager::update_word(const QHash<QString, QString> &word_data, const QList<int> selected_tags)
{
    QSqlQuery query;

    bool success = query.prepare(QString("UPDATE words SET "
                                         "word=:word, "
                                         "meaning=:meaning, "
                                         "nature=:nature, "
                                         "comment=:comment, "
                                         "example=:example, "
                                         "pronunciation=:pronunciation, "
                                         "WHERE id=:id "
                                         "AND list_id=:list_id"));
    QHash<QString, QString>::const_iterator i;
    for(i = word_data.begin(); i != word_data.end(); ++i) {
        if(i.key() != "score") {
            query.bindValue(":"+i.key(), i.value());
        }
    }

    success &= query.exec();
    if(!success)
        last_error = query.lastError().text();

    // Handle tags
    success = query.prepare(QString("SELECT tag_id FROM words_tags "
    "WHERE word_id =  :id"));
    query.bindValue(":id", word_data["id"]);
    success &= query.exec();
    if(!success)
        last_error = query.lastError().text();

    QList<int> existing_tags;
    while(query.next()) {
       existing_tags << query.value(0).toInt();
    }

    QStringList tags_to_delete, tags_to_add;

    for (int i = 0, l = existing_tags.size(); i < l; ++ i) {
        int item = existing_tags[i];
        if(!selected_tags.contains(item))
            tags_to_delete << QString::number(item);
    }

    for (int i = 0, l = selected_tags.size(); i < l; ++ i)
    {
        int item = selected_tags[i];
        if(!existing_tags.contains(item))
            tags_to_add << QString::number(item);
    }

    for (int i = 0, l = tags_to_add.size(); i < l; ++i)
    {
        success &= query.prepare(QString("INSERT INTO words_tags(word_id, tag_id) "
                                         "VALUES (:word_id, :tag_id)"
                                         ));
        query.bindValue(":word_id", word_data["id"]);
        query.bindValue(":tag_id", tags_to_add.at(i));
        success &= query.exec();
    }

    //for (int i = 0, l = tags_to_delete.size(); i < l; ++i)
    success &= query.prepare(QString("DELETE FROM words_tags "
                                     "WHERE word_id = :word_id "
                                     "AND tag_id IN (%1)"
                                     ).arg(tags_to_delete.join(", ")));
    query.bindValue(":word_id", word_data["id"]);
    success &= query.exec();

    if (!success)
        last_error = query.lastError().text();

    return success;
}

// Two sets of words are considered as possible duplicates of one another if
// more than half the words of the smaller set are in common with the larger set.
bool DatabaseManager::find_duplicates(int test_id, const QString &word, QStringList &reply_keys, QList<QStringList> &reply_values)
{
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
        for(int i = 0; i < reply_keys.size(); ++i)
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

bool DatabaseManager::count(int test_id, int &res)
{
    QSqlQuery query;
    bool success = query.prepare(QString("SELECT COUNT(*) FROM words "
                            "WHERE list_id=:list_id"));
    query.bindValue(":list_id", test_id);
    
    if ((success &= query.next()))
        res = query.value(0).toInt();
    else
        last_error = query.lastError().text();
    return success;
}
