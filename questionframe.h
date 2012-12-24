#ifndef QUESTIONFRAME_H
#define QUESTIONFRAME_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

#include "wordframe.h"

class QuestionFrame : public WordFrame{
    Q_OBJECT

public:
    QuestionFrame(Test &test, QWidget *parent);
    ~QuestionFrame();

    QString getAnswer();
    void ask_question(const QString& text);

private:
    QLabel* label;
    QLineEdit* edit;

public slots:
    void disable_validation();
};

#endif // QUESTIONFRAME_H
