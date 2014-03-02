#include "ImportDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

ImportDialog::ImportDialog(DatabaseManager *database_manager, QWidget *parent) :
    QDialog(parent),
    layout(this),
    question(tr("To what list do you want to import this word?"), this),
    test_buttons(database_manager->get_lists(), this),
    cancel_button(tr("&Cancel"), this)
{
    // question
    layout.addWidget(&question);

    // language buttons
    layout.addWidget(&test_buttons);
    connect(&(test_buttons.signal_mapper), SIGNAL(mapped(QObject*)), this, SLOT(choose_test_list(QObject*)));

    cancel_button.setDefault(true);
    layout.addWidget(&cancel_button);
    connect(&(cancel_button), SIGNAL(clicked()), this, SLOT(reject()));
}

void ImportDialog::choose_test_list(QObject *obj)
{
    Test *test((Test *) obj);
    dst_test_id = test->get_id();
    accept();
}
