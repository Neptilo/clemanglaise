#include "import_wizard/ListImportWizard.h"
#include "import_wizard/ImportBehavior.h"

#include <QComboBox>
#include <QDebug>
#include <QNetworkReply>

#include "duplicate_utils.h"
#include "import_wizard/Importer.h"
#include "import_wizard/SingleImportWizard.h"
#include "NetworkReplyReader.h"

#include <import_wizard/DuplicatePage.h>

ListImportWizard::ListImportWizard(DatabaseManager *database_manager, Test *test, QWidget *parent) :
    QWizard(parent),
    Importer(database_manager),
    src_test(test),
    dst_test(nullptr),
    dst_list_page(database_manager, true, this),
    behavior_page(this),
    progress_page(this),
    nb_inserted(0),
    nb_replaced(0),
    nb_updated(0),
    nb_discarded(0),
    nb_failed(0),
    reply_list(nullptr)
{
    setWindowTitle(tr("Import a vocabulary list"));
#ifdef Q_OS_ANDROID
    setWindowState(Qt::WindowMaximized);
#endif

    // page to choose destination list of the import
    connect(&dst_list_page, SIGNAL(chosen(Test *)), this, SLOT(save_and_next(Test *)));
    setPage(Page_DstList, &dst_list_page);

    // page to choose import behavior
    setPage(Page_Behavior, &behavior_page);
    connect(&behavior_page, SIGNAL(choose_behavior(int)), this, SLOT(choose_behavior(int)));
    choose_behavior(0); // don't check for duplicates

    // page to show progress and status
    setPage(Page_Progress, &progress_page);
    connect(&progress_page, SIGNAL(ready()), this, SLOT(import_tags_and_list())); // emitted when page shows up
    connect(&progress_page, SIGNAL(completeChanged()), this, SLOT(update_on_complete()));
    setOption(QWizard::NoBackButtonOnLastPage);

    // page to show possible duplicates for specific duplicates if user has chosen to be prompted for every detected duplicate
    //    setPage(Page_Duplicates, &duplicate_page);
}

int ListImportWizard::nextId() const
{
    switch (currentId()) {
    case Page_DstList:
        if(dst_test){
            int nb_rows;
            if(database_manager->count(dst_test->get_id(), nb_rows)){
                if(nb_rows){
                    return Page_Behavior;
                }else{
                    return Page_Progress;
                }
            }else{
                qDebug() << database_manager->pop_last_error();
                return -1;
            }
        }else
            return -1;
    case Page_Behavior:
        return Page_Progress;
    case Page_Duplicates:
        return Page_Progress;
    case Page_Progress:
        // If the user chose to be asked for each duplicate, the next page is the one that shows the list of duplicates for the word being imported.
        if(chosen_behavior != ImportBehavior::Ask || progress_page.isComplete())
            return -1;
        else
            return -1;
    default:
        return -1;
    }
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

void ListImportWizard::import_tags_and_list()
{
    QString behavior_text;
    switch (chosen_behavior) {
    case ImportBehavior::DontCheck:
        behavior_text = tr("without checking for duplicates");
        break;
    case ImportBehavior::Discard:
        behavior_text = tr("and discarding duplicates");
        break;
    case ImportBehavior::Replace:
        behavior_text = tr("and replacing duplicates");
        break;
    case ImportBehavior::Merge:
        behavior_text = tr("and merging duplicates");
        break;
    case ImportBehavior::Ask:
        behavior_text = tr("and prompting for each duplicate");
        break;
    default:
        qDebug() << tr("Invalid behavior code (%1)").arg(chosen_behavior);
        return;
    }
    progress_page.setSubTitle(tr("Importing list <b>%1</b> to <b>%2</b> %3.").arg(src_test->get_name()).arg(dst_test->get_name()).arg(behavior_text));

    // request to PHP file for the list of all words
    const QUrl url = QUrl(QString("https://neptilo.com/php/clemanglaise/search.php?list_id=%1").arg(src_test->get_id()));
    progress_page.set_status(tr("Retrieving vocabulary list from server"));
    QNetworkReply* reply = NetworkReplyReader::nam->get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(read_reply()));
}

void ListImportWizard::read_reply()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    QString reply_string = reply->readAll().replace('\0', "");
    reply->deleteLater();
    reply_list = new QStringList(reply_string.split('\n'));

    // find used tag names in list
    const QUrl url = QUrl(QString("https://neptilo.com/php/clemanglaise/find_used_tags.php?list_id=%1").arg(src_test->get_id()));
    progress_page.set_status(tr("Retrieving tag names from server"));
    QNetworkReply* tag_reply = NetworkReplyReader::nam->get(QNetworkRequest(url));
    connect(tag_reply, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(read_tag_reply(QNetworkReply*)));
}

void ListImportWizard::read_tag_reply(QNetworkReply* reply)
{
    QString reply_string = reply->readAll().replace('\0', "");
    reply->deleteLater();
    QStringList tag_reply_list = reply_string.split('\n', QString::SkipEmptyParts);
    online_tag_ids.clear();
    tag_names.clear();
    for(int i = 0, l = tag_reply_list.size(); i < l-1; i += 2) {
        online_tag_ids << tag_reply_list.at(i).toInt();
        tag_names << tag_reply_list.at(i+1);
    }

    QStringList word_keys;
    // has to be consistent with the actual query in the PHP file
    word_keys << "id" << "word" << "meaning" << "pronunciation" << "nature" << "comment" << "example" << "hint"  << "score" << "tag_ids";
    if (!reply_list) {
        progress_page.append_log(tr("<b>Error:</b> reply_list is nullptr."));
        return;
    }
    int nb_words = reply_list->size()/word_keys.size();

    // Progress steps: one for each local tag search and one for each word import
    progress_page.set_max_progress(tag_names.size()+nb_words);

    // search for tag names in local database and create missing ones
    // TODO: factor this in Importer.cpp
    offline_tag_ids = QList<int>();
    for (int i = 0; i < tag_names.size(); ++i) {
        progress_page.set_status(
                    tr("Looking for tag <b>%1</b> in local database")
                    .arg(tag_names.at(i).trimmed()));
        QRegularExpression re(
            "\\s*" + QRegularExpression::escape(tag_names.at(i).trimmed()) + "\\s*",
            QRegularExpression::CaseInsensitiveOption);
        database_manager->find_tags(tag_reply_list); // reply_list's contents are replaced
        int tag_name_ind = tag_reply_list.indexOf(re);
        if (tag_name_ind >= 0) {
            // The tag name was found
            // add the corresponding id to the offline id list
            offline_tag_ids << tag_reply_list.at(tag_name_ind-1).toInt();
        } else {
            // The tag name was not found
            // create this tag in the local database
            int tag_id;
            progress_page.set_status(
                        tr("Adding tag <b>%1</b> to local database")
                        .arg(tag_names.at(i).trimmed()));
            if (database_manager->add_tag(tag_names.at(i), tag_id))
                offline_tag_ids << tag_id;
            else {
                offline_tag_ids << -1;
                progress_page.append_log(
                            tr("<b>SQLite error: </b>%1")
                            .arg(database_manager->pop_last_error()));
                progress_page.set_status(tr("Import failed."));
                return;
            }
        }
        progress_page.increase_progress();
    }

    // import words
    for(int i = 0; i < nb_words; ++i){
        QHash<QString, QString> word_data;
        for(int j = 0; j < word_keys.size(); ++j)
            word_data[word_keys.at(j)] = reply_list->at(i*word_keys.size()+j);
        // TODO add list_id in reply_string ?
        word_data["list_id"] = QString::number(dst_test->get_id());

        // update word_data's tag ids with new local ids
        QStringList online_tag_ids_str = word_data["tag_ids"].split(", ", QString::SkipEmptyParts);
        QStringList word_tag_ids_str;
        for (int j = 0; j < online_tag_ids_str.size(); ++j) {
            int tag_id = offline_tag_ids.at(online_tag_ids.indexOf(online_tag_ids_str.at(j).toInt()));
            word_tag_ids_str << QString::number(tag_id);
        }
        word_data["tag_ids"] = word_tag_ids_str.join(", ");

        progress_page.set_status(tr("Importing \"<b>%1</b>\"").arg(word_data["word"]));

        // Do different things according to chosen behavior
        if(chosen_behavior == ImportBehavior::DontCheck){
            if(import(word_data))
                ++nb_inserted;
            else{
                progress_page.append_log(tr("<b>Error: </b>%1").arg(get_error()));
                ++nb_failed;
            }
        }else{
            // Check duplicates
            QStringList duplicate_keys;
            QList<QStringList> duplicate_values;
            if(database_manager->find_duplicates(dst_test->get_id(), word_data["word"], duplicate_keys, duplicate_values)){
                if(duplicate_values.empty()){
                    // No duplicate found
                    if(import(word_data))
                        ++nb_inserted;
                    else{
                        progress_page.append_log(tr("<b>Error: </b>%1").arg(get_error()));
                        ++nb_failed;
                    }
                }else{
                    // Duplicate found
                    switch (chosen_behavior) {
                    case ImportBehavior::Replace:
                        // guess most probable duplicate from duplicate_values, and update it.
                    {
                        int best_duplicate_ind = find_best_duplicate(word_data, duplicate_keys, duplicate_values);
                        word_data["id"] = duplicate_values.at(best_duplicate_ind).at(duplicate_keys.indexOf("id"));
                        if(database_manager->update_word(word_data))
                            ++nb_replaced;
                        else{
                            progress_page.append_log(tr("<b>SQLite error: </b>%1").arg(database_manager->pop_last_error()));
                            ++nb_failed;
                        }
                    }
                        break;
                    case ImportBehavior::Merge:
                        // same as before and merge like in merge_word from SingleImportWizard
                    {
                        int best_duplicate_ind = find_best_duplicate(word_data, duplicate_keys, duplicate_values);
                        // rebuild data of the duplicate
                        QHash<QString, QString> best_duplicate_data;
                        for(int i = 0; i < duplicate_keys.size(); ++i)
                            best_duplicate_data[duplicate_keys.at(i)] = duplicate_values.at(best_duplicate_ind).at(i);
                        word_data = merge_word(word_data, best_duplicate_data);
                        word_data["id"] = duplicate_values.at(best_duplicate_ind).at(duplicate_keys.indexOf("id"));
                        if(database_manager->update_word(word_data))
                            ++nb_updated;
                        else{
                            progress_page.append_log(tr("<b>SQLite error: </b>%1").arg(database_manager->pop_last_error()));
                            ++nb_failed;
                        }
                    }
                        break;
                    case ImportBehavior::Ask:
                    {
                        SingleImportWizard single_import_wizard(database_manager, word_data, dst_test, this);
                        if(single_import_wizard.exec()){
                            // Show confirmation
                            progress_page.set_status(tr("Import succeeded!"));
                            switch (single_import_wizard.chosen_behavior) {
                            case ImportBehavior::DontCheck:
                                ++nb_inserted;
                                break;
                            case ImportBehavior::Merge:
                                ++nb_updated;
                                break;
                            default:
                                qDebug() << tr("This should not happen. The single import wizard succeeded but the behavior is unknown (%1).").arg(single_import_wizard.chosen_behavior);
                                break;
                            }
                        }else
                            ++nb_failed;
                    }
                        break;
                    case ImportBehavior::Discard:
                        ++nb_discarded;
                        break;
                    default:
                        qDebug() << tr("This should not happen. Import behavior \"%1\" is not recognized.").arg(chosen_behavior);
                    }
                }
            } // TODO: else show error
        }

        progress_page.increase_progress();
    }
    delete reply_list;
    reply_list = nullptr;

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

void ListImportWizard::update_on_complete()
{
    if(progress_page.isComplete()){
        // disable cancel button
        button(QWizard::CancelButton)->setEnabled(false);
    }
}
