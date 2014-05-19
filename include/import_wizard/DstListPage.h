#ifndef DSTLISTPAGE_H
#define DSTLISTPAGE_H

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWizardPage>

#include "AddListView.h"
#include "DatabaseManager.h"
#include "LanguageButtons.h"

class DstListPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DstListPage(DatabaseManager *database_manager, QWidget *parent = 0);    
    bool isComplete();
private:
    DatabaseManager *database_manager;
    QVBoxLayout layout;
    QLabel question;
    LanguageButtons test_buttons;
    AddListView *add_list_view;

signals:
    void chosen(Test *test);

public slots:
    void create_add_list_view();
    void remove_add_list_view();
private slots:

};

#endif // DSTLISTPAGE_H
