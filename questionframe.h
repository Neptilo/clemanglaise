#ifndef QUESTIONFRAME_H
#define QUESTIONFRAME_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

class QuestionFrame : public QWidget{
    Q_OBJECT

public:
    QuestionFrame(QWidget* parent);
    ~QuestionFrame();

    QString getAnswer();
    void ask_question(const QString& text);

private:
    QLabel* label;
    QLineEdit* edit;
    QLayout* layout;
    QPushButton* OK_button;

public slots:
    void disable_validation();
};

#endif // QUESTIONFRAME_H
