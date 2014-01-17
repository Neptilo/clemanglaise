#ifndef QUESTIONFRAME_H
#define QUESTIONFRAME_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

#include "wordframe.h"
#include "handwritingarea.h"

class QuestionFrame : public WordFrame{
    Q_OBJECT

public:
    QuestionFrame(Test &test, QWidget *parent);
    ~QuestionFrame();

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
