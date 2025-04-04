#include "TestView.h"

#include <QAction>
#include <QApplication>
#include <QMessageBox>
#ifndef Q_OS_WASM
#include <QSqlDatabase>
#include <QSqlError>
#endif
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStyleFactory>
#include <QTimer>
#include <QToolButton>
#include <QWizard>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

#include "AndroidStyle.h"
#ifndef Q_OS_WASM
#include "import_wizard/DstListPage.h"
#include "import_wizard/DuplicatePage.h"
#include "import_wizard/ListImportWizard.h"
#include "import_wizard/SingleImportWizard.h"
#endif
#include "InterfaceParameters.h"
#include "NetworkReplyReader.h"
#include "resource_utils.h"
#include "AddTagView.h"

TestView::TestView(Test &test,
                   #ifndef Q_OS_WASM
                   DatabaseManager *database_manager,
                   #endif
                   bool admin, QWidget *parent):
    QWidget(parent),
    add_view(nullptr),
    add_tag_view(nullptr),
    admin(admin),
    answer_view(nullptr),
    #ifndef Q_OS_WASM
    database_manager(database_manager),
    #endif
    question_view(nullptr),
    request(nullptr),
    search_view(nullptr),
    status(this),
    test(test, this),
    title(nullptr),
    update_view(nullptr),
    add_button(nullptr),
    search_button(nullptr),
    import_button(nullptr),
    delete_button(nullptr),
    tags_box(nullptr)
{
    // has to be consistent with the actual query in the PHP file
    word_keys << "id" << "word" << "meaning" << "nature" << "comment" << "example" << "pronunciation" << "hint" << "tag_ids";

    create_actions();
    create_interface();
    init();
}

TestView::~TestView(){
    delete request;
}

void TestView::create_actions()
{
    back_action = new QAction(
                getIcon("go-previous"), tr("Go &back to test list"), this);
    back_action->setShortcut(QKeySequence::Back);
    connect(back_action, SIGNAL(triggered()), this, SLOT(go_back()));

    if (!test.is_remote() || admin) {
        add_action = new QAction(getIcon("list-add"), tr("Add a &word"), this);
        add_action->setShortcut(QKeySequence::New);
        connect(add_action, SIGNAL(triggered()), this, SLOT(add_word()));

        add_tag_action = new QAction(getIcon("list-add"), tr("Add a &tag"), this);
        connect(add_tag_action, SIGNAL(triggered()), this, SLOT(add_tag()));
    }

    search_action = new QAction(
                getIcon("edit-find"), tr("&Search for words"), this);
    search_action->setShortcut(QKeySequence::Find);
    connect(search_action, SIGNAL(triggered()), this, SLOT(search()));

    if (test.is_remote()) {
        import_action = new QAction(
                    getIcon("document-save"), tr("&Import this vocabulary list"), this);
        import_action->setShortcut(QKeySequence::Save);
        connect(import_action, SIGNAL(triggered()), this, SLOT(import_list()));
    }

    delete_action = new QAction(
                getIcon("edit-delete"), tr("&Delete this vocabulary list"), this);
    connect(delete_action, SIGNAL(triggered()), this, SLOT(delete_list()));
}

void TestView::create_interface()
{
    layout = new QVBoxLayout(this);

    // header
    header_layout = new QHBoxLayout;
    back_button = new QToolButton(this);
    back_button->setDefaultAction(back_action);
    QString title_str = QString("<b>%1</b> (%2)")
            .arg(test.get_name(), test.is_remote()?tr("online"):tr("offline"));
    title = new QLabel(title_str, this);
    title->setAlignment(Qt::AlignCenter);
    layout->addLayout(header_layout);
    header_layout->addWidget(back_button);
    header_layout->addWidget(title, Qt::AlignCenter);

    tool_bar_layout = new QHBoxLayout;
    if (!test.is_remote() || admin) {
        add_button = new QToolButton(this);
        add_button->setDefaultAction(add_action);
        add_button->addAction(add_tag_action);
        tool_bar_layout->addWidget(add_button);
    }
    search_button = new QToolButton(this);
    search_button->setDefaultAction(search_action);
    tool_bar_layout->addWidget(search_button);
    if (test.is_remote()) {
        import_button = new QToolButton(this);
        import_button->setDefaultAction(import_action);
        tool_bar_layout->addWidget(import_button);
    }
    if (!test.is_remote() || admin) {
        delete_button = new QToolButton(this);
        delete_button->setDefaultAction(delete_action);
        tool_bar_layout->addWidget(delete_button);
    }
    tags_box = new CheckableComboBox(this);
    tags_box->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    tool_bar_layout->addWidget(tags_box);
    layout->addLayout(tool_bar_layout);

    TestView::resizeEvent(nullptr);
}

void TestView::init_button(QToolButton *button)
{
    if (button) {
        button->setFixedSize(InterfaceParameters::widget_unit, InterfaceParameters::widget_unit);
        button->setIconSize(QSize(InterfaceParameters::widget_unit/2, InterfaceParameters::widget_unit/2));
    }
}

// This function is called every time the user comes back from another view.
void TestView::init()
{
    if (test.is_remote()) {
        question_view = new QuestionView(&test, admin, this);
        layout->addWidget(question_view);
        update_request();
        QNetworkReply* reply = NetworkReplyReader::nam->get(*request);
        connect(reply, SIGNAL(finished()), this, SLOT(read_reply()));
#ifndef Q_OS_WASM
    }else{
        // set initial size of a sub-list to test on
        int count;
        QString error;
        if(database_manager->count(test.get_id(), count, true)){
            list_size_limit = std::max(1, count);
            QHash<QString, QString> word_data;
            for (int i = 0; i < 4; i++) // add 4 words at once to the cache at the start
            {
                if(!database_manager->find_lowest(test.get_id(),
                                                  word_data,
                                                  selected_tags,
                                                  list_size_limit,
                                                  word_ids_in_queue.values()))
                    break;

                word_data_queue.push(word_data);
                word_ids_in_queue.insert(word_data["id"].toInt());
            }
            QString sqlite_error(database_manager->pop_last_error());
            if (sqlite_error != "")
                error = tr("<b>SQLite error: </b>")+sqlite_error;
            else if (word_data_queue.empty())
                error = tr("The selected list is currently empty.");
        }else{
            QString sqlite_error(database_manager->pop_last_error());
            error = tr("<b>SQLite error: </b>")+sqlite_error;
        }
        if (error == "") {
            QHash<QString, QString>& word_data = word_data_queue.front();
            QString word = word_data["word"];
            QString hint = word_data["hint"];
            question_view = new QuestionView(&test, admin, this);
            layout->addWidget(question_view);
            question_view->ask_question(word, hint);
        } else {
            status.setText(error);
            layout->addWidget(&status);
            status.show();
        }
#endif
    }
    find_tags();

    if (back_button)
        back_button->show();
    if (add_button)
        add_button->show();
    if (search_button)
        search_button->show();
    if (import_button)
        import_button->show();
    if (delete_button)
        delete_button->show();
    if (tags_box)
        tags_box->show();
}

void TestView::shrink(){
    resize(0,0); //minimumSizeHint());
}

void TestView::update_request() {
    // If 0 is in the list, remove it and set untagged to true to say we also want to look for untagged words, else set it to false.
    QList<int> selected_tags_copy(selected_tags);
    bool untagged = selected_tags_copy.removeOne(0);
    QStringList selected_tags_str;
    for(int i = 0; i < selected_tags_copy.length(); ++i)
        selected_tags_str << QString::number(selected_tags_copy.at(i));
    // Request to PHP or local file
    QUrl url;
    url = QUrl(QString(NetworkReplyReader::api_url + "find_random.php"
                       "?list_id=%1&tag_ids=%2&untagged=%3")
               .arg(test.get_id())
               .arg(selected_tags_str.join(","))
               .arg(untagged));
    delete request; // It cannot be deleted before because it still has to be available when a new question is loaded. (The request remains the same.)
    request = new QNetworkRequest(url);
}

void TestView::read_reply(){
    auto reply = qobject_cast<QNetworkReply*>(sender());
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(status_code.toInt() != 200){
        status.setText(reply->readAll().replace('\0', ""));
        layout->addWidget(&status);
        status.show();
        return;
    }
    status.hide();
    if(question_view){ // If question_frame is deleted, that means the user has changed the view to another one before the NAM request was finished, so we want to ignore the NAM's reply.
        // Store the lines of the reply in the "reply_list" attribute
        QString reply_string(reply->readAll().replace('\0', ""));
        reply->deleteLater();
        if(reply_string.isEmpty())
            question_view->show_error(tr("The selected list is currently empty."));
        else{
            QStringList word_values = reply_string.split('\n');
            QHash<QString, QString> word_data;
            for(int i = 0; i < word_keys.size(); ++i)
                word_data[word_keys.at(i)] = word_values.at(i);
            word_data_queue.push(word_data);

            // Everything is ready for the question view to ask the question.
            question_view->ask_question(word_data["word"], word_data["hint"]);
        }
    }
}

void TestView::read_delete_list_reply()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    QString reply_string(reply->readAll().replace('\0', ""));
    reply->deleteLater();
    if(reply_string.isEmpty())
        delete this;
    else{
        status.setText(reply->readAll().replace('\0', ""));
        layout->addWidget(&status);
        status.show();
    }
}

void TestView::validate_question(){

    // Create a new answer frame
    delete answer_view;
    answer_view = new AnswerView(word_data_queue.front(),
                                 question_view->get_answer(),
                                 &test,
                             #ifndef Q_OS_WASM
                                 database_manager,
                             #endif
                                 this);
    layout->addWidget(answer_view);
}

void TestView::validate_answer() {
    // Remove everything
    delete question_view;
    question_view = nullptr;
    if(answer_view){
        // update list size depending on whether the last answer was correct
        float size_increment = static_cast<float>(list_size_limit)/++asked_in_this_session;
        list_size_limit = answer_view->get_correct() ?
                              list_size_limit+std::max(1.f, size_increment) :
                              std::max(1.f, list_size_limit-2.f/3*size_increment);

        answer_view->hide();
    }

    // Create a new question frame
    question_view = new QuestionView(&test, admin, this); // Is it deleted somewhere? It should because of "new".
    layout->addWidget(question_view);

    // Request for a new question
#ifndef Q_OS_WASM
    if (!test.is_remote()) {
        int count;
        database_manager->count(test.get_id(), count, true);
        word_ids_in_queue.remove(word_data_queue.front()["id"].toInt());
        word_data_queue.pop();
        QHash<QString, QString> word_data;
        QString error;
        if(database_manager->find_lowest(test.get_id(),
                                         word_data,
                                         selected_tags,
                                         std::min(count+1, list_size_limit),
                                         word_ids_in_queue.values())){
            word_data_queue.push(word_data);
            word_ids_in_queue.insert(word_data["id"].toInt());
        }
        QString sqlite_error(database_manager->pop_last_error());
        if (sqlite_error != "")
            error = tr("<b>SQLite error: </b>")+sqlite_error;
        else if (word_data_queue.empty())
            error = tr("There are no more words to ask.");
        if (error == "") {
            QHash<QString, QString>& word_data = word_data_queue.front();
            QString word = word_data["word"];
            QString hint = word_data["hint"];
            question_view->ask_question(word, hint);
        } else {
            status.setText(error);
            layout->addWidget(&status);
            status.show();
        }
    } 
    else
#endif
    {
        word_data_queue.pop();
        QNetworkReply* reply = NetworkReplyReader::nam->get(*request);
        connect(reply, SIGNAL(finished()), this, SLOT(read_reply()));
    }
}

void TestView::update_question(){
    if (test.is_remote())
        update_request();
    validate_answer();
}

void TestView::update_question(int){
    update_question();
}

void TestView::delete_list()
{
    QMessageBox::StandardButton ret = QMessageBox::question(
        this,
        tr("Confirm deletion"),
        tr("Are you sure you want to delete the vocabulary list \"<b>%1</b>\"?")
                .arg(test.get_name()));
    if(ret == QMessageBox::Yes){
        if(test.is_remote()){
            // request to PHP file
            QUrlQuery post_data;
            post_data.addQueryItem("list_id", QString::number(test.get_id()));
            const QUrl url(NetworkReplyReader::api_url + "delete_list.php");
            QNetworkRequest request(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader,
                              "application/x-www-form-urlencoded");

            // Send the request
            QNetworkReply* reply = NetworkReplyReader::nam->post(request, post_data.query().toUtf8());
            connect(reply, SIGNAL(finished()),
                    this, SLOT(read_delete_list_reply()));
#ifndef Q_OS_WASM
        }else{
            // offline
            if (database_manager->delete_list(test.get_id()))
                delete this;
            else {
                QString error(database_manager->pop_last_error());
                if(error == "")
                    status.setText(tr("Deletion failed."));
                else
                    status.setText(tr("<b>SQLite error: </b>")+error);
                layout->addWidget(&status);
                status.show();
            }
#endif
        }
    }
}

void TestView::delete_word()
{
    qDebug() << tr("Not implemented yet"); // TODO
}

void TestView::update_selected_tags(QModelIndex top_left, QModelIndex)
{
    QMap<int, QVariant> item_data = tags_box->model()->itemData(top_left);
    int map_size = item_data.size();
    int variant = item_data[Qt::UserRole].toInt();
    switch (item_data[Qt::CheckStateRole].toInt()) {
    case Qt::Checked:
        if (map_size > 2)
            selected_tags << variant ;
        break;
    case Qt::Unchecked:
        if (map_size > 2)
            selected_tags.removeOne(variant);
        break;
    default:
        qDebug() << tr("Wrong check state value");
        break;
    }
    update_question();
}

void TestView::add_tag()
{
    remove_widgets();

    // Create a new add frame
    QStringList default_values_list;
    default_values_list << "" << "";
    add_tag_view = new AddTagView(&test,
                                  tr("<b>Add a new tag</b>"),
                                  default_values_list,
                                  tr("Add"),
                                  "add_tag",
                                  tr("Tag successfully added!"),
                              #ifndef Q_OS_WASM
                                  database_manager,
                              #endif
                                  this);
    layout->addWidget(add_tag_view);
}

void TestView::add_word()
{
    remove_widgets();

    // Create a new add frame
    QHash<QString, QString> default_values;
    for(int i = 0; i < word_keys.size(); ++i)
        default_values[word_keys.at(i)] = "";
    add_view = new EditView(&test,
                            tr("<b>Add a new word</b>"),
                            default_values,
                            tr("Add"),
                            "add_word",
                            tr("Word successfully added!"),
                        #ifndef Q_OS_WASM
                            database_manager,
                        #endif
                            this);
    layout->addWidget(add_view);
}

void TestView::update_word()
{
    remove_widgets();

    // Create a new add frame
    update_view = new EditView(&test,
                               tr("<b>Edit a word entry</b>"),
                               word_data_queue.front(),
                               tr("Edit"),
                               "update_word",
                               tr("Word successfully edited!"),
                           #ifndef Q_OS_WASM
                               database_manager,
                           #endif
                               this);
    layout->addWidget(update_view);
}

void TestView::search()
{
    remove_widgets();

    // Create a new search frame
    search_view = new SearchView(&test,
                             #ifndef Q_OS_WASM
                                 database_manager,
                             #endif
                                 !test.is_remote()||admin, this);
    layout->addWidget(search_view);
}

void TestView::go_back() {
    if (search_view) {
        if (!search_view->go_back()) {
            delete search_view;
            search_view = nullptr;
            init();
        } // Else search_view already handled the go back action.
    } else if (
            add_view ||
            add_tag_view ||
            update_view) {
        delete add_view;
        delete add_tag_view;
        delete update_view;
        add_view = nullptr;
        add_tag_view = nullptr;
        update_view = nullptr;
        init();
    } else
        delete this;
}

void TestView::find_tags() {
    if (test.is_remote()) {
        // Request to PHP file
        const QUrl url = QUrl(QString(NetworkReplyReader::api_url + "find_used_tags.php?list_id=%1").arg(test.get_id()));
        QNetworkRequest request(url);
        QNetworkReply* reply = NetworkReplyReader::nam->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(read_reply_tags()));
#ifndef Q_OS_WASM
    } else {
        // Offline
        database_manager->find_used_tags(test.get_id(), tag_reply_list);
        read_reply("");
#endif
    }
}

void TestView::read_reply_tags()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());

    // Store the lines of the reply in the "reply_list" attribute
    QString reply_string = reply->readAll().replace('\0', "");
    reply->deleteLater();
    read_reply(reply_string);
}

void TestView::read_reply(QString reply_string) {
    if (test.is_remote())
        tag_reply_list = reply_string.split('\n', Qt::SkipEmptyParts);

    // fill the tag combo box
    tags_box->disconnect();
    tags_box->clear();
    int l = tag_reply_list.size()/2;
    QStandardItemModel *model = new QStandardItemModel(l+1, 1);
    tags_box->set_title(tr("Filter by tags"));
    auto item = new QStandardItem(tr("Without any tags"));
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(Qt::Unchecked, Qt::CheckStateRole);
    model->setItem(0, 0, item);
    for (int i = 0; i < l; ++i) {
        item = new QStandardItem(tag_reply_list.at(2*i+1).trimmed());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        item->setData(QVariant(tag_reply_list.at(2*i).toInt()), Qt::UserRole);
        model->setItem(i+1, 0, item);
    }
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(update_selected_tags(QModelIndex,QModelIndex)));
    tags_box->setModel(model);
}

void TestView::remove_widgets()
{
    delete question_view;
    question_view = nullptr;
    delete answer_view;
    answer_view = nullptr;
    if (add_button)
        add_button->hide();
    if (search_button)
        search_button->hide();
    if (import_button)
        import_button->hide();
    if (delete_button)
        delete_button->hide();
    if (tags_box)
        tags_box->hide();
    status.hide();
}

#ifndef Q_OS_WASM
void TestView::import_word()
{
    SingleImportWizard import_wizard(database_manager, word_data_queue.front(), nullptr, this);
    if(import_wizard.exec())
        // Show confirmation
        status.setText(tr("Import succeeded!"));
    else
        status.setText(import_wizard.get_error());
    layout->addWidget(&status);
    status.show();
}

void TestView::import_list()
{
    ListImportWizard import_wizard(database_manager, &test, this);
    if(import_wizard.exec())
        // Show confirmation
        status.setText(tr("Import succeeded!"));
    else
        status.setText(import_wizard.get_error());
    layout->addWidget(&status);
    status.show();
}
#endif

void TestView::resizeEvent(QResizeEvent *)
{
    init_button(back_button);
    init_button(add_button);
    init_button(search_button);
    init_button(import_button);
    init_button(delete_button);
    if (title)
    {
        title->setFixedHeight(InterfaceParameters::widget_unit);
        QFont font = title->font();
        font.setPointSize(0.5 * InterfaceParameters::widget_unit);
        title->setFont(font);
    }
    if (tags_box)
        tags_box->setFixedHeight(InterfaceParameters::widget_unit);
}
