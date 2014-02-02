#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QRadioButton>

#include "AddListView.h"
#include "TestView.h"
#include "LanguageButtons.h"
#include "Test.h"
#include "DatabaseManager.h"

class HomeView : public QWidget{
    Q_OBJECT

private:
    QPushButton add_list_button;
    AddListView *add_list_view;
    bool admin;
    DatabaseManager database_manager;
    QLayout *layout;
    QNetworkAccessManager nam;
    LanguageButtons *offline_buttons;
    LanguageButtons *online_buttons;
    QLabel *title;
    QLabel *work_offline;
    QLabel *work_remote;

public:
    explicit HomeView(bool admin = false, QWidget *parent = 0);
    ~HomeView();
    
signals:
    
public slots:
    void add_list();
    void init();
    void init_offline_test_buttons();
    void remove_add_list_view();
    void start_test(QObject *obj);
    void read_reply_lists(QNetworkReply *reply);
};

#endif // HOMEWINDOW_H
