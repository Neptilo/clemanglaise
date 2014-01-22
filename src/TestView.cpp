#include "TestView.h"

#include <QSqlDatabase>
#include <QSqlError>

#include "NetworkReplyReader.h"
#include "string_utils.h"
#include "ThemeView.h"

TestView::TestView(Test &test, DatabaseManager *database_manager, QString str_title, bool admin, QWidget *parent):
    QWidget(parent),
    add_button(NULL),
    add_frame(NULL),
    add_theme_button(NULL),
    add_theme_frame(NULL),
    admin(admin),
    answer_frame(NULL),
    back_button(NULL),
    database_manager(database_manager),
    layout(NULL),
    nam(NULL),
    nam_themes(),
    question_frame(NULL),
    reply_list(),
    reply_list_theme(),
    request(NULL),
    search_button(NULL),
    search_frame(NULL),
    status(this),
    test(test),
    theme(NULL),
    themes(NULL),
    title(NULL),
    update_button(NULL),
    update_frame(NULL),
    update_theme_frame(NULL)
{
    title = new QLabel(str_title, this);
    title->setAlignment(Qt::AlignHCenter);
    layout = new QVBoxLayout(this);
    answer_frame = new AnswerView(test, this);

    layout->addWidget(title);

    theme = new QLabel(tr("<i>Choose a theme</i>"), this);
    layout->addWidget(theme);
    themes = new QComboBox(this);
    // set the ComboBox to that width.
    themes->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    layout->addWidget(themes);

    back_button = new QPushButton(tr("Go &back to tests list"), this);
    back_button->setIcon(QIcon::fromTheme("go-home", QIcon(getImgPath("go-home.png"))));
    connect(back_button, SIGNAL(clicked()), this, SLOT(go_back()));
    layout->addWidget(back_button);

    if(!test.is_remote_work() || admin){
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
    }

    search_button = new QPushButton(tr("&Search for words"), this);
    search_button->setIcon(QIcon::fromTheme("edit-find", QIcon(getImgPath("edit-find.png"))));
    connect(search_button, SIGNAL(clicked()), this, SLOT(search()));
    layout->addWidget(search_button);

    init();
    connect(&nam_themes, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply_themes(QNetworkReply*)));
}

TestView::~TestView(){
    delete request;
}

// This function is called every time the user comes back from another view.
void TestView::init()
{
    if (test.is_remote_work()) {
        question_frame = new QuestionView(test, this);
        layout->addWidget(question_frame);
        update_request();
        nam = new QNetworkAccessManager(this);
        connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
        nam->get(*request);
    }else{
        int index = themes->currentIndex();
        QString lang = test.get_src() + test.get_dst();
        if(database_manager->find_lowest(lang, reply_list, themes->itemData(index).toInt())){
            QString word = reply_list.at(1);
            QString theme = reply_list.at(9);
            question_frame = new QuestionView(test, this);
            layout->addWidget(question_frame);
            question_frame->ask_question(word, theme);
        }else{
            if(database_manager->get_last_error() == " ")
                status.setText(tr("The selected list is currently empty."));
            else
                status.setText(tr("<b>SQLite error: </b>")+database_manager->get_last_error());
            layout->addWidget(&status);
        }
    }
    find_themes();

    // Show everything
    if(!test.is_remote_work() || admin){
        update_button->show();
        add_theme_button->show();
        add_button->show();
    }
    theme->show();
    themes->show();
    back_button->show();
    search_button->show();
    status.show();
}

void TestView::update_request() {
	int index = themes->currentIndex();
	QString lang = test.get_src() + test.get_dst();
	// Request to PHP or local file
	QUrl url;
	url = QUrl("http://neptilo.com/php/clemanglaise/find_lowest.php?lang=" + lang +"&id_theme="+themes->itemData(index).toString());
	delete request; // It cannot be deleted before because it still has to be available when a new question is loaded. (The request stays the same.)
	request = new QNetworkRequest(url);
}

void TestView::read_reply(QNetworkReply* reply){
    if(question_frame){ // If question_frame is deleted, that means the user has changed the frame to another one before the NAM request was finished, so we want to ignore the NAM's reply.
        // Store the lines of the reply in the "reply_list" attribute
        QString reply_string(reply->readAll());
        reply->deleteLater();
        reply_list = reply_string.split('\n');

        // Everything is ready for the question frame to ask the question.
        QString word = reply_list.at(1);
        QString theme = reply_list.at(9);
        question_frame->ask_question(word, theme);
    }
}

void TestView::validate_question(){

    // Create a new answer frame
    delete answer_frame;
    answer_frame = new AnswerView(reply_list, question_frame->getAnswer(), test, database_manager, this);
    layout->addWidget(answer_frame);
}

void TestView::validate_answer() {
	int index = themes->currentIndex();

    // Remove everything
    delete question_frame;
    question_frame = NULL;
    if(answer_frame)
        answer_frame->hide();

    // Create a new question frame
    question_frame = new QuestionView(test, this); // Is it deleted somewhere? It should because of "new".
    layout->addWidget(question_frame);

    // Request for a new question
    if (!test.is_remote_work()) {
		QString lang = test.get_src() + test.get_dst();
		database_manager->find_lowest(lang, reply_list, themes->itemData(index).toInt());
    } 
	else 
		nam->get(*request);
}

void TestView::update_question(int){
    update_request();
    validate_answer();
}

void TestView::add_theme()
{
    remove_widgets();

    // Create a new add frame
    QStringList default_values_list;
    default_values_list << "" << "";
    add_theme_frame = new ThemeView(test, tr("<b>Add a new theme</b>"), default_values_list, tr("Add"), "add_theme", tr("Theme successfully added!"), database_manager, this);
    layout->addWidget(add_theme_frame);
    connect(add_theme_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestView::add_word()
{
    remove_widgets();

    // Create a new add frame
    QStringList default_values_list;
	//word << meaning << nature << comment << exple << id_theme << pronunciation << score<< theme

    default_values_list << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
    add_frame = new EditView(test, tr("<b>Add a new word</b>"), default_values_list, tr("Add"), "add", tr("Word successfully added!"), database_manager, this);
    layout->addWidget(add_frame);
    connect(add_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestView::update_word()
{
    remove_widgets();

    // Create a new add frame
    update_frame = new EditView(test, tr("<b>Edit a word entry</b>"), reply_list, tr("Edit"), "update", tr("Word successfully edited!"), database_manager, this);
    layout->addWidget(update_frame);
    connect(update_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestView::search()
{
    remove_widgets();

    // Create a new search frame
    search_frame = new SearchView(test, database_manager, !test.is_remote_work()||admin, this);
    layout->addWidget(search_frame);
    connect(search_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestView::go_back() {
	delete this;
}

void TestView::find_themes() {
    if (!test.is_remote_work()) {
        // Offline
		database_manager->find_used_themes(test.get_src()+test.get_dst(), reply_list_theme);
		read_reply();
	} else { 
		// Request to PHP file
        const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/find_used_themes.php?lang=" + test.get_src() + test.get_dst());
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
	if (test.is_remote_work())
		reply_list_theme = reply_string.split('\n', QString::SkipEmptyParts);
    themes->disconnect();
    themes->clear();
    themes->addItem("---");
	for(int i=0, l = reply_list_theme.count(); i<l-1; i+=2) {
		themes->addItem(reply_list_theme.at(i+1), QVariant(reply_list_theme.at(i).toInt()));
    }    
    connect(themes, SIGNAL(currentIndexChanged(int)), this, SLOT(update_question(int)));
}

void TestView::remove_widgets()
{
    delete question_frame;
    question_frame = NULL;
    delete answer_frame;
    answer_frame = NULL;
    if(!test.is_remote_work() || admin){
        update_button->hide();
        add_theme_button->hide();
        add_button->hide();
    }
    theme->hide();
    themes->hide();
    back_button->hide();
    search_button->hide();
    status.hide();
}
