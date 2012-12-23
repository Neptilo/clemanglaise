#include <QtNetwork>
#include <QTextDocument>

#include "addframe.h"
#include "ampersand_escape.h"

AddFrame::AddFrame(Test &test, QWidget *parent) :
    QWidget(parent),
    test(test)
{

    layout = new QFormLayout(this);

    title = new QLabel(tr("<b>Add a new word</b>"), this);
    layout->addWidget(title);

    word = new QLineEdit(this);
    layout->addRow(tr("&Word:"), word);

    nature = new QComboBox(this);
    nature->addItem("---");
    nature->addItem(tr("Adjective"), QVariant("adj"));
    nature->addItem(tr("Adverb"), QVariant("adv"));
    nature->addItem(tr("Article"), QVariant("art"));
    nature->addItem(tr("Conjunction"), QVariant("conj"));
    nature->addItem(tr("Noun"), QVariant("n"));
    nature->addItem(tr("Preposition"), QVariant("prep"));
    nature->addItem(tr("Pronoun"), QVariant("pron"));
    nature->addItem(tr("Verb"), QVariant("v"));
    layout->addRow(tr("&Nature:"), nature);

    meaning = new QLineEdit(this);
    layout->addRow(tr("&Meaning:"), meaning);

    comment = new QTextEdit(this);
    layout->addRow(tr("&Comment:"), comment);

    example = new QTextEdit(this);
    layout->addRow(tr("&Example:"), example);

    status = new QLabel(this);
    layout->addWidget(status);

    OK_button = new QPushButton(tr("Add"), this);
    connect(OK_button, SIGNAL(clicked()), this, SLOT(add_word()));
    layout->addWidget(OK_button);

    cancel_button = new QPushButton(tr("Cancel"), this);
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(back()));
    layout->addWidget(cancel_button);
}

AddFrame::~AddFrame(){
    delete title;
    delete word;
    delete nature;
    delete meaning;
    delete comment;
    delete example;
    delete OK_button;
    delete cancel_button;
    delete layout;
}

void AddFrame::add_word(){
    status->setText(tr("Sending data..."));
    QUrl post_data;
    post_data.addQueryItem("word", ampersand_escape(word->text()));
    post_data.addQueryItem("nature", nature->itemData(nature->currentIndex()).toString());
    post_data.addQueryItem("meaning", ampersand_escape(meaning->text()));
    post_data.addQueryItem("comment", ampersand_escape(comment->toPlainText()));
    post_data.addQueryItem("example", ampersand_escape(example->toPlainText()));
    post_data.addQueryItem("lang", test.getSrc() + test.getDst());

    const QUrl url("http://neptilo.com/php/clemanglaise/add.php");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkAccessManager* nam = new QNetworkAccessManager;

    // Will show confirmation when loading of reply is finished
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(show_confirmation(QNetworkReply*)));

    // Send the request
    nam->post(request, post_data.encodedQuery());
}

void AddFrame::show_confirmation(QNetworkReply* reply){
    const QString reply_string(reply->readAll());
    reply->deleteLater();
    if(reply_string.compare("")){
        status->setText(reply_string);
    }else{
        status->setText(tr("Word successfully added!"));
    }
    delete OK_button;
    cancel_button->setText(tr("Back to test"));
}

void AddFrame::back(){
    delete this;
}
