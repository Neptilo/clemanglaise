#ifndef THEMEFRAME_H
#define THEMEFRAME_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QToolButton>
#include <QtNetwork>
#include <QFormLayout>
#include <QComboBox>
#include <QTextEdit>

#include "test.h"

class ThemeFrame : public QWidget{
    Q_OBJECT

public:
    ThemeFrame(Test &test, const QString &title, const QStringList &default_values, const QString &OK_button_value, const QString &php_filename, const QString &success_message, QWidget *parent);
    ~ThemeFrame();

    void read_reply(QString reply_string);

private:
    QLabel* title;
    QLabel* status;
    QLineEdit* theme_edit;
    QToolButton* OK_button;
	QNetworkAccessManager nam; 
    QToolButton* cancel_button;
    QToolButton* continue_button;
    QFormLayout* layout;
    Test test;
    QString php_filename;
    QStringList default_values;
    QString success_message;
	QLabel* theme;
	QComboBox* themes;

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
