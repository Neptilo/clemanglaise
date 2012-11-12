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

class AddFrame : public QWidget{
    Q_OBJECT

public:
    AddFrame(QWidget* parent);
    ~AddFrame();

private:
    QLabel* title;
    QLabel* status;
    QComboBox* nature;
    QLineEdit* word;
    QLineEdit* meaning;
    QTextEdit* comment;
    QTextEdit* example;
    QPushButton* OK_button;
    QFormLayout* layout;

public slots:
    void add_word();
    void show_confirmation(QNetworkReply* reply);
    void exit();
};

#endif // ADDFRAME_H
