#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QLabel>
#include <QList>
#include <QRadioButton>
#include <QSlider>
#include <QWidget>

#include "AddListView.h"
#ifndef Q_OS_WASM
#include "DatabaseManager.h"
#endif
#include "LanguageButtons.h"
#include "Switcher.h"
#include "Test.h"
#include "TestView.h"

class HomeView : public QWidget{
    Q_OBJECT

private:
    AddListView *add_list_view;
    bool admin;
#ifndef Q_OS_WASM
    DatabaseManager database_manager;
#endif
    QLayout *layout;
    bool remote;
    LanguageButtons *test_buttons;
    Switcher test_source_switcher; // switches between a local source and an online server to fetch the list of tests
    QLabel *title;
    QLabel info_label;

public:
    explicit HomeView(bool admin = false, QWidget *parent = nullptr);
    ~HomeView();
    
signals:
    
public slots:
    void add_list();
    void init();
    void shrink();
    void list_created(Test *test);
    void start_test(Test *test);
    void read_reply_lists();
    void remove_add_list_view();
    void set_test_source(bool remote);

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // HOMEWINDOW_H
