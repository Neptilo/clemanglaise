#ifndef ANSWERVIEW_H
#define ANSWERVIEW_H

#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QtNetwork>

#include "WordView.h"
#include "DatabaseManager.h"

class AnswerView: public WordView{
    Q_OBJECT

public:
    AnswerView(Test *test, QWidget *parent);
    AnswerView(const QHash<QString, QString> &word_data, const QString &player_answer, Test *test, DatabaseManager * database_manager, QWidget *parent);
    bool get_correct();

private:
    DatabaseManager *database_manager;
    bool correct;
};

#endif // ANSWERVIEW_H
