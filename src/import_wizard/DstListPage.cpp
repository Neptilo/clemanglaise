#include "import_wizard/DstListPage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

DstListPage::DstListPage(DatabaseManager *database_manager, QWidget *parent) :
    QWizardPage(parent),
    layout(this),
    question(tr("To what list do you want to import this word?"), this),
    test_buttons(database_manager->get_lists(), this)
{
    // question
    layout.addWidget(&question);

    // language buttons
    connect(&test_buttons, SIGNAL(clicked(Test*)), this, SLOT(forward_click(Test*)));
    layout.addWidget(&test_buttons);

    // to make the QWizard call isComplete()
    emit completeChanged();
}

void DstListPage::forward_click(Test *test)
{
    emit clicked(test);
}

// to prevent QWizard from showing "Next" button
bool DstListPage::isComplete()
{
    return false;
}
