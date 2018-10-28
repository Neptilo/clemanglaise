#include "import_wizard/SingleImportWizard.h"

#include <QDebug>
#include <QNetworkReply>

#include "AddListView.h"
#include "duplicate_utils.h"
#include "import_wizard/DstListPage.h"
#include "import_wizard/DuplicatePage.h"
#include "import_wizard/Importer.h"
#include "string_utils.h"

SingleImportWizard::SingleImportWizard(DatabaseManager *database_manager, const QHash<QString, QString> &word_data, Test *dst_test, QWidget *parent) :
    QWizard(parent),
    Importer(database_manager),
    chosen_behavior(ImportBehavior::DontCheck),
    word_data(word_data),
    dst_list_page(nullptr),
    duplicate_page(this),
    progress_page(this),
    dst_test(dst_test),
    tag_nam(this)
{
    setWindowTitle(tr("Import a word"));
#ifdef Q_OS_ANDROID
    setWindowState(Qt::WindowMaximized);
#endif

    if(dst_test)
        check_duplicates(dst_test);
    else{
        // page to choose destination list of the import
        dst_list_page = new DstListPage(database_manager, false, this);
        connect(dst_list_page, SIGNAL(chosen(Test *)), this, SLOT(check_duplicates(Test *)));
        setPage(Page_DstList, dst_list_page);
    }

    // page to show the possible duplicates
    duplicate_page.word_data = &word_data;
    connect(&duplicate_page, SIGNAL(choose_behavior(ImportBehavior::Behavior)), this, SLOT(choose_behavior(ImportBehavior::Behavior)));
    setPage(Page_Duplicates, &duplicate_page);

    // page to show progress and status
    setPage(Page_Progress, &progress_page);
    connect(&progress_page, SIGNAL(ready()), this, SLOT(import_tags_and_word())); // emitted when page shows up
    connect(&progress_page, SIGNAL(completeChanged()), this, SLOT(update_on_complete()));
    setOption(QWizard::NoBackButtonOnLastPage);

    connect(&tag_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_tag_reply(QNetworkReply*)));
}

int SingleImportWizard::nextId() const
{
    switch (currentId()) {
    case Page_DstList:
        if(duplicate_page.duplicate_values.empty())
            return Page_Progress;
        else
            return Page_Duplicates;
    case Page_Duplicates:
        return Page_Progress;
    case Page_Progress:
        return -1;
    default:
        return -1;
    }
}

void SingleImportWizard::showEvent(QShowEvent *)
{
    button(QWizard::NextButton)->setEnabled(false);
}

void SingleImportWizard::check_duplicates(Test *test)
{
    if(test){
        word_data["list_id"] = QString::number(test->get_id());
        dst_test = test;
        duplicate_page.setSubTitle(tr("The word \"<b>%1</b>\" will be imported in <b>%2</b>.").arg(word_data["word"]).arg(dst_test->get_name()));
        if(database_manager->find_duplicates(test->get_id(), word_data["word"], duplicate_page.duplicate_keys, duplicate_page.duplicate_values)){
            if(duplicate_page.duplicate_values.empty()) {
                duplicate_page.setSubTitle(tr("The word \"<b>%1</b>\" will be imported in <b>%2</b>.").arg(word_data["word"]).arg(test->get_name()));
                next();
            } else
                next();
        } else
            progress_page.append_log(
                        tr("<b>SQLite error: </b>%1")
                        .arg(database_manager->pop_last_error()));
    }else{
        // create vocabulary list
        dst_list_page->create_add_list_view();
    }
}

bool SingleImportWizard::update_word(const QHash<QString, QString> &word_data)
{
    if(!dst_test->get_id()){
        progress_page.append_log(tr("Destination test id has not been defined."));
    }

    if(!database_manager->update_word(word_data)){
        progress_page.append_log(
                    tr("<b>SQLite error: </b>%1")
                    .arg(database_manager->pop_last_error()));
        return false;
    }else{
        return true;
    }
}

void SingleImportWizard::choose_behavior(ImportBehavior::Behavior behavior)
{
    chosen_behavior = behavior;
}

void SingleImportWizard::import_tags_and_word()
{
    // retrieve word's tag ids
    QStringList online_tag_ids_str = word_data["tag_ids"].split(", ", QString::SkipEmptyParts);
    // Progress steps: one for PHP request to find tags, one for each local tag search and one for the word import
    progress_page.set_max_progress(online_tag_ids_str.size()+2);
    online_tag_ids = QList<int>();
    for (int i = 0; i < online_tag_ids_str.size(); ++i)
        online_tag_ids << online_tag_ids_str.at(i).toInt();

    // find name of each tag
    find_tags();
}

void SingleImportWizard::find_tags() {
    // Request to PHP file
    const QUrl url = QUrl("https://neptilo.com/php/clemanglaise/find_tags.php");
    QNetworkRequest request(url);
    progress_page.set_status(tr("Retrieving tag names from server"));
    tag_nam.get(request);
}

void SingleImportWizard::read_tag_reply(QNetworkReply *reply) {
    progress_page.increase_progress();
    bool success = true;

    // find names of tags
    QString reply_string = reply->readAll().replace('\0', "");
    reply->deleteLater();
    QStringList reply_list = reply_string.split('\n', QString::SkipEmptyParts);
    tag_names = QStringList();
    for (int i = 0; i < online_tag_ids.size(); ++i) {
        int tag_id = online_tag_ids.at(i);
        int tag_id_ind = reply_list.indexOf(QString::number(tag_id));
        if (tag_id_ind >= 0) // should always be true
            tag_names << reply_list.at(tag_id_ind+1);
        else
            tag_names << "";
    }

    // search for tag names in local database and create missing ones
    offline_tag_ids = QList<int>();
    for (int i = 0; i < tag_names.size(); ++i) {
        progress_page.set_status(
                    tr("Looking for tag <b>%1</b> in local database")
                    .arg(tag_names.at(i).trimmed()));
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        QRegExp re(
                    "\\s*" + QRegExp::escape(tag_names.at(i).trimmed()) + "\\s*",
                    Qt::CaseInsensitive);
#else
        QRegularExpression re(
                    "\\s*" + QRegularExpression::escape(tag_names.at(i).trimmed()) + "\\s*",
                    QRegularExpression::CaseInsensitiveOption);
#endif
        database_manager->find_tags(reply_list); // reply_list's contents are replaced
        int tag_name_ind = reply_list.indexOf(re);
        if (tag_name_ind >= 0) {
            // The tag name was found
            // add the corresponding id to the offline id list
            offline_tag_ids << reply_list.at(tag_name_ind-1).toInt();
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
                success = false;
            }
        }
        progress_page.increase_progress();
    }

    // update word_data's tag ids with new local ids
    QStringList online_tag_ids_str = word_data["tag_ids"].split(", ", QString::SkipEmptyParts);
    QStringList word_tag_ids_str;
    for (int i = 0; i < online_tag_ids_str.size(); ++i) {
        int tag_id = offline_tag_ids.at(online_tag_ids.indexOf(online_tag_ids_str.at(i).toInt()));
        word_tag_ids_str << QString::number(tag_id);
    }
    word_data["tag_ids"] = word_tag_ids_str.join(", ");

    // import word
    progress_page.set_status(tr("Importing word"));
    switch(chosen_behavior){
    case ImportBehavior::DontCheck:
        // import anyway
        success &= import(word_data);
        break;
    case ImportBehavior::Merge:
    {
        const QHash<QString, QString> word_to_merge_data = duplicate_page.get_word_to_merge();
        QHash<QString, QString> merged_word_data = merge_word(word_data, word_to_merge_data);
        merged_word_data["id"] = word_to_merge_data["id"];
        success &= update_word(merged_word_data);
        break;
    }
    case ImportBehavior::Replace:
    {
        word_data["id"] = duplicate_page.get_word_to_merge()["id"];
        success &= update_word(word_data);
        break;
    }
    default:
        progress_page.append_log(
                    tr("Invalid import behavior code (%1). Maybe it has not been initialized.")
                    .arg(chosen_behavior));
        success = false;
    }
    progress_page.increase_progress();
    if(success)
        progress_page.set_status(tr("Import succeeded."));
    else
        progress_page.set_status(tr("Import failed."));
}

void SingleImportWizard::update_on_complete()
{
    if(progress_page.isComplete()){
        // disable cancel button
        button(QWizard::CancelButton)->setEnabled(false);
    }
}
