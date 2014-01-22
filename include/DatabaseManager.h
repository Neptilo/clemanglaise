#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QHash>

const QString DBNAME="clemanglaise.db.sqlite";

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);
	bool open_db();
    bool add_word(const QHash<QString, QString> &word_data);
	bool add_theme(const QString theme);
    bool create_theme_table();
    bool create_word_table(const QString &lang);
    bool find_lowest(QString &lang, QStringList &reply_list, int id_theme = -1);
	bool set_score(const QString& lang, const QString& id, const int& correct);
	QSqlQuery find_themes();
	QSqlQuery find_used_themes(const QString& lang);
	QSqlQuery search(const QString& lang, const QString& expr);
    QString get_last_error() const;
    bool update_word(const QHash<QString, QString> &word_data);

private:
    QSqlDatabase db;
    QString last_error;
	void init();

signals:

public slots:

};

#endif // DATABASEMANAGER_H
