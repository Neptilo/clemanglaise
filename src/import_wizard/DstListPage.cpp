#include "import_wizard/DstListPage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

DstListPage::DstListPage(DatabaseManager *database_manager, bool multiple, QWidget *parent) :
    QWizardPage(parent),
    database_manager(database_manager),
    layout(this),
    question(this),
    test_buttons(database_manager->get_lists(), true, this),
    add_list_view(nullptr)
{
    // question
    if(multiple)
        question.setText(tr("To what list do you want to import this vocabulary list?"));
    else
        question.setText(tr("To what list do you want to import this word?"));
    question.setWordWrap(true);
    layout.addWidget(&question);

    // language buttons
    connect(&test_buttons, SIGNAL(clicked(Test*)), this, SIGNAL(chosen(Test*)));
    layout.addWidget(&test_buttons);

    // to make the QWizard call isComplete()
    emit completeChanged();
}

// to prevent QWizard from showing "Next" button
bool DstListPage::isComplete()
{
    return false;
}

void DstListPage::create_add_list_view()
{
    add_list_view = new AddListView(database_manager, false, this);
    // TODO: how to make add_list_view return the new created test id, so dst_list_page can emit "chosen" signal with Test* argument?
    connect(add_list_view, SIGNAL(created(Test*)), this, SIGNAL(chosen(Test*)));
    connect(add_list_view, SIGNAL(canceled()), this, SLOT(remove_add_list_view()));
    layout.addWidget(add_list_view);
}

void DstListPage::remove_add_list_view()
{
    delete add_list_view;
    add_list_view = nullptr;
}
