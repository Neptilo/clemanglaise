#ifndef ADDLISTVIEW_H
#define ADDLISTVIEW_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include "DatabaseManager.h"

class AddListView : public QWidget
{
    Q_OBJECT
public:
    explicit AddListView(DatabaseManager *database_manager, QWidget *parent = 0);

private:
    QPushButton cancel_button;
    QPushButton create_button;
    DatabaseManager *database_manager;
    QLineEdit dst_edit;
    QLineEdit name_edit;
    QLineEdit src_edit;
    QLabel status;
    QLabel title;

signals:
    void add_list_success();

public slots:
    void add_list();

};

#endif // ADDLISTVIEW_H
