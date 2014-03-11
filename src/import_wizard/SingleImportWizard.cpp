#include <QDebug>

#include "import_wizard/DstListPage.h"
#include "import_wizard/DuplicatePage.h"
#include "import_wizard/SingleImportWizard.h"

SingleImportWizard::SingleImportWizard(DatabaseManager *database_manager, const QHash<QString, QString> &word_data, QWidget *parent) :
    QWizard(parent),
    database_manager(database_manager),
    word_data(word_data),
    dst_test_id(0),
    dst_list_page(database_manager, this),
    duplicate_page(word_data, this)
{
    setWindowTitle(tr("Import a word"));
    QList<QWizard::WizardButton> button_layout;
    button_layout << QWizard::CancelButton << QWizard::BackButton;
    setButtonLayout(button_layout);

    // page to choose destination list of the import
    connect(&dst_list_page, SIGNAL(clicked(Test *)), this, SLOT(check_duplicates(Test *)));
    addPage(&dst_list_page);

    // page to show the possible duplicates
    addPage(&duplicate_page);
}

void SingleImportWizard::check_duplicates(Test *test)
{
    dst_test_id = test->get_id();
    if(database_manager->find_duplicates(test->get_id(), word_data["word"], duplicate_page.duplicate_keys, duplicate_page.duplicate_values)){ // word_entry.at(1) is the word.
        if(duplicate_page.duplicate_values.empty())
            import_word();
        else
            next();
    } // TODO: else show error
}

void SingleImportWizard::import_word()
{
    if(!dst_test_id){ // TODO: show error message
        qDebug() << tr("Destination test ID has not been defined.");
        reject();
    }

    // Define data to send
    // word_data["id"] is the ID in the original list. It has no use here.
    word_data["test_id"] = QString::number(dst_test_id);

    if(!database_manager->add_word(word_data)){ // TODO: show error message if it fails
        qDebug() << tr("<b>SQLite error: </b>") << database_manager->pop_last_error();
        reject();
    }

    accept();
}
