#ifndef QUESTIONVIEW_H
#define QUESTIONVIEW_H

#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

#include "WordView.h"
#include "HandwritingArea.h"

class QuestionView : public WordView{
    Q_OBJECT

public:
    QuestionView(Test *test, bool admin, QWidget *parent);
    ~QuestionView();

    QString get_answer();
    void ask_question(const QString &word, const QString &hint);
    void show_error(const QString &error);

private:
    bool admin;

    HandwritingArea *handwriting_area;

    // GUI
    QBoxLayout *top_layout;
    QLabel *label;
    QToolButton *edit_button;
    QToolButton *import_button;
    QToolButton *delete_button;
    QBoxLayout *input_layout;
    QLineEdit *edit;

    // actions
    QAction *edit_action;
    QAction *import_action;
    QAction *delete_action;

    void create_actions();

public slots:
    void disable_validation();

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // QUESTIONVIEW_H
