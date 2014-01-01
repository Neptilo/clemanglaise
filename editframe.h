#ifndef EDITFRAME_H
#define EDITFRAME_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
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
    void read_reply(QString reply_string);
	void disable_edition(bool ok);

private:
    QLabel* title;
    QLabel* status;
    QComboBox* nature_edit;
    QComboBox* themes;
    QNetworkAccessManager nam;
    QNetworkAccessManager theme_nam;
    QLineEdit* word_edit;
    QLineEdit* meaning_edit;
    QLineEdit* pronunciation_edit;
    QTextEdit* comment_edit;
    QTextEdit* example_edit;
    QPushButton* OK_button;
    QPushButton* cancel_button;
    QPushButton* continue_button;
    QFormLayout* layout;
    Test test;
    QString php_filename;
    QStringList default_values;
    QString success_message;

public slots:
    void edit_word();
	void read_reply(QNetworkReply * reply); 
    void show_confirmation(QNetworkReply* reply);
    void show_confirmation();
    void back();
    void reset();
	void find_themes();
};

#endif // EDITFRAME_H
