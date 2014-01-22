#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMapIterator>
#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
	init(); 

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

bool DatabaseManager::add_word(const QHash<QString, QString> &word_data)
{

	QSqlQuery query;
	QString lang = word_data["lang"];
    bool success = query.prepare(QString("INSERT INTO words_%1(word, meaning, nature, pronunciation, comment, example, id_theme) "
                                 "VALUES(:word, :meaning, :nature, :pronunciation, :comment, :example, :theme)").arg(lang));
	
    for(QHash<QString, QString>::const_iterator i = word_data.begin(); i != word_data.end(); ++i) {
		if(i.key() != "lang" && i.key() != "id") {
			query.bindValue(":"+i.key(), i.value());
		}
	}

	if(!success)
		last_error = query.lastError().text();
	return query.exec();
}

bool DatabaseManager::delete_word(const QString& lang, const int& id) {
	QSqlQuery query;
	bool success = query.prepare(QString("DELETE FROM words_%1 WHERE id = :id").arg(lang));
	query.bindValue(":id", id);

	if (!success)
		last_error = query.lastError().text();
	return query.exec(); 
}

bool DatabaseManager::add_theme(const QString theme) {
	QString str = "insert into themes "  
		"(name) "
		"values (:name)";	
	QSqlQuery query;
	query.prepare(str);
	query.bindValue(":name", theme);
	bool success = query.exec();
	if (!success)
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

bool DatabaseManager::create_word_table(const QString &lang)
{

	QSqlQuery query;
	bool success = query.exec(QString("CREATE TABLE IF NOT EXISTS words_%1("
				"id INTEGER PRIMARY KEY, "
				"word VARCHAR(50) NOT NULL DEFAULT '', "
				"meaning VARCHAR(100) NOT NULL DEFAULT '', "
				"pronunciation VARCHAR(100) DEFAULT '', "
				"nature VARCHAR(5) NOT NULL DEFAULT '', "
				"comment TEXT DEFAULT '', "
				"example TEXT DEFAULT '', "
				"correctly_answered INT NOT NULL DEFAULT 0, "
				"asked INT NOT NULL DEFAULT 0, "
				"score DECIMAL(2,2) DEFAULT 0, "
				"id_theme INTEGER NULL, "
				"FOREIGN KEY(id_theme) REFERENCES themes(id) ON DELETE SET NULL)").arg(lang));
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

	// id_theme = 0 if no theme was selected
	QString cond = (id_theme > 0)? QString("id_theme = %1").arg(id_theme): "1";

	query = QSqlQuery(QString("SELECT words_%1.ID, word, meaning, nature, comment, example, id_theme, pronunciation, score, name "
				"FROM words_%1 "
				"LEFT OUTER JOIN themes "
				"ON themes.ID = words_%1.id_theme "
				"WHERE score=(SELECT MIN(score) FROM words_%1 WHERE %2) AND %2 "
				"ORDER BY RANDOM() LIMIT 1"
				).arg(lang).arg(cond));
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

bool DatabaseManager::set_score(const QString& lang, const QString& id, const int& correct) {
	QSqlQuery query;
	bool success = query.prepare(
			QString("UPDATE words_%1 " 
				"SET correctly_answered = correctly_answered + :correct, "
				"asked = asked + 1, "
				"score = ROUND((correctly_answered + :cor) * 1.0 /(asked+2), 2) "
				"WHERE id = :id").arg(lang));
	query.bindValue(":id", id.toInt());
	query.bindValue(":correct", correct);
	query.bindValue(":cor", correct);
	if(!success)
		last_error = query.lastError().text(); 
	query.exec(); 

	return success;
}

QString DatabaseManager::get_last_error() const
{
	return last_error;
}

QSqlQuery DatabaseManager::find_themes() { 
	QSqlQuery query("SELECT * FROM themes ORDER BY name ASC");
	return query; 
}

QSqlQuery DatabaseManager::find_used_themes(const QString& lang) {
	QSqlQuery query(
			QString("SELECT DISTINCT (themes.id), name "
				"FROM words_%1 " 
				"INNER JOIN themes "
				"ON themes.id = words_%1.id_theme "
				"ORDER BY name ASC").arg(lang)
			);
	return query;
}

void DatabaseManager::search(const QString& lang, const QString& expr, QStringList& reply_list){
	QSqlQuery query(
			QString ("select words_%1.id, word, meaning, nature, comment, example, id_theme, name, pronunciation, score " 
				"from words_%1 " 
				"left outer join themes "
				"on themes.id = words_%1.id_theme " 
				"where word like '%%2%' or "
				"meaning like '%%2%' or "
				"pronunciation like '%%2%'").arg(lang).arg(expr));
	while (query.next()) {
		reply_list = QStringList();
		for(int i = 0; i < 10; ++i)
			reply_list << query.value(i).toString();
		
	}
}


bool DatabaseManager::update_word(const QHash<QString, QString> &word_data)
{
	QString lang = word_data["lang"];
	QSqlQuery query;

	bool success = query.prepare(QString("UPDATE words_%1 "  
				"SET word=:word, "
				"meaning=:meaning, "
				"nature=:nature, "
				"comment=:comment, "
				"example=:example, "
				"id_theme=:theme, "
				"pronunciation=:pronunciation "
				"WHERE id=:id").arg(lang));	
    for(QHash<QString, QString>::const_iterator i = word_data.begin(); i != word_data.end(); ++i) {
		if(i.key() != "lang") {
			query.bindValue(":"+i.key(), i.value());
		}
	}
	if(!success)
		last_error = query.lastError().text();
	query.exec();

	return success;

}

void DatabaseManager::init()
{
	if(!open_db()) {
		qDebug() << "Error while opening the database";
		return;
	} 

	create_theme_table();
	create_word_table("enfr");
	create_word_table("enja");
	create_word_table("enzh");
	create_word_table("enhr");

}
