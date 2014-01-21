#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);
    bool add_word(const QHash<QString, QString> &word_data);
    bool create_theme_table();
    bool create_word_table(QString &lang);
    bool find_lowest(QString &lang, QStringList &reply_list, int id_theme = -1);
    QString get_last_error() const;
    bool update_word(const QHash<QString, QString> &word_data);

private:
    QSqlDatabase db;
    QString last_error;

signals:

public slots:

};

#endif // DATABASEMANAGER_H
