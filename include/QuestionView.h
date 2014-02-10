#ifndef QUESTIONFRAME_H
#define QUESTIONFRAME_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

#include "WordView.h"
#include "HandwritingArea.h"

class QuestionView : public WordView{
    Q_OBJECT

public:
    QuestionView(Test *test, QWidget *parent);
    ~QuestionView();

    QString getAnswer();
    void ask_question(const QString &word, const QString &theme);
    void show_error(const QString &error);

private:
    QLabel *label;
    QLineEdit *edit;
    HandwritingArea *handwriting_area;

public slots:
    void disable_validation();
};

#endif // QUESTIONFRAME_H
