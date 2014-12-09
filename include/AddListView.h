#ifndef ADDLISTVIEW_H
#define ADDLISTVIEW_H

#include <QCompleter>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QWidget>

#include "DatabaseManager.h"
#include "Test.h"

class AddListView : public QWidget
{
    Q_OBJECT
public:
    explicit AddListView(DatabaseManager *database_manager, bool remote, QWidget *parent = 0);
    ~AddListView();
    Test *get_test();

private:
    QCompleter *completer;
    QPushButton cancel_button;
    QPushButton create_button;
    DatabaseManager *database_manager;
    QLineEdit dst_edit;
    QNetworkAccessManager *nam;
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
    void add_offline_list();
    void add_online_list();
    void show_confirmation(QNetworkReply* reply);
};

#endif // ADDLISTVIEW_H
