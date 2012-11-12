#include "addframe.h"
#include <QtNetwork>

AddFrame::AddFrame(QWidget *parent) :
    QWidget(parent){

    layout = new QFormLayout(this);

    title = new QLabel("<b>Add a new word</b>", this);
    layout->addWidget(title);

    word = new QLineEdit(this);
    layout->addRow(tr("&Word:"), word);

    nature = new QComboBox(this);
    nature->addItem("---");
    nature->addItem("Adjective", QVariant("adj"));
    nature->addItem("Adverb", QVariant("adv"));
    nature->addItem("Article", QVariant("art"));
    nature->addItem("Conjunction", QVariant("conj"));
    nature->addItem("Noun", QVariant("n"));
    nature->addItem("Preposition", QVariant("prep"));
    nature->addItem("Pronoun", QVariant("pron"));
    nature->addItem("Verb", QVariant("v"));
    layout->addRow(tr("&Nature:"), nature);

    meaning = new QLineEdit(this);
    layout->addRow(tr("&Meaning:"), meaning);

    comment = new QTextEdit(this);
    layout->addRow(tr("&Comment:"), comment);

    example = new QTextEdit(this);
    layout->addRow(tr("&Example:"), example);

    status = new QLabel(this);
    layout->addWidget(status);

    OK_button = new QPushButton("Add", this);
    connect(OK_button, SIGNAL(clicked()), this, SLOT(add_word()));
    layout->addWidget(OK_button);
}

AddFrame::~AddFrame(){
    delete title;
    delete word;
    delete nature;
    delete meaning;
    delete comment;
    delete example;
    delete OK_button;
    delete layout;
}

void AddFrame::add_word(){
    status->setText("Sending data...");
    QUrl post_data;
    post_data.addQueryItem("word", word->text());
    post_data.addQueryItem("nature", nature->itemData(nature->currentIndex()).toString());
    post_data.addQueryItem("meaning", meaning->text());
    post_data.addQueryItem("comment", comment->toPlainText());
    post_data.addQueryItem("example", example->toPlainText());
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
    disconnect(OK_button);
    OK_button->setText("Back to test");
    connect(OK_button, SIGNAL(clicked()), this, SLOT(exit()));
    connect(OK_button, SIGNAL(clicked()), parent(), SLOT(init()));
}

void AddFrame::exit(){
    delete this;
}
