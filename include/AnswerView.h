#ifndef ANSWERFRAME_H
#define ANSWERFRAME_H

#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QtNetwork>

#include "WordView.h"

class AnswerView: public WordView{
    Q_OBJECT

public:
    AnswerView(Test& test, QWidget *parent);
    AnswerView(const QStringList &reply_list, const QString &player_answer, Test& test, QWidget *parent);
};

#endif // ANSWERFRAME_H
