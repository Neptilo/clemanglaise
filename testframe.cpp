#include "testframe.h"

TestFrame::TestFrame(Test &test, QWidget *parent):
    QWidget(parent),
    test(test)
{
    layout = new QVBoxLayout(this);
    answer_frame = new AnswerFrame(test, this);
    init();
}

TestFrame::~TestFrame(){
    delete question_frame;
    delete answer_frame;
    delete add_frame;
    delete request;
    delete nam;
    delete reply_list;
    delete layout;
}

void TestFrame::init(){
    label = new QLabel(tr("Choose a theme"), this);
    layout->addWidget(label);

	themes = new QComboBox(this);
	themes->addItem("Restaurant");
	themes->addItem("Business");
	themes->addItem("Internship");
    layout->addWidget(themes);

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
		parser = new Parser();
		parser->parse();
		url = QUrl(Parser::get_working_path("file_out"));
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
    reply_list = new QStringList(reply_string->split('\n'));

    // Everything is ready for the question frame to ask the question.
    QString word = reply_list->at(1);
    question_frame->ask_question(word);
}

void TestFrame::validate_question(){

    // Create a new answer frame
    delete answer_frame;
    answer_frame = new AnswerFrame(*reply_list, question_frame->getAnswer(), test, this);
    layout->addWidget(answer_frame);
}

void TestFrame::validate_answer(){

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

void TestFrame::add_word(){

    // Remove everything
    delete question_frame;
    answer_frame->hide();
    add_button->disconnect();
    add_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    update_button->disconnect();
    update_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    search_button->disconnect();
    search_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.

    // Create a new add frame
    QStringList default_values_list;
    default_values_list << "" << "" << "" << "" << "" << "" << "";
    add_frame = new EditFrame(test, tr("<b>Add a new word</b>"), default_values_list, tr("Add"), "add", tr("Word successfully added!"), this);
    layout->addWidget(add_frame);
    connect(add_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::update_word(){

    // Remove everything
    delete question_frame;
    answer_frame->hide();
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
