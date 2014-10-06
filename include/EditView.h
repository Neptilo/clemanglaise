#ifndef EDITVIEW_H
#define EDITVIEW_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPushButton>
#include <QtNetwork>
#include <QFormLayout>
#include <QComboBox>
#include <QTextEdit>

#include "Test.h"
#include "DatabaseManager.h"

class EditView : public QWidget{
    Q_OBJECT

public:
    EditView(Test *test, const QString &title, const QHash<QString, QString> &default_values, const QString &OK_button_value, const QString &php_filename, const QString &success_message, DatabaseManager *database_manager, QWidget *parent = 0);
    ~EditView();
    void read_reply(QString reply_string="");
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
    int id_theme;
    QString php_filename;
    QHash<QString, QString> default_values;
    // list of themes
    // Elements at even indices are the theme IDs and elements at odd indices are their corresponding names.
    QStringList reply_list;
    QString success_message;
    DatabaseManager *database_manager;
    Test *test;
    // This method is called after a word has been successfully added or edited.
    // It updates attributes and the UI to allow the user to add another word. In particular it prepares for an "add" behavior instead of "update".
    void prepare_to_continue();

public slots:
    void shrink();
    void edit_word();
	void read_reply(QNetworkReply * reply); 
    void show_confirmation(QNetworkReply* reply);
    void show_confirmation(bool success);
    void back();
    void reset();
	void find_themes();
};

#endif // EDITVIEW_H
