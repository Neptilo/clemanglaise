#include "testframe.h"
#include "themeframe.h"

TestFrame::TestFrame(Test &test, QString str_title, QWidget *parent):
    QWidget(parent),
	nam_themes(),
    test(test)
{
	title = new QLabel(str_title,this);
    title->setAlignment(Qt::AlignHCenter);
    layout = new QVBoxLayout(this);
    answer_frame = new AnswerFrame(test, this);
    init();
	find_themes(); 
    connect(&nam_themes, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply_themes(QNetworkReply*)));
}

TestFrame::~TestFrame(){
    delete request;
    delete nam;
    delete reply_list;
}

void TestFrame::init(){

	layout->addWidget(title); 

	theme = new QLabel(tr("Choose a theme"), this);
	layout->addWidget(theme);
	themes = new QComboBox(this);
	layout->addWidget(themes);

    back_button = new QPushButton(tr("Go back to tests list"), this);
    connect(back_button, SIGNAL(clicked()), this, SLOT(go_back()));
    layout->addWidget(back_button);


    add_theme_button = new QPushButton(tr("Add a theme"), this);
    connect(add_theme_button, SIGNAL(clicked()), this, SLOT(add_theme()));
    layout->addWidget(add_theme_button);

    add_button = new QPushButton(tr("Add word"), this);
    connect(add_button, SIGNAL(clicked()), this, SLOT(add_word()));
    layout->addWidget(add_button);

    search_button = new QPushButton(tr("Search for words"), this);
    connect(search_button, SIGNAL(clicked()), this, SLOT(search()));
    layout->addWidget(search_button);

    update_button = new QPushButton(tr("Edit this word entry"), this);
    connect(update_button, SIGNAL(clicked()), this, SLOT(update_word()));
    layout->addWidget(update_button);

    question_frame = new QuestionFrame(test, this);
    layout->addWidget(question_frame);

    // Request to PHP file
	QUrl url;
	if (test.isRemoteWork()) {
        url = QUrl("http://neptilo.com/php/clemanglaise/find_lowest.php?lang=" + test.getSrc() + test.getDst());
	} else {
		parser = new Parser(test.getSrc() + test.getDst());
		parser->parse();
		url = QUrl(Parser::get_working_path(parser->getFileout()));
	}
    request = new QNetworkRequest(url);
    nam = new QNetworkAccessManager;

    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));

    nam->get(*request);
}

void TestFrame::read_reply(QNetworkReply* reply){

    // Store the lines of the reply in the "reply_list" attribute
    QString* reply_string = new QString(reply->readAll());
    reply->deleteLater();
    reply_list = test.isRemoteWork()?new QStringList(reply_string->split('\n')):new QStringList(reply_string->split(QRegExp(ENDL)));

    // Everything is ready for the question frame to ask the question.
    QString word = reply_list->at(1);
    QString theme = reply_list->at(6);
	if (test.isRemoteWork()) {
		question_frame->ask_question(word, theme);
	} else {
		question_frame->ask_question(word, Parser::getTheme(theme.toInt()));
	}
}

void TestFrame::validate_question(){

    // Create a new answer frame
    delete answer_frame;
    answer_frame = new AnswerFrame(*reply_list, question_frame->getAnswer(), test, this);
    layout->addWidget(answer_frame);
}

void TestFrame::validate_answer() {

    // Remove everything
    delete question_frame;
    answer_frame->hide();

    // Create a new question frame
    question_frame = new QuestionFrame(test, this); // Is it deleted somewhere? It should because of "new".
    layout->addWidget(question_frame);

    // Request for a new question
	if (!test.isRemoteWork()) {
		parser->parse();
	}
    nam->get(*request);
}


void TestFrame::add_theme() {
    // Remove everything
    delete question_frame;
    answer_frame->hide();
	back_button->disconnect();
	back_button->hide();
    add_theme_button->disconnect();
    add_theme_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    add_button->disconnect();
    add_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    update_button->disconnect();
    update_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    search_button->disconnect();
    search_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.

    // Create a new add frame
    QStringList default_values_list;
    default_values_list << "" << "";
    add_theme_frame = new ThemeFrame(test, tr("<b>Add a new theme</b>"), default_values_list, tr("Add"), "add_theme", tr("Theme successfully added!"), this);
    layout->addWidget(add_theme_frame);
    connect(add_theme_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::add_word(){

    // Remove everything
    delete question_frame;
    answer_frame->hide();
	back_button->disconnect();
	back_button->hide();
    add_theme_button->disconnect();
    add_theme_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    add_button->disconnect();
    add_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    update_button->disconnect();
    update_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    search_button->disconnect();
    search_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.

    // Create a new add frame
    QStringList default_values_list;
	//word << meaning << nature << comment << exple << theme << pronunciation <<score 
    default_values_list << "" << "" << "" << "" << "" << "" << "" << "";
    add_frame = new EditFrame(test, tr("<b>Add a new word</b>"), default_values_list, tr("Add"), "add", tr("Word successfully added!"), this);
    layout->addWidget(add_frame);
    connect(add_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::update_word(){

    // Remove everything
    delete question_frame;
    answer_frame->hide();
	back_button->disconnect();
	back_button->hide();
    add_theme_button->disconnect();
    add_theme_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    add_button->disconnect();
    add_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    update_button->disconnect();
    update_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    search_button->disconnect();
    search_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.

    // Create a new add frame
    update_frame = new EditFrame(test, tr("<b>Edit a word entry</b>"), *reply_list, tr("Edit"), "update", tr("Word successfully edited!"), this);
    layout->addWidget(update_frame);
    connect(update_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::search()
{
    // Remove everything
    delete question_frame;
    answer_frame->hide();
	back_button->disconnect();
	back_button->hide();
    add_theme_button->disconnect();
    add_theme_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    add_button->disconnect();
    add_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    update_button->disconnect();
    update_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    search_button->disconnect();
    search_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.

    // Create a new search frame
    search_frame = new SearchFrame(test, this);
    layout->addWidget(search_frame);
    connect(search_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::go_back() {
	delete this;
}

void TestFrame::find_themes() {
	if (!test.isRemoteWork()) {
        // Offline
        Parser* p = new Parser(test.getSrc() + test.getDst());
		read_reply(p->search("", Parser::getThemeFile()));
	} else { 
		// Request to PHP file
        const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/find_themes.php");
		QNetworkRequest request(url);
		nam_themes.get(request);
	}
}

void TestFrame::read_reply_themes(QNetworkReply* reply)
{
    // Store the lines of the reply in the "reply_list" attribute
    QString reply_string = reply->readAll();
    reply->deleteLater();
	read_reply(reply_string);
}

void TestFrame::read_reply(QString reply_string) {
    QStringList reply_list(reply_string.split('\n', QString::SkipEmptyParts));
	for(int i=0, l = reply_list.count(); i<l-1; i+=2) {
		themes->addItem(reply_list.at(i+1), QVariant(reply_list.at(i).toInt()));
	}
}
