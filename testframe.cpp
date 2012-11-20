#include "testframe.h"

TestFrame::TestFrame(QWidget *parent)
    : QWidget(parent){

    setWindowTitle("Clemanglaise");
    layout = new QVBoxLayout(this);

    answer_frame = new AnswerFrame(this);

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
    add_button = new QPushButton("Add word", this);
    connect(add_button, SIGNAL(clicked()), this, SLOT(add_word()));
    layout->addWidget(add_button);

    question_frame = new QuestionFrame(this);
    layout->addWidget(question_frame);

    // Request to PHP file
    const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/find_random.php");
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
    QString word = reply_list->at(0);
    question_frame->ask_question(word);
}

void TestFrame::validate_question(){

    // Create a new answer frame
    delete answer_frame;
    answer_frame = new AnswerFrame(this, *reply_list, question_frame->getAnswer());
    layout->addWidget(answer_frame);
}

void TestFrame::validate_answer(){

    // Remove everything
    delete question_frame;
    answer_frame->hide();

    // Create a new question frame
    question_frame = new QuestionFrame(this);
    layout->addWidget(question_frame);

    // Request for a new question
    nam->get(*request);
}

void TestFrame::add_word(){

    // Remove everything
    delete question_frame;
    answer_frame->hide();
    add_button->disconnect();
    add_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.

    // Create a new add frame
    add_frame = new AddFrame(this);
    layout->addWidget(add_frame);
    connect(add_frame, SIGNAL(destroyed()), this, SLOT(init()));
}
