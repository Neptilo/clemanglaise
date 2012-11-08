#include "addframe.h"
#include <QtNetwork>

AddFrame::AddFrame(QWidget *parent) :
    QWidget(parent){

    layout = new QVBoxLayout(this);

    title = new QLabel("Add a new word", this);
    layout->addWidget(title);

    word = new QLabel("Word: ", this);
    layout->addWidget(word);

    word_edit = new QLineEdit(this);
    layout->addWidget(word_edit);

    meaning = new QLabel("Meaning: ", this);
    layout->addWidget(meaning);

    meaning_edit = new QLineEdit(this);
    layout->addWidget(meaning_edit);

    comment = new QLabel("Comment: ", this);
    layout->addWidget(comment);

    comment_edit = new QLineEdit(this);
    layout->addWidget(comment_edit);

    example = new QLabel("Example: ", this);
    layout->addWidget(example);

    example_edit = new QLineEdit(this);
    layout->addWidget(example_edit);

    status = new QLabel(this);
    layout->addWidget(status);

    OK_button = new QPushButton("Add", this);
    connect(OK_button, SIGNAL(clicked()), this, SLOT(add_word()));
    layout->addWidget(OK_button);
}

AddFrame::~AddFrame(){
    delete title;
    delete word;
    delete meaning;
    delete comment;
    delete example;
    delete word_edit;
    delete meaning_edit;
    delete comment_edit;
    delete example_edit;
    delete OK_button;
    delete layout;
}

void AddFrame::add_word(){
    status->setText("Sending data...");
    QUrl post_data;
    post_data.addQueryItem("word", word_edit->text());
    post_data.addQueryItem("meaning", meaning_edit->text());
    post_data.addQueryItem("comment", comment_edit->text());
    post_data.addQueryItem("example", example_edit->text());
    const QUrl url("http://neptilo.com/php/clemanglaise/add.php");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkAccessManager* nam = new QNetworkAccessManager;
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(show_confirmation(QNetworkReply*)));
    nam->post(request, post_data.encodedQuery());
}

void AddFrame::show_confirmation(QNetworkReply* reply){
    const QString reply_string(reply->readAll());
    reply->deleteLater();
    if(reply_string.compare("")){
        status->setText(reply_string);
    }else{
        status->setText("Word successfully added!");
    }
    OK_button->setText("Back to test");
    // TODO: Connect button back to test
}
