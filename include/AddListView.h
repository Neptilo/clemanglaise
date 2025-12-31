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
            Test *existing_test = nullptr,
            QWidget *parent = nullptr);

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
    Test *test;  // Pointer to the test being edited (in edit mode) or nullptr (in create mode)

signals:
    void created(Test *test);
    void canceled();

public slots:
#ifndef Q_OS_WASM
    void add_offline_list();
    void update_offline_list();
#endif
    void add_online_list();
    void update_online_list();
    void show_confirmation();
};

#endif // ADDLISTVIEW_H
