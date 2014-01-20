#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent),
    db(QSqlDatabase::addDatabase("QSQLITE")) // Find QSLite driver
{
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

    // Open database
    db.open();
}

bool DatabaseManager::create_theme_table()
{
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS themes("
                              "ID INT PRIMARY KEY, "
                              "name TEXT UNIQUE NOT NULL"
                              ")");
    if(!success)
        last_error = query.lastError().text();
    return success;
}

bool DatabaseManager::create_word_table(QString &lang)
{
    // Check if table exists, else create it
    QSqlQuery query("SELECT name FROM sqlite_master WHERE type='table' AND name='themes'");
    if(!query.next())
        if(!create_theme_table())
            return false;

    bool success = query.exec(QString("CREATE TABLE IF NOT EXISTS words_%1("
                                      "ID INT PRIMARY KEY, "
                                      "word TEXT UNIQUE NOT NULL DEFAULT '', "
                                      "meaning TEXT NOT NULL DEFAULT '', "
                                      "pronunciation TEXT DEFAULT '', "
                                      "nature TEXT NOT NULL DEFAULT '', "
                                      "comment TEXT NOT NULL DEFAULT '', "
                                      "example TEXT NOT NULL DEFAULT '', "
                                      "correctly_answered INT NOT NULL DEFAULT '0', "
                                      "asked INT NOT NULL DEFAULT '0', "
                                      "score REAL NOT NULL DEFAULT '0', "
                                      "id_theme INT NULL"
                                      ")").arg(lang));
    if(!success)
        last_error = query.lastError().text();
    return success;
}

bool DatabaseManager::find_lowest(QString& lang, QStringList& reply_list, int id_theme)
{
    // Check if table exists, else create it
    QSqlQuery query(QString("SELECT name FROM sqlite_master WHERE type='table' AND name='words_%1'").arg(lang));

    if(!query.next())
        if(!create_word_table(lang))
            return false;

    // id_theme = -1 if no theme was selected
    QString cond = (id_theme >= 0)? QString("id_theme = %1").arg(id_theme): "1";

    query = QSqlQuery(QString("SELECT words_%1.ID, word, meaning, nature, comment, example, id_theme, pronunciation, score, name "
                              "FROM words_%1 "
                              "LEFT OUTER JOIN themes "
                              "ON themes.ID = words_%1.id_theme "
                              "WHERE score=(SELECT MIN(score) FROM words_%1 WHERE %2) AND %2 "
                              "ORDER BY RANDOM() LIMIT 1"
                              ).arg(lang).arg(cond));

    if (query.next())
    {
        qDebug() << query.boundValues().values();
        //reply_list = query.boundValues().values<QList<QString>();
        // TODO: uncomment and fix line above
        return true;
    }else{
        last_error = query.lastError().text();
        return false;
    }
}

QString DatabaseManager::get_last_error() const
{
    return last_error;
}
