#ifndef ADDFRAME_H
#define ADDFRAME_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtNetwork>
#include <QFormLayout>
#include <QComboBox>
#include <QTextEdit>

#include "test.h"

class AddFrame : public QWidget{
    Q_OBJECT

public:
    AddFrame(Test &test, QWidget *parent);
    ~AddFrame();

private:
    QLabel* title;
    QLabel* status;
    QComboBox* nature;
    QLineEdit* word;
    QLineEdit* meaning;
    QLineEdit* pronunciation;
    QTextEdit* comment;
    QTextEdit* example;
    QPushButton* OK_button;
    QPushButton* cancel_button;
    QFormLayout* layout;
    Test test;

public slots:
    void add_word();
    void show_confirmation(QNetworkReply* reply);
    void back();
};

#endif // ADDFRAME_H
