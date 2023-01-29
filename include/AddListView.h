#ifndef ADDLISTVIEW_H
#define ADDLISTVIEW_H

#include <QCompleter>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QWidget>

#ifndef Q_OS_WASM
    #include "DatabaseManager.h"
#endif
#include "Test.h"

class AddListView : public QWidget
{
    Q_OBJECT
public:
    explicit AddListView(
        #ifndef Q_OS_WASM
            DatabaseManager *database_manager,
        #endif
            bool remote,
            QWidget *parent = nullptr);
    ~AddListView();
    Test *get_test();

private:
    bool check_inputs(QString& src_test, QString& dst_test, QString& test_flag, QString& error);

    QCompleter *lang_completer;
    QCompleter *country_completer;
    QPushButton cancel_button;
    QPushButton create_button;
#ifndef Q_OS_WASM
    DatabaseManager *database_manager = nullptr;
#endif
    QLineEdit dst_edit;
    QLineEdit name_edit;
    QLineEdit src_edit;
    QLineEdit flag_edit;
    QLabel status;
    QLabel title;
    Test *test;

signals:
    void created(Test *test);
    void canceled();

public slots:
#ifndef Q_OS_WASM
    void add_offline_list();
#endif
    void add_online_list();
    void show_confirmation();
};

#endif // ADDLISTVIEW_H
