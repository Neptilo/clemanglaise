#ifndef ADDFRAME_H
#define ADDFRAME_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtNetwork>

class AddFrame : public QWidget{
    Q_OBJECT

public:
    AddFrame(QWidget* parent);
    ~AddFrame();

private:
    QLabel* title;
    QLabel* word;
    QLabel* meaning;
    QLabel* comment;
    QLabel* example;
    QLabel* status;
    QLineEdit* word_edit;
    QLineEdit* meaning_edit;
    QLineEdit* comment_edit;
    QLineEdit* example_edit;
    QPushButton* OK_button;
    QVBoxLayout* layout;

public slots:
    void add_word();
    void show_confirmation(QNetworkReply* reply);
};

#endif // ADDFRAME_H
