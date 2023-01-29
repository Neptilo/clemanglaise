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
#ifndef Q_OS_WASM
#include "DatabaseManager.h"
#endif

class AddTagView : public QWidget{
    Q_OBJECT

public:
    AddTagView(Test *test,
               const QString &title,
               const QStringList &default_values,
               const QString &OK_button_value,
               const QString &php_filename,
               const QString &success_message,
           #ifndef Q_OS_WASM
               DatabaseManager * database_manager,
           #endif
               QWidget *parent);
    ~AddTagView();

    void read_reply(QString reply_string);

private:
    QLabel* title;
    QLabel* status;
    QLineEdit* tag_edit;
    QPushButton* OK_button;
    QPushButton* continue_button;
    QFormLayout* layout;
    Test *test;
    QString php_filename;
    QStringList default_values;
    QStringList reply_list; 
    QString success_message;
    QLabel* tag;
    QComboBox* tags;
#ifndef Q_OS_WASM
    DatabaseManager* database_manager;
#endif

public slots:
    void edit_tag();
    void read_edit_tag_reply();
    void show_confirmation();
    void read_reply();
    void find_tags(); 
    void reset();
};

#endif // THEMEVIEW_H
