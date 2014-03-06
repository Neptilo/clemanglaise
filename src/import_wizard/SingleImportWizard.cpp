#include <QDebug>

#include "import_wizard/DstListPage.h"
#include "import_wizard/DuplicatePage.h"
#include "import_wizard/SingleImportWizard.h"

SingleImportWizard::SingleImportWizard(DatabaseManager *database_manager, QStringList &word_entry, QWidget *parent) :
    QWizard(parent),
    database_manager(database_manager),
    word_entry(word_entry),
    dst_test_id(0),
    dst_list_page(database_manager, this),
    duplicate_page(this)
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
    if(database_manager->find_duplicates(test->get_id(), word_entry.at(1), duplicate_page.duplicate_keys, duplicate_page.duplicate_values)){ // word_entry.at(1) is the word.
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
    QHash<QString, QString> word_data;
    // reply_list.at(0) is the ID in the original list. It has no use here.
    word_data["word"] = word_entry.at(1);
    word_data["meaning"] = word_entry.at(2);
    word_data["nature"] = word_entry.at(3);
    word_data["comment"] = word_entry.at(4);
    word_data["example"] = word_entry.at(5);
    word_data["theme"] = word_entry.at(6);
    word_data["pronunciation"] = word_entry.at(7);
    word_data["test_id"] = QString::number(dst_test_id);

    if(!database_manager->add_word(word_data)){ // TODO: show error message if it fails
        qDebug() << tr("<b>SQLite error: </b>") << database_manager->pop_last_error();
        reject();
    }

    accept();
}
