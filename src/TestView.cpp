#include "TestView.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTimer>
#include <QWizard>

#include "import_wizard/DstListPage.h"
#include "import_wizard/DuplicatePage.h"
#include "import_wizard/ListImportWizard.h"
#include "import_wizard/SingleImportWizard.h"
#include "NetworkReplyReader.h"
#include "string_utils.h"
#include "ThemeView.h"

TestView::TestView(Test &test, DatabaseManager *database_manager, QString str_title, bool admin, QWidget *parent):
    QWidget(parent),
    add_button(NULL),
    add_view(NULL),
    add_theme_button(NULL),
    add_theme_frame(NULL),
    admin(admin),
    answer_view(NULL),
    back_button(NULL),
    database_manager(database_manager),
    delete_list_button(NULL),
    layout(NULL),
    nam(NULL),
    nam_themes(),
    question_view(NULL),
    request(NULL),
    search_button(NULL),
    import_button(NULL),
    search_view(NULL),
    status(this),
    test(test, this),
    theme(NULL),
    themes(NULL),
    title(NULL),
    update_button(NULL),
    update_view(NULL),
    update_theme_view(NULL)
{
    title = new QLabel(str_title, this);
    title->setAlignment(Qt::AlignHCenter);
    layout = new QVBoxLayout(this);
    answer_view = new AnswerView(&test, this);

    layout->addWidget(title);

    theme = new QLabel(tr("<i>Filter by tags</i>"), this);
    layout->addWidget(theme);
    themes = new QComboBox(this);
    // set the ComboBox to that width.
    tags = new QListWidget(this);
    tags->setSelectionMode(QAbstractItemView::ExtendedSelection);
    layout->addWidget(tags);
    themes->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    layout->addWidget(themes);

    back_button = new QPushButton(tr("Go &back to tests list"), this);
    back_button->setIcon(QIcon::fromTheme("go-home", QIcon(getImgPath("go-home.png"))));
    connect(back_button, SIGNAL(clicked()), this, SLOT(go_back()));
    layout->addWidget(back_button);

    if(!test.is_remote() || admin){
        add_theme_button = new QPushButton(tr("Add a &theme"), this);
        add_theme_button->setIcon(QIcon::fromTheme("list-add",QIcon(getImgPath("list-add.png"))));
        connect(add_theme_button, SIGNAL(clicked()), this, SLOT(add_theme()));
        layout->addWidget(add_theme_button);

        add_button = new QPushButton(tr("Add a &word"), this);
        add_button->setIcon(QIcon::fromTheme("list-add",QIcon(getImgPath("list-add.png"))));
        connect(add_button, SIGNAL(clicked()), this, SLOT(add_word()));
        layout->addWidget(add_button);

        update_button = new QPushButton(tr("&Edit this word entry"), this);
        update_button->setIcon(QIcon::fromTheme("accessories-text-editor", QIcon(getImgPath("accessories-text-editor.png"))));
        connect(update_button, SIGNAL(clicked()), this, SLOT(update_word()));
        layout->addWidget(update_button);

        delete_list_button = new QPushButton(tr("&Delete this vocabulary list"), this);
        delete_list_button->setIcon(QIcon::fromTheme("process-stop", QIcon(getImgPath("process-stop.png"))));
        connect(delete_list_button, SIGNAL(clicked()), this, SLOT(delete_list()));
        layout->addWidget(delete_list_button);
    }

    search_button = new QPushButton(tr("&Search for words"), this);
    search_button->setIcon(QIcon::fromTheme("edit-find", QIcon(getImgPath("edit-find.png"))));
    connect(search_button, SIGNAL(clicked()), this, SLOT(search()));
    layout->addWidget(search_button);

    if(test.is_remote()){
        import_button = new QPushButton(tr("&Import this vocabulary list"));
        import_button->setIcon(QIcon::fromTheme("document-save", QIcon(getImgPath("document-save.png"))));
        connect(import_button, SIGNAL(clicked()), this, SLOT(import_list()));
        layout->addWidget(import_button);
    }

    init();
    connect(&nam_themes, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply_themes(QNetworkReply*)));
}

TestView::~TestView(){
    delete request;
}

// This function is called every time the user comes back from another view.
void TestView::init()
{
    //layout->setSizeConstraint(QLayout::SetFixedSize);
    //adjustSize();
    //QTimer::singleShot(0,this, SLOT(shrink()));
    if (test.is_remote()) {
        question_view = new QuestionView(&test, this);
        layout->addWidget(question_view);
        update_request();
        nam = new QNetworkAccessManager(this);
        connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
        nam->get(*request);
    }else{
        QList<QListWidgetItem *> selected_items  = tags->selectedItems();
        QList<int> selected_tags;
        for (int i = 0, l = selected_items.size(); i<l; ++i)
            selected_tags << selected_items.at(i)->data(Qt::UserRole).toInt();

        if(database_manager->find_lowest(test.get_id(), word_data, selected_tags)){
            QString word = word_data["word"];
            QString theme = word_data["theme"];
            question_view = new QuestionView(&test, this);
            layout->addWidget(question_view);
            question_view->ask_question(word, theme);
        }else{
            QString error(database_manager->pop_last_error());
            if(error == "")
                status.setText(tr("The selected list is currently empty."));
            else
                status.setText(tr("<b>SQLite error: </b>")+error);
            layout->addWidget(&status);
            status.show();
        }
    }
    find_tags();

    // Show everything
    if(!test.is_remote() || admin){
        update_button->show();
        add_theme_button->show();
        add_button->show();
        delete_list_button->show();
    }
    theme->show();
    themes->show();
    tags->show();
    back_button->show();
    search_button->show();
    if(test.is_remote())
        import_button->show();
}

void TestView::shrink(){
    resize(0,0); //minimumSizeHint());
}

void TestView::update_request() {
	int index = themes->currentIndex();
	// Request to PHP or local file
	QUrl url;
    url = QUrl(QString("http://neptilo.com/php/clemanglaise/find_lowest.php?test_id=%1&id_theme=%2").arg(test.get_id()).arg(themes->itemData(index).toString()));
	delete request; // It cannot be deleted before because it still has to be available when a new question is loaded. (The request stays the same.)
	request = new QNetworkRequest(url);
}

void TestView::read_reply(QNetworkReply* reply){
    if(question_view){ // If question_frame is deleted, that means the user has changed the view to another one before the NAM request was finished, so we want to ignore the NAM's reply.
        // Store the lines of the reply in the "reply_list" attribute
        QString reply_string(reply->readAll());
        reply->deleteLater();
        if(reply_string.isEmpty())
            question_view->show_error(tr("The selected list is currently empty."));
        else{
            QStringList word_keys;
            // has to be consistent with the actual query in the PHP file
            word_keys << "id" << "word" << "meaning" << "nature" << "comment" << "example" << "id_theme" << "pronunciation" << "score" << "name";
            QStringList word_values = reply_string.split('\n');
            for(int i = 0; i < word_keys.size(); ++i)
                word_data[word_keys.at(i)] = word_values.at(i);

            // Everything is ready for the question view to ask the question.
            question_view->ask_question(word_data["word"], word_data["theme"]);
        }
    }
}

void TestView::read_delete_list_reply(QNetworkReply *reply)
{
    QString reply_string(reply->readAll());
    reply->deleteLater();
    if(reply_string.isEmpty())
        delete this;
    else{
        status.setText(reply->readAll());
        layout->addWidget(&status);
        status.show();
    }
}

void TestView::validate_question(){

    // Create a new answer frame
    delete answer_view;
    answer_view = new AnswerView(word_data, question_view->get_answer(), &test, database_manager, this);
    layout->addWidget(answer_view);
}

void TestView::validate_answer() {
    QList<QListWidgetItem *> selected_items  = tags->selectedItems();
    QList<int> selected_tags;
    for (int i = 0, l = selected_items.size(); i<l; ++i)
       selected_tags << selected_items.at(i)->data(Qt::UserRole).toInt();

    // Remove everything
    delete question_view;
    question_view = NULL;
    if(answer_view)
        answer_view->hide();

    // Create a new question frame
    question_view = new QuestionView(&test, this); // Is it deleted somewhere? It should because of "new".
    layout->addWidget(question_view);

    // Request for a new question
    if (!test.is_remote()) {
        if(database_manager->find_lowest(test.get_id(), word_data, selected_tags)){
            QString word = word_data["word"];
            QString theme = word_data["theme"];
            question_view->ask_question(word, theme);
        }else{
            QString error(database_manager->pop_last_error());
            if(error == "")
                status.setText(tr("The selected list is currently empty."));
            else
                status.setText(tr("<b>SQLite error: </b>")+error);
            layout->addWidget(&status);
            status.show();
        }
    } 
	else 
		nam->get(*request);
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
                tr("Are you sure you want to delete the vocabulary list \"<b>%1</b>\"?").arg(test.get_name())
            #if QT_VERSION < QT_VERSION_CHECK(5,0,0)
                , QMessageBox::Yes | QMessageBox::No
            #endif
                );
    if(ret == QMessageBox::Yes){
        if(test.is_remote()){
            // request to PHP file
            nam->disconnect();
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
            QUrl post_data;
#else
            QUrlQuery post_data;
#endif
            post_data.addQueryItem("test_id", QString::number(test.get_id()));
            const QUrl url("http://neptilo.com/php/clemanglaise/delete_list.php");
            QNetworkRequest request(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            nam->setCookieJar(NetworkReplyReader::cookie_jar); // By default, nam takes ownership of the cookie jar.
            nam->cookieJar()->setParent(0); // Unset the cookie jar's parent so it is not deleted when nam is deleted, and can still be used by other NAMs.

            // Send the request
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
            nam->post(request, post_data.encodedQuery());
#else
            connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_delete_list_reply(QNetworkReply*)));
            nam->post(request, post_data.query(QUrl::FullyEncoded).toUtf8());
#endif
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
        }
    }
}

void TestView::add_theme()
{
	remove_widgets();

	// Create a new add frame
	QStringList default_values_list;
	default_values_list << "" << "";
    add_theme_frame = new ThemeView(&test, tr("<b>Add a new theme</b>"), default_values_list, tr("Add"), "add_theme", tr("Theme successfully added!"), database_manager, this);
	layout->addWidget(add_theme_frame);
	connect(add_theme_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestView::add_word()
{
	remove_widgets();

	// Create a new add frame
    QStringList word_keys;
    // has to be consistent with the actual content of reply_list
    word_keys << "id" << "word" << "meaning" << "nature" << "comment" << "example" << "id_theme" << "pronunciation" << "score" << "theme";
    QHash<QString, QString> default_values;
    for(int i = 0; i < word_keys.size(); ++i)
        default_values[word_keys.at(i)] = "";
    add_view = new EditView(&test, tr("<b>Add a new word</b>"), default_values, tr("Add"), "add", tr("Word successfully added!"), database_manager, this);
    layout->addWidget(add_view);
    connect(add_view, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestView::update_word()
{
	remove_widgets();

	// Create a new add frame
    update_view = new EditView(&test, tr("<b>Edit a word entry</b>"), word_data, tr("Edit"), "update", tr("Word successfully edited!"), database_manager, this);
    layout->addWidget(update_view);
    connect(update_view, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestView::search()
{
	remove_widgets();

	// Create a new search frame
    search_view = new SearchView(&test, database_manager, !test.is_remote()||admin, this);
    layout->addWidget(search_view);
    connect(search_view, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestView::go_back() {
	delete this;
}

void TestView::find_tags() {
    if (!test.is_remote()) {
		// Offline
        database_manager->find_used_tags(test.get_id(), reply_list_theme);
		read_reply();
	} else { 
		// Request to PHP file
        const QUrl url = QUrl(QString("http://neptilo.com/php/clemanglaise/find_used_themes.php?test_id=%1").arg(test.get_id()));
		QNetworkRequest request(url);
		nam_themes.get(request);
	}
}

void TestView::read_reply_themes(QNetworkReply* reply)
{
	// Store the lines of the reply in the "reply_list" attribute
	QString reply_string = reply->readAll();
	reply->deleteLater();
	read_reply(reply_string);
}

void TestView::read_reply(QString reply_string) {
    if (test.is_remote())
		reply_list_theme = reply_string.split('\n', QString::SkipEmptyParts);
	themes->disconnect();
	themes->clear();
	themes->addItem("---");
    //tags
	tags->disconnect();
    tags->clear();
	tags->addItem(tr("Without any tags"));
	for(int i=0, l = reply_list_theme.count(); i<l-1; i+=2) {
        QListWidgetItem* item = new QListWidgetItem(reply_list_theme.at(i+1).trimmed());
        item->setData(Qt::UserRole, QVariant(reply_list_theme.at(i).toInt()));
        tags->addItem(item);
	}
    connect(tags, SIGNAL(itemSelectionChanged()), this, SLOT(update_question()));
	for(int i=0, l = reply_list_theme.count(); i<l-1; i+=2) {
		themes->addItem(reply_list_theme.at(i+1), QVariant(reply_list_theme.at(i).toInt()));
	}    
	connect(themes, SIGNAL(currentIndexChanged(int)), this, SLOT(update_question(int)));
}

void TestView::remove_widgets()
{
    delete question_view;
    question_view = NULL;
    delete answer_view;
    answer_view = NULL;
    if(!test.is_remote() || admin){
		update_button->hide();
		add_theme_button->hide();
		add_button->hide();
        delete_list_button->hide();
	}
	theme->hide();
	themes->hide();
	tags->hide();
	back_button->hide();
	search_button->hide();
    if(test.is_remote())
        import_button->hide();
    status.hide();
}


void TestView::import_word()
{
    SingleImportWizard import_wizard(database_manager, word_data, NULL, this);
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
