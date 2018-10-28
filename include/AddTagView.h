#ifndef THEMEVIEW_H
#define THEMEVIEW_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtNetwork>
#include <QFormLayout>
#include <QComboBox>
#include <QTextEdit>

#include "Test.h"
#include "DatabaseManager.h"

class AddTagView : public QWidget{
    Q_OBJECT

public:
    AddTagView(Test *test, const QString &title, const QStringList &default_values, const QString &OK_button_value, const QString &php_filename, const QString &success_message, DatabaseManager * database_manager, QWidget *parent);
    ~AddTagView();

    void read_reply(QString reply_string="");

private:
    QLabel* title;
    QLabel* status;
    QLineEdit* tag_edit;
    QPushButton* OK_button;
    QNetworkAccessManager nam; 
    QPushButton* continue_button;
    QFormLayout* layout;
    Test *test;
    QString php_filename;
    QStringList default_values;
    QStringList reply_list; 
    QString success_message;
    QLabel* tag;
    QComboBox* tags;
    DatabaseManager* database_manager;

public slots:
    void edit_tag();
    void show_confirmation(QNetworkReply* reply);
    void show_confirmation();
    void read_reply(QNetworkReply * reply); 
    void find_tags(); 
    void reset();
};

#endif // THEMEVIEW_H
