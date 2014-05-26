#include <QDebug>

#include "AddListView.h"
#include "import_wizard/DstListPage.h"
#include "import_wizard/DuplicatePage.h"
#include "import_wizard/Importer.h"
#include "import_wizard/SingleImportWizard.h"
#include "string_utils.h"

SingleImportWizard::SingleImportWizard(DatabaseManager *database_manager, const QHash<QString, QString> &word_data, QWidget *parent) :
    QWizard(parent),
    Importer(database_manager),
    word_data(word_data),
    dst_list_page(database_manager, this),
    duplicate_page(word_data, this),
    dst_test(NULL)
{
    setWindowTitle(tr("Import a word"));

    // page to choose destination list of the import
    connect(&dst_list_page, SIGNAL(chosen(Test *)), this, SLOT(check_duplicates(Test *)));
    addPage(&dst_list_page);

    // page to show the possible duplicates
    connect(&duplicate_page, SIGNAL(choose_behavior(int)), this, SLOT(choose_behavior(int)));
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
        if(database_manager->find_duplicates(test->get_id(), word_data["word"], duplicate_page.duplicate_keys, duplicate_page.duplicate_values)){ // word_entry.at(1) is the word.
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
        dst_list_page.create_add_list_view();
    }
}

bool SingleImportWizard::import_word()
{
    return import(dst_test->get_id(), word_data);
}

bool SingleImportWizard::merge_word(const QHash<QString, QString> &word_to_merge_data)
{
    if(!dst_test->get_id()){ // TODO: show error message
        qDebug() << tr("Destination test ID has not been defined.");
        reject();
    }
    QHash<QString, QString>::iterator i;
    for (i = word_data.begin(); i != word_data.end(); ++i){
        if(i.key() != "id"){
            QRegExp split_sep;
            QString join_sep;
            if(i.key() == "comment" || i.key() == "example"){
                split_sep = QRegExp("<br ?/>", Qt::CaseInsensitive);
                join_sep = "<br />";
            }else{
                split_sep = QRegExp(",");
                join_sep = ", ";
            }
            i.value() = merge_string(i.value(), word_to_merge_data[i.key()], split_sep, join_sep);
        }
    }

    if(!database_manager->update_word(dst_test->get_id(), word_data)){ // TODO: show error message if it fails
        qDebug() << tr("<b>SQLite error: </b>") << database_manager->pop_last_error();
        return false;
    }else{
        return true;
    }
}

void SingleImportWizard::choose_behavior(int behavior)
{
    chosen_behavior = behavior;
}

QString SingleImportWizard::merge_string(
        const QString &left_string,
        const QString &right_string,
        const QRegExp &split_sep,
        const QString &join_sep)
{
    QSet<QString> left_set = trimmed(left_string.split(split_sep)).toSet();
    QSet<QString> right_set = trimmed(right_string.split(split_sep)).toSet();
    left_set.unite(right_set);
    return QStringList(left_set.toList()).join(join_sep);
}

// function called when "Finish" button is clicked
// If an error occurs, QWizard::reject() will be called instead of QWizard::accept().
void SingleImportWizard::accept()
{
    bool success;
    switch(chosen_behavior){
    case 0:
        // import anyway
        success = import_word();
        break;
    case 1:
    {
        // merge
        const QHash<QString, QString> word_to_merge_data = duplicate_page.get_word_to_merge();
        success = merge_word(word_to_merge_data);
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
