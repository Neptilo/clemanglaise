#include "DatabaseManager.h"

#if defined(Q_OS_ANDROID)
#include <QAndroidJniObject>
#endif
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
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

bool DatabaseManager::open_db()
{
    // Find QSLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

#if defined(Q_OS_ANDROID)
    // in Android, store database file in external storage
    QAndroidJniObject media_dir = QAndroidJniObject::callStaticObjectMethod(
                "android/os/Environment",
                "getExternalStorageDirectory",
                "()Ljava/io/File;");
    QAndroidJniObject media_path = media_dir.callObjectMethod("getAbsolutePath", "()Ljava/lang/String;");
    QString path(media_path.toString());
    path.append(QDir::separator()).append("Clemanglaise");
    // Create Clemanglaise folder if it doesn't exist
    if(!QDir(path).exists())
        QDir(path).mkpath(".");
    path.append(QDir::separator()).append("clemanglaise.sqlite");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
#elif defined(Q_OS_LINUX)
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

    // Open database
    bool success = db.open();
    if(!success) {
        last_error = db.lastError().text();
        return false;
    }
    QSqlQuery query;
    success &= query.exec("PRAGMA foreign_keys = ON");
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
                              "dst VARCHAR(3) NOT NULL, "
                              "flag VARCHAR(2) NOT NULL"
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
                              "hint TEXT DEFAULT '', "
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

// test_id is an output: the inserted ID.
bool DatabaseManager::add_list(const QString &name, const QString &src, const QString &dst, const QString &flag, int &test_id) {
    QSqlQuery query;
    query.exec("BEGIN");
    bool success = query.prepare("INSERT INTO lists "
                                 "(name, src, dst, flag) "
                                 "VALUES(:name, :src, :dst, :flag)");
    query.bindValue(":name", name);
    query.bindValue(":src", src);
    query.bindValue(":dst", dst);
    query.bindValue(":flag", flag);
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

bool DatabaseManager::add_list(const QString &name, const QString &src, const QString &dst, const QString &flag) {
    int test_id;
    return add_list(name, src, dst, flag, test_id);
}

bool DatabaseManager::add_tag(const QString &tag) {
    int tag_id;
    return add_tag(tag, tag_id);
}

bool DatabaseManager::add_tag(const QString &tag, int &tag_id) {
    QString str = "INSERT INTO tags "
            "(name) "
            "VALUES(:name)";
    QSqlQuery query;
    bool success = query.prepare(str);
    query.bindValue(":name", tag);
    success &= query.exec();
    if (!success) {
        last_error = query.lastError().text();
        return false;
    }
    success &= query.exec("SELECT LAST_INSERT_ROWID()");
    if (query.next())
        tag_id = query.value(0).toInt();
    else{
        last_error = query.lastError().text();
        return false;
    }
    return success;
}

bool DatabaseManager::add_word(const QHash<QString, QString> &word_data, const QList<int> selected_tags)
{
    QSqlQuery query;
    query.exec("BEGIN");
    bool success = query.prepare(QString("INSERT INTO words(list_id, word, meaning, nature, pronunciation, comment, example, hint) "
                                         "VALUES(:list_id, :word, :meaning, :nature, :pronunciation, :comment, :example, :hint)"));
    for(QHash<QString, QString>::const_iterator i = word_data.begin(); i != word_data.end(); ++i) {
        if(i.key() != "id") {
            query.bindValue(":"+i.key(), i.value().trimmed());
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

    // Add tags to many-to-many table
    for (int i =0,l= selected_tags.size(); i<l && success; ++i)
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
    QString tag_ids_str = word_data["tag_ids"];
    if (!tag_ids_str.isEmpty()) {
        QHash<QString, QString> new_word_data(word_data);
        new_word_data.remove("tag_ids");
        QStringList tag_id_list = tag_ids_str.split(", ");
        QList<int> tag_ids;
        for (int i = 0; i < tag_id_list.size(); ++i)
            tag_ids << tag_id_list.at(i).toInt();
        return add_word(new_word_data, tag_ids);
    }

    QSqlQuery query;
    bool success = query.prepare(QString("INSERT INTO words(list_id, word, meaning, nature, pronunciation, comment, example, hint) "
                                         "VALUES(:list_id, :word, :meaning, :nature, :pronunciation, :comment, :example, :hint)"));
    for(QHash<QString, QString>::const_iterator i = word_data.begin(); i != word_data.end(); ++i) {
        if(i.key() != "id") {
            query.bindValue(":"+i.key(), i.value().trimmed());
        }
    }
    success &= query.exec();

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

bool DatabaseManager::find_lowest(int test_id, QHash<QString, QString> &word_data, QList<int> tag_ids)
{
    QString tags_cond;
    QStringList selected_tags_str;
    for (int i = 0, l=tag_ids.size(); i < l; ++i)
        if (tag_ids.at(i) != 0)
            selected_tags_str << QString::number(tag_ids.at(i));

    QString tags_selected = selected_tags_str.join(", ");
    if (tag_ids.size()==0) // No tags filter set
        tags_cond = "1";
    else if (tag_ids.contains(0)) // account for words with no tags
        tags_cond = QString("(tag_id IN (%1) OR tag_id is NULL)").arg(tags_selected);
    else
        tags_cond = QString("tag_id IN (%1)").arg(tags_selected);

    QStringList reply_keys;
    reply_keys << "id" << "word" << "meaning" << "nature" << "comment" << "example"  << "pronunciation" << "hint" << "tag_ids";

    QSqlQuery query(QString("SELECT words.id, word, meaning, nature, comment, example, pronunciation, hint "
                            "FROM words "
                            "LEFT OUTER JOIN words_tags "
                            "ON words.id = words_tags.word_id "
                            "WHERE list_id = %1 AND %2 "
                            "GROUP BY words.id "
                            "ORDER BY score, RANDOM() "
                            "LIMIT 1"
                            ).arg(test_id).arg(tags_cond));
    int id_index = query.record().indexOf("id");
    if (query.next()) {
        word_data.clear();
        for(int i = 0; i < reply_keys.size(); ++i)
            word_data.insert(reply_keys.at(i), query.value(i).toString());
        QStringList tag_id_list;
        get_tags_id(query.value(id_index).toInt(), tag_id_list);
        word_data.insert("tag_ids", tag_id_list.join(", "));
        return true;
    } else {
        last_error = query.lastError().text();
        return false;
    }
}

bool DatabaseManager::find_tags(QStringList& reply_list) {
    QSqlQuery query("SELECT * FROM tags ORDER BY name ASC");
    reply_list = QStringList();
    int nb_fields = query.record().count();
    while (query.next())
        for(int i = 0; i < nb_fields; ++i)
            reply_list << query.value(i).toString();
    return true;
}

bool DatabaseManager::find_used_tags(int test_id, QStringList& reply_list) {
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
        return false;
    }
    reply_list = QStringList();
    int nb_fields = query.record().count();
    while (query.next())
        for(int i = 0; i < nb_fields; ++i)
            reply_list << query.value(i).toString();
    return true;
}

QList<Test> DatabaseManager::get_lists()
{
    QSqlQuery query("SELECT ID, name, src, dst, flag FROM lists");
    QList<Test> test_list;
    while (query.next())
        test_list << Test(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), query.value(4).toString(), false);
    return test_list;
}

QString DatabaseManager::pop_last_error()
{
    QString err = last_error.trimmed(); // "Because it can sometimes be "" or " ".
    last_error = " ";
    return err;
}

bool DatabaseManager::search(int test_id, const QString& expr, const QList<int> selected_tags, QStringList &reply_list){

    QStringList selected_tags_str;
    for (int i = 0, l=selected_tags.size(); i < l; ++i)
        if (selected_tags.at(i) != 0)
            selected_tags_str << QString::number(selected_tags.at(i));

    QString tags_selected = selected_tags_str.join(", ");

    QString tags_cond;
    if (selected_tags.size()==0)
        tags_cond = QString("1");
    else if (selected_tags.contains(0))
        tags_cond = QString("(tag_id IN (%1) OR tag_id is NULL)").arg(tags_selected);
    else
        tags_cond = QString("tag_id IN (%1)").arg(tags_selected);

    QSqlQuery query(
                QString ("SELECT DISTINCT (words.id), word, meaning, pronunciation, nature, comment, example, hint, score "
                         "FROM words "
                         "LEFT OUTER JOIN words_tags "
                         "ON words_tags.word_id = words.id "
                         "WHERE list_id = %1 "
                         "AND %2 "
                         "AND (word LIKE '%%3%' OR "
                         "meaning LIKE '%%3%' OR "
                         "pronunciation LIKE '%%3%')"
                         ).arg(test_id).arg(tags_cond).arg(expr));
    reply_list = QStringList();
    int nb_fields = query.record().count();
    QStringList tag_list_id;
    while (query.next()){
        for(int i = 0; i < nb_fields; ++i)
            reply_list << query.value(i).toString();
        tag_list_id.clear();
        get_tags_id(query.value(0).toInt(), tag_list_id);
        reply_list << tag_list_id.join(", ");
    }
    // to be consistent with the online type of result which returns one empty line
    reply_list << "";
    return true;
}

bool DatabaseManager::get_tags(int word_id, QStringList &word_tags) {
    QSqlQuery query;
    query.prepare(QString("SELECT name from tags "
                          "INNER JOIN words_tags "
                          "ON tags.id = words_tags.tag_id "
                          "WHERE word_id = :id "
                          "ORDER BY name ASC"));
    query.bindValue(":id", word_id);
    if (!query.exec()) {
        last_error = query.lastError().text();
        return false;
    }
    while(query.next())
        word_tags << query.value(0).toString();
    return true;
}

bool DatabaseManager::get_tags_id(int word_id, QStringList &word_tags_id) {
    QSqlQuery query;
    query.prepare(QString("SELECT tags.id from tags "
                          "INNER JOIN words_tags "
                          "ON tags.id = words_tags.tag_id "
                          "WHERE word_id = :id "
                          "ORDER BY name ASC"));
    query.bindValue(":id", word_id);
    if (!query.exec()) {
        last_error = query.lastError().text();
        return false;
    }
    while(query.next())
        word_tags_id << query.value(0).toString();
    return true;
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
    if(query.numRowsAffected() != 1){
        last_error = QString("%1 rows were affected by the update query.").arg(query.numRowsAffected());
        return false;
    }

    return success;
}

bool DatabaseManager::update_word(const QHash<QString, QString> &word_data)
{
    QString tag_ids_str = word_data["tag_ids"];
    if (!tag_ids_str.isEmpty()) {
        QHash<QString, QString> new_word_data(word_data);
        new_word_data.remove("tag_ids");
        QStringList tag_id_list = tag_ids_str.split(", ");
        QList<int> tag_ids;
        for (int i = 0; i < tag_id_list.size(); ++i)
            tag_ids << tag_id_list.at(i).toInt();
        return update_word(new_word_data, tag_ids);
    }

    QSqlQuery query;

    bool success = query.prepare(QString("UPDATE words SET "
                                         "word=:word, "
                                         "meaning=:meaning, "
                                         "nature=:nature, "
                                         "comment=:comment, "
                                         "example=:example, "
                                         "pronunciation=:pronunciation, "
                                         "hint=:hint "
                                         "WHERE id=:id"));
    QHash<QString, QString>::const_iterator i;
    for(i = word_data.begin(); i != word_data.end(); ++i) {
        if(i.key() != "score") {
            query.bindValue(":"+i.key(), i.value());
        }
    }

    success &= query.exec();
    if(!success)
        last_error = query.lastError().text();
    if(query.numRowsAffected() != 1){
        last_error = QString("%1 rows were affected by the update query.").arg(query.numRowsAffected());
        return false;
    }
    return success;
}

bool DatabaseManager::update_word(const QHash<QString, QString> &word_data, const QList<int> selected_tags)
{
    QSqlQuery query;
    query.exec("BEGIN");

    bool success = query.prepare(QString("UPDATE words SET "
                                         "word=:word, "
                                         "meaning=:meaning, "
                                         "nature=:nature, "
                                         "comment=:comment, "
                                         "example=:example, "
                                         "pronunciation=:pronunciation, "
                                         "hint=:hint "
                                         "WHERE id=:id"));

    QHash<QString, QString>::const_iterator i;
    for(i = word_data.begin(); i != word_data.end(); ++i) {
        if(i.key() != "score") {
            query.bindValue(":"+i.key(), i.value().trimmed());
        }
    }
    success &= query.exec();
    if(!success){
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }
    if(query.numRowsAffected() != 1){
        last_error = QString("%1 rows were affected by the update query.").arg(query.numRowsAffected());
        query.exec("ROLLBACK");
        return false;
    }

    // Handle tags
    success = query.prepare(QString("SELECT tag_id FROM words_tags "
    "WHERE word_id =  :id"));
    query.bindValue(":id", word_data["id"]);
    success &= query.exec();
    if(!success){
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }

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

    for (int i = 0, l = selected_tags.size(); i < l; ++ i) {
        int item = selected_tags[i];
        if(!existing_tags.contains(item))
            tags_to_add << QString::number(item);
    }
    for (int i = 0, l = tags_to_add.size(); i < l && success; ++i) {
        success &= query.prepare(QString("INSERT INTO words_tags(word_id, tag_id) "
                                         "VALUES (:word_id, :tag_id)"
                                         ));
        query.bindValue(":word_id", word_data["id"]);
        query.bindValue(":tag_id", tags_to_add.at(i));
        success &= query.exec();
    }
    if (!success){
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }

    success &= query.prepare(QString("DELETE FROM words_tags "
                                     "WHERE word_id = :word_id "
                                     "AND tag_id IN (%1)"
                                     ).arg(tags_to_delete.join(", ")));
    query.bindValue(":word_id", word_data["id"]);
    success &= query.exec();

    if (!success){
        last_error = query.lastError().text();
        query.exec("ROLLBACK");
        return false;
    }
    query.exec("COMMIT");

    return true;
}

// Two sets of words are considered as possible duplicates of one another if
// more than half the words of the smaller set are in common with the larger set.
bool DatabaseManager::find_duplicates(int test_id, const QString &word, QStringList &reply_keys, QList<QStringList> &reply_values)
{
    QStringList word_list = word.split(",");
    for (int i = 0; i < word_list.size(); ++i)
        word_list.replace(i, word_list.at(i).trimmed());
    reply_keys.clear();
    reply_keys << "id" << "word" << "meaning" << "nature" << "comment" << "example" << "pronunciation" << "hint" << "score";
    QStringList cond;
    for (int i = 0; i < word_list.size(); ++i)
        cond << QString("word LIKE '%%1%'").arg(word_list[i]);
    QSqlQuery query(QString("SELECT %3 FROM words WHERE list_id = %1 AND (%2)")
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
        if(2*nb_words_in_common > qMin(word_list.size(), reply_word_entry.size())){
            // get this entry's tags
            QStringList tag_list_id;
            get_tags_id(query.value(0).toInt(), tag_list_id);
            entry << tag_list_id.join(", ");

            reply_values << entry;
        }
    }
    reply_keys << "tag_ids";
    return true;
}

bool DatabaseManager::count(int test_id, int &res)
{
    QSqlQuery query;
    bool success = query.prepare(QString("SELECT COUNT(*) FROM words "
                            "WHERE list_id=:list_id"));
    query.bindValue(":list_id", test_id);
    if (!success) {
        last_error = query.lastError().text();
        return success;
    }
    success &= query.exec();
    if (!success) {
        last_error = query.lastError().text();
        return success;
    }
    success &= query.next();
    if (success)
        res = query.value(0).toInt();
    else
        last_error = query.lastError().text();
    return success;
}
