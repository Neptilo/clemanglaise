#ifndef ANSWERVIEW_H
#define ANSWERVIEW_H

#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>

#include "WordView.h"
#ifndef Q_OS_WASM
#include "DatabaseManager.h"
#endif

class AnswerView: public WordView{
    Q_OBJECT

public:
    AnswerView(Test *test, QWidget *parent);
    AnswerView(const QHash<QString, QString> &word_data,
               const QString &player_answer,
               Test *test,
           #ifndef Q_OS_WASM
               DatabaseManager * database_manager,
           #endif
               QWidget *parent);
    bool get_correct();

private:
#ifndef Q_OS_WASM
    DatabaseManager *database_manager;
#endif
    bool correct;
};

#endif // ANSWERVIEW_H
