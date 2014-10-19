#include <QDebug>

#include "AddListView.h"
#include "duplicate_utils.h"
#include "import_wizard/DstListPage.h"
#include "import_wizard/DuplicatePage.h"
#include "import_wizard/Importer.h"
#include "import_wizard/SingleImportWizard.h"
#include "string_utils.h"

SingleImportWizard::SingleImportWizard(DatabaseManager *database_manager, const QHash<QString, QString> &word_data, Test *dst_test, QWidget *parent) :
    QWizard(parent),
    Importer(database_manager),
    word_data(word_data),
    dst_list_page(NULL),
    duplicate_page(this),
    dst_test(dst_test)
{
    setWindowTitle(tr("Import a word"));

    if(dst_test)
        check_duplicates(dst_test);
    else{
        // page to choose destination list of the import
        dst_list_page = new DstListPage(database_manager, false, this);
        connect(dst_list_page, SIGNAL(chosen(Test *)), this, SLOT(check_duplicates(Test *)));
        addPage(dst_list_page);
    }

    // page to show the possible duplicates
    duplicate_page.word_data = &word_data;
    connect(&duplicate_page, SIGNAL(choose_behavior(ImportBehavior::Behavior)), this, SLOT(choose_behavior(ImportBehavior::Behavior)));
    addPage(&duplicate_page);
}

void SingleImportWizard::showEvent(QShowEvent *)
{
    button(QWizard::NextButton)->setEnabled(false);
}

void SingleImportWizard::check_duplicates(Test *test)
{
    if(test){
        dst_test = test;
        duplicate_page.setSubTitle(tr("The word \"<b>%1</b>\" will be imported in <b>%2</b>.").arg(word_data["word"]).arg(dst_test->get_name()));
        if(database_manager->find_duplicates(test->get_id(), word_data["word"], duplicate_page.duplicate_keys, duplicate_page.duplicate_values)){
            if(duplicate_page.duplicate_values.empty()){
                if(import_word())
                    QWizard::accept();
                else
                    reject();
            }else{
                duplicate_page.setSubTitle(tr("The word \"<b>%1</b>\" will be imported in <b>%2</b>.").arg(word_data["word"]).arg(test->get_name()));
                next();
            }
        } // TODO: else show error
    }else{
        // create vocabulary list
        dst_list_page->create_add_list_view();
    }
}

bool SingleImportWizard::import_word()
{
    return import(word_data);
}

bool SingleImportWizard::update_word(const QHash<QString, QString> &word_data)
{
    if(!dst_test->get_id()){ // TODO: show error message
        qDebug() << tr("Destination test ID has not been defined.");
        reject();
    }

    if(!database_manager->update_word(word_data)){ // TODO: show error message if it fails
        qDebug() << tr("<b>SQLite error: </b>") << database_manager->pop_last_error();
        return false;
    }else{
        return true;
    }
}

void SingleImportWizard::choose_behavior(ImportBehavior::Behavior behavior)
{
    chosen_behavior = behavior;
}

// function called when "Finish" button is clicked
// If an error occurs, QWizard::reject() will be called instead of QWizard::accept().
// TODO: replace by QWizardPage::validatePage()
void SingleImportWizard::accept()
{
    bool success;
    switch(chosen_behavior){
    case ImportBehavior::DontCheck:
        // import anyway
        success = import_word();
        break;
    case ImportBehavior::Merge:
    {
        const QHash<QString, QString> word_to_merge_data = duplicate_page.get_word_to_merge();
        QHash<QString, QString> merged_word_data = merge_word(word_data, word_to_merge_data);
        merged_word_data["id"] = word_to_merge_data["id"];
        success = update_word(merged_word_data);
        break;
    }
    case ImportBehavior::Replace:
    {
        success = update_word(word_data);
        break;
    }
    default:
        qDebug() << tr("Invalid import behavior code (%1). Maybe it has not been initialized.").arg(chosen_behavior);
        success = false;
    }
    if(success)
        QWizard::accept();
    else
        reject();
}
