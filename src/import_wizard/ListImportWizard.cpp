#include "import_wizard/ListImportWizard.h"

#include <QComboBox>
#include <QNetworkReply>

#include "duplicate_utils.h"
#include "import_wizard/Importer.h"

ListImportWizard::ListImportWizard(DatabaseManager *database_manager, Test *test, QWidget *parent) :
    QWizard(parent),
    Importer(database_manager),
    src_test(test),
    dst_test(NULL),
    dst_list_page(database_manager, this),
    behavior_page(this),
    progress_page(this),
    nam(this),
    nb_inserted(0),
    nb_replaced(0),
    nb_updated(0),
    nb_discarded(0),
    nb_failed(0)
{
    setWindowTitle(tr("Import a vocabulary list"));

    // page to choose destination list of the import
    connect(&dst_list_page, SIGNAL(chosen(Test *)), this, SLOT(save_and_next(Test *)));
    addPage(&dst_list_page);

    // page to choose import behavior
    addPage(&behavior_page);
    connect(&behavior_page, SIGNAL(choose_behavior(int)), this, SLOT(choose_behavior(int)));
    choose_behavior(3); // merge

    // page to show progress and status
    addPage(&progress_page);
    connect(&progress_page, SIGNAL(import_list()), this, SLOT(import_list())); // emitted when page shows up
    setOption(QWizard::NoBackButtonOnLastPage);

    connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
}

void ListImportWizard::showEvent(QShowEvent *)
{
     button(QWizard::NextButton)->setEnabled(false);
}

void ListImportWizard::save_and_next(Test *test)
{
    if(test){
        dst_test = test;
        behavior_page.setSubTitle(tr("The list <b>%1</b> will be imported in <b>%2</b>.").arg(src_test->get_name()).arg(dst_test->get_name()));
        next();
    }else{
        // create vocabulary list
        // This will call save_and_next again with a non null argument.
        dst_list_page.create_add_list_view();
    }
}

void ListImportWizard::choose_behavior(int behavior)
{
    chosen_behavior = behavior;
}

void ListImportWizard::import_list()
{
    QString behavior_text;
    switch (chosen_behavior) {
    case 0:
        behavior_text = tr("without checking for duplicates");
        break;
    case 1:
        behavior_text = tr("and discarding duplicates");
        break;
    case 2:
        behavior_text = tr("and replacing duplicates");
        break;
    case 3:
        behavior_text = tr("and merging duplicates");
        break;
    case 4:
        behavior_text = tr("and prompting for each duplicate");
        break;
    default:
        qDebug() << tr("Invalid behavior code (%1)").arg(chosen_behavior);
        return;
    }
    progress_page.setSubTitle(tr("Importing list <b>%1</b> to <b>%2</b> %3.").arg(src_test->get_name()).arg(dst_test->get_name()).arg(behavior_text));
    // request to PHP file for the list of all words
    const QUrl url = QUrl(QString("http://neptilo.com/php/clemanglaise/search.php?test_id=%1").arg(src_test->get_id()));
    nam.get(QNetworkRequest(url));
}

void ListImportWizard::read_reply(QNetworkReply* reply)
{
    QString reply_string = reply->readAll();
    reply->deleteLater();
    QStringList word_keys;
    // has to be consistent with the actual query in the PHP file
    word_keys << "id" << "word" << "meaning" << "pronunciation" << "nature" << "comment" << "example" << "id_theme"  << "score" << "theme";
    QStringList reply_list = reply_string.split('\n');
    int nb_words = reply_list.size()/word_keys.size();
    progress_page.set_max_progress(nb_words);
    for(int i = 0; i < nb_words; ++i){

        QHash<QString, QString> word_data;
        for(int j = 0; j < word_keys.size(); ++j){
            word_data[word_keys.at(j)] = reply_list.at(i*word_keys.size()+j);
        }
        progress_page.set_status(tr("Importing \"<b>%1</b>\"").arg(word_data["word"]));

        // Do different things according to chosen behavior
        if(chosen_behavior == 0){
            if(import(dst_test->get_id(), word_data))
                ++nb_inserted;
            else
                ++nb_failed;
        }else{
            // Check duplicates
            QStringList duplicate_keys;
            QList<QStringList> duplicate_values;
            if(database_manager->find_duplicates(dst_test->get_id(), word_data["word"], duplicate_keys, duplicate_values)){
                if(duplicate_values.empty()){
                    // No duplicate found
                    if(import(dst_test->get_id(), word_data))
                        ++nb_inserted;
                    else
                        ++nb_failed;
                }else{
                    // Duplicate found
                    switch (chosen_behavior) {
                    case 2:
                        // replace
                        // guess most probable duplicate from duplicate_values, and update it.
                    {
                        int best_duplicate_ind = find_best_duplicate(word_data, duplicate_keys, duplicate_values);
                        word_data["id"] = duplicate_values.at(best_duplicate_ind).at(duplicate_keys.indexOf("id"));
                        if(database_manager->update_word(dst_test->get_id(), word_data))
                            ++nb_replaced;
                        else
                            ++nb_failed;
                    }
                        break;
                    case 3:
                        // merge
                        // same as before and merge like in merge_word from SingleImportWizard
                    {
                        int best_duplicate_ind = find_best_duplicate(word_data, duplicate_keys, duplicate_values);
                        // rebuild data of the duplicate
                        QHash<QString, QString> best_duplicate_data;
                        for(int i = 0; i < duplicate_keys.size(); ++i)
                            best_duplicate_data[duplicate_keys.at(i)] = duplicate_values.at(best_duplicate_ind).at(i);
                        word_data = merge_word(word_data, best_duplicate_data);
                        word_data["id"] = duplicate_values.at(best_duplicate_ind).at(duplicate_keys.indexOf("id"));
                        if(database_manager->update_word(dst_test->get_id(), word_data))
                            ++nb_updated;
                        else
                            ++nb_failed;
                    }
                    default: // case 1: discard
                        break;
                    }
                }
            } // TODO: else show error
        }


        progress_page.increase_progress();
    }

    // print recap of import
    QStringList recap_list;
    if(nb_inserted)
        recap_list << tr("inserted <b>%1</b> word(s)").arg(nb_inserted);
    if(nb_replaced)
        recap_list << tr("replaced <b>%1</b> word(s)").arg(nb_replaced);
    if(nb_updated)
        recap_list << tr("updated <b>%1</b> word(s)").arg(nb_updated);
    if(nb_discarded)
        recap_list << tr("discarded <b>%1</b> word(s)").arg(nb_discarded);
    if(nb_failed)
        recap_list << tr("failed to import <b>%1</b> word(s)").arg(nb_failed);
    if(recap_list.isEmpty())
        recap_list << tr("There was nothing to do.");
    QString recap = tr("Import finished: ")+recap_list.join(tr(", "));
    progress_page.set_status(recap);
}
