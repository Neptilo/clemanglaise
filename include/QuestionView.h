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
    QuestionView(Test &test, QWidget *parent);
    ~QuestionView();

    QString getAnswer();
    void ask_question(const QString& word, const QString& theme);
	//static void set_button(QToolButton* button, const QString& text, const QString & icon_path);

private:
    QLabel* label;
    QLineEdit* edit;
    HandwritingArea* handwriting_area;

public slots:
    void disable_validation();
};

#endif // QUESTIONFRAME_H
