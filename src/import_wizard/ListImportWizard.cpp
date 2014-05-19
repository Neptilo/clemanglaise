#include "import_wizard/ListImportWizard.h"
#include "import_wizard/Importer.h"

#include <QComboBox>
#include <QNetworkReply>

ListImportWizard::ListImportWizard(DatabaseManager *database_manager, Test *test, QWidget *parent) :
    QWizard(parent),
    Importer(database_manager),
    test(test),
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
    button(QWizard::NextButton)->setEnabled(false);

    // page to choose destination list of the import
    connect(&dst_list_page, SIGNAL(chosen(Test *)), this, SLOT(save_and_next(Test *)));
    addPage(&dst_list_page);

    // page to choose import behavior
    addPage(&behavior_page);
    connect(&behavior_page, SIGNAL(choose_behavior(int)), this, SLOT(choose_behavior(int)));

    // page to show progress and status
    addPage(&progress_page);
    connect(&progress_page, SIGNAL(import_list()), this, SLOT(import_list()));

    connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
}

//
void ListImportWizard::showEvent(QShowEvent *)
{
     button(QWizard::NextButton)->setEnabled(false);
}

void ListImportWizard::save_and_next(Test *test)
{
    if(test){
        dst_test_id = test->get_id();
        next();
    }else{
        // create vocabulary list
        dst_list_page.create_add_list_view();
    }
}

void ListImportWizard::choose_behavior(int behavior)
{
    chosen_behavior = behavior;
}

void ListImportWizard::import_list()
{
    // request to PHP file for the list of all words
    const QUrl url = QUrl(QString("http://neptilo.com/php/clemanglaise/search.php?test_id=%1").arg(test->get_id()));
    nam.get(QNetworkRequest(url));
}

void ListImportWizard::read_reply(QNetworkReply* reply)
{
    QString reply_string = reply->readAll();
    reply->deleteLater();
    QStringList word_keys;
    // has to be consistent with the actual query in the PHP file
    word_keys << "id" << "word" << "meaning" << "nature" << "comment" << "example" << "id_theme" << "pronunciation" << "score" << "theme";
    QStringList reply_list = reply_string.split('\n');
    int nb_words = reply_list.size()/word_keys.size();
    progress_page.set_max_progress(nb_words);
    for(int i = 0; i < nb_words; ++i){
        QHash<QString, QString> word_data;
        for(int j = 0; j < word_keys.size(); ++j){
            word_data[word_keys.at(j)] = reply_list.at(i*word_keys.size()+j);
        }
        progress_page.set_status(tr("Importing \"<b>%1</b>\"").arg(word_data["word"]));
        if(import(word_data))
            ++nb_inserted;
        else
            ++nb_failed;
        progress_page.increase_progress();
    }

    // print recap of import
    QStringList recap_list;
    if(nb_inserted)
        recap_list << tr("inserted <b>%1</b> word(s)").arg(nb_inserted);
    if(nb_replaced)
        recap_list << tr("inserted <b>%1</b> word(s)").arg(nb_replaced);
    if(nb_updated)
        recap_list << tr("inserted <b>%1</b> word(s)").arg(nb_updated);
    if(nb_discarded)
        recap_list << tr("inserted <b>%1</b> word(s)").arg(nb_discarded);
    if(nb_failed)
        recap_list << tr("inserted <b>%1</b> word(s)").arg(nb_failed);
    if(recap_list.isEmpty())
        recap_list << tr("There was nothing to do.");
    QString recap = tr("Import finished: ")+recap_list.join(tr(", "));
    progress_page.set_status(recap);
}
