#ifndef ADDLISTVIEW_H
#define ADDLISTVIEW_H

#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QWidget>

#include "DatabaseManager.h"

class AddListView : public QWidget
{
    Q_OBJECT
public:
    explicit AddListView(DatabaseManager *database_manager, bool remote, QWidget *parent = 0);

private:
    QPushButton cancel_button;
    QPushButton create_button;
    DatabaseManager *database_manager;
    QLineEdit dst_edit;
    QNetworkAccessManager *nam;
    QLineEdit name_edit;
    QLineEdit src_edit;
    QLabel status;
    QLabel title;

signals:
    void add_list_success();

public slots:
    void add_offline_list();
    void add_online_list();
    void show_confirmation(QNetworkReply* reply);
};

#endif // ADDLISTVIEW_H
