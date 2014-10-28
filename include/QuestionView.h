#ifndef QUESTIONVIEW_H
#define QUESTIONVIEW_H

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

    QString get_answer();
    void ask_question(const QString &word, const QString &hint);
    void show_error(const QString &error);

private:
    QLabel *label;
    QPushButton *import_button;
    QLineEdit *edit;
    HandwritingArea *handwriting_area;

public slots:
    void disable_validation();
};

#endif // QUESTIONVIEW_H
