#ifndef ANSWERFRAME_H
#define ANSWERFRAME_H

#include <QLabel>
#include <QtNetwork>
#include <QPushButton>

#include "wordframe.h"

class AnswerFrame : public WordFrame{
public:
    AnswerFrame(Test& test, QWidget *parent);
    AnswerFrame(const QStringList &reply_list, const QString &player_answer, Test& test, QWidget *parent);
    void setLabel(QString &text);

private:
    QLabel* label;
    QPushButton* OK_button;
};

#endif // ANSWERFRAME_H
