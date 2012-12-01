#ifndef ANSWERFRAME_H
#define ANSWERFRAME_H

#include <QLabel>
#include <QLayout>
#include <QtNetwork>
#include <QPushButton>

class AnswerFrame : public QWidget{
public:
    AnswerFrame(QWidget *parent);
    AnswerFrame(QWidget *parent, const QStringList& reply_list, const QString& player_answer);
    void setLabel(QString &text);

private:
    QLabel* label;
    QLayout* layout;
    QPushButton* OK_button;
};

#endif // ANSWERFRAME_H
