#include <QDebug>

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
    duplicate_page(word_data, this)
{
    setWindowTitle(tr("Import a word"));

    // page to choose destination list of the import
    connect(&dst_list_page, SIGNAL(clicked(Test *)), this, SLOT(check_duplicates(Test *)));
    addPage(&dst_list_page);

    // page to show the possible duplicates
    connect(&duplicate_page, SIGNAL(merge_word(QHash<QString,QString>)), this, SLOT(merge_word(QHash<QString,QString>)));
    addPage(&duplicate_page);
}

void SingleImportWizard::check_duplicates(Test *test)
{
    dst_test_id = test->get_id();
    if(database_manager->find_duplicates(test->get_id(), word_data["word"], duplicate_page.duplicate_keys, duplicate_page.duplicate_values)){ // word_entry.at(1) is the word.
        if(duplicate_page.duplicate_values.empty())
            if(import_word(word_data)){
                accept();
            }else{
                reject();
            }
        else
            next();
    } // TODO: else show error
}

void SingleImportWizard::merge_word(const QHash<QString, QString> &word_to_merge_data)
{
    if(!dst_test_id){ // TODO: show error message
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

    word_data["test_id"] = QString::number(dst_test_id);
    if(!database_manager->update_word(word_data)){ // TODO: show error message if it fails
        qDebug() << tr("<b>SQLite error: </b>") << database_manager->pop_last_error();
        reject();
    }

    accept();
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
