#ifndef EDITFRAME_H
#define EDITFRAME_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtNetwork>
#include <QFormLayout>
#include <QComboBox>
#include <QTextEdit>

#include "test.h"

class EditFrame : public QWidget{
    Q_OBJECT

public:
    EditFrame(Test &test, const QString &title, const QStringList &default_values, const QString &OK_button_value, const QString &php_filename, const QString &success_message, QWidget *parent);
    ~EditFrame();

private:
    QLabel* title;
    QLabel* status;
    QComboBox* nature_edit;
    QLineEdit* word_edit;
    QLineEdit* meaning_edit;
    QLineEdit* pronunciation_edit;
    QTextEdit* comment_edit;
    QTextEdit* example_edit;
    QPushButton* OK_button;
    QPushButton* cancel_button;
    QFormLayout* layout;
    Test test;
    QString php_filename;
    QStringList default_values;
    QString success_message;

public slots:
    void edit_word();
    void show_confirmation(QNetworkReply* reply);
    void back();
};

#endif // EDITFRAME_H
