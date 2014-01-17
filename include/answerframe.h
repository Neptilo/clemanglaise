#ifndef ANSWERFRAME_H
#define ANSWERFRAME_H

#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QtNetwork>

#include "wordframe.h"

class AnswerFrame : public WordFrame{
    Q_OBJECT

public:
    AnswerFrame(Test& test, QWidget *parent);
    AnswerFrame(const QStringList &reply_list, const QString &player_answer, Test& test, QWidget *parent);
};

#endif // ANSWERFRAME_H
