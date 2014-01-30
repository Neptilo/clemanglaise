#ifndef THEMEFRAME_H
#define THEMEFRAME_H

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

class ThemeView : public QWidget{
    Q_OBJECT

public:
    ThemeView(Test *test, const QString &title, const QStringList &default_values, const QString &OK_button_value, const QString &php_filename, const QString &success_message, DatabaseManager * database_manager, QWidget *parent);
    ~ThemeView();

    void read_reply(QString reply_string="");

private:
    QLabel* title;
    QLabel* status;
    QLineEdit* theme_edit;
    QPushButton* OK_button;
	QNetworkAccessManager nam; 
    QPushButton* cancel_button;
    QPushButton* continue_button;
    QFormLayout* layout;
    Test *test;
    QString php_filename;
    QStringList default_values;
	QStringList reply_list; 
    QString success_message;
	QLabel* theme;
	QComboBox* themes;
	DatabaseManager* database_manager;

public slots:
    void edit_theme();
    void show_confirmation(QNetworkReply* reply);
    void show_confirmation();
	void read_reply(QNetworkReply * reply); 
	void find_themes(); 
    void back();
    void reset();
};

#endif // THEMEFRAME_H
