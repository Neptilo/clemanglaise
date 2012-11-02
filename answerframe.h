#ifndef ANSWERFRAME_H
#define ANSWERFRAME_H

#include <QLabel>
#include <QVBoxLayout>
#include <QtNetwork>
#include <QPushButton>

class AnswerFrame : public QWidget{
public:
    AnswerFrame(QWidget *parent, const QStringList& reply_list, const QString& player_answer);
    ~AnswerFrame();
    void setLabel(QString &text);

private:
    QLabel* label;
    QVBoxLayout* layout;
    QPushButton* OK_button;
};

#endif // ANSWERFRAME_H
