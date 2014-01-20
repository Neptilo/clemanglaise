#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QRadioButton>

#include "TestView.h"
#include "LanguageButtons.h"
#include "Test.h"
#include "DatabaseManager.h"

class HomeView : public QWidget{
    Q_OBJECT

private:
    bool admin;
    DatabaseManager database_manager;
    QLayout *layout;
    LanguageButtons *offline_buttons;
    LanguageButtons *online_buttons;
    QList<Test> tests;
    QLabel *title;
    QLabel *work_offline;
    QLabel *work_remote;

public:
    explicit HomeView(bool admin = false, QWidget *parent = 0);
    ~HomeView();
    
signals:
    
public slots:
    void start_test(int i);
    void init();
};

#endif // HOMEWINDOW_H
