#include <QtNetwork>
#include <QTextDocument>

#include "editframe.h"
#include "string_utils.h"
#include "Parser.h"

EditFrame::EditFrame(Test &test, const QString &title, const QStringList &default_values, const QString &OK_button_value, const QString &php_filename, const QString &success_message, QWidget *parent) :
    QWidget(parent),
    test(test)
{
    this->php_filename = php_filename;
    this->default_values = default_values;
    this->success_message = success_message;

    layout = new QFormLayout(this);

    this->title = new QLabel(title, this);
    layout->addWidget(this->title);

    // Define explicit variables for the default values of the fields.
    QString word = ampersand_unescape(default_values.at(1));
    QString meaning = ampersand_unescape(default_values.at(2));
    QString nature = default_values.at(3);
    QString comment = default_values.at(4);
    QString example = default_values.at(5);
    QString pronunciation = ampersand_unescape(default_values.at(6));

    word_edit = new QLineEdit(word, this);
    layout->addRow(tr("&Word:"), word_edit);

    nature_edit = new QComboBox(this);
    nature_edit->addItem("---");
    nature_edit->addItem(tr("Adjective"), QVariant("adj"));
    nature_edit->addItem(tr("Adverb"), QVariant("adv"));
    nature_edit->addItem(tr("Article"), QVariant("art"));
    nature_edit->addItem(tr("Classifier"), QVariant("clas"));
    nature_edit->addItem(tr("Conjunction"), QVariant("conj"));
    nature_edit->addItem(tr("Interjecion"), QVariant("inter"));
    nature_edit->addItem(tr("Noun"), QVariant("n"));
    nature_edit->addItem(tr("Preposition"), QVariant("prep"));
    nature_edit->addItem(tr("Pronoun"), QVariant("pron"));
    nature_edit->addItem(tr("Verb"), QVariant("v"));
    nature_edit->setCurrentIndex(nature_edit->findData(QVariant(nature)));
    layout->addRow(tr("&Nature:"), nature_edit);

    meaning_edit = new QLineEdit(meaning, this);
    layout->addRow(tr("&Meaning:"), meaning_edit);

    QStringList list;
    list << "ja" << "zh";
    bool asked_pronunciation = list.contains(test.getDst());

    if(asked_pronunciation){
        pronunciation_edit = new QLineEdit(pronunciation, this);
        layout->addRow(tr("&Pronunciation:"), pronunciation_edit);
    }

    comment_edit = new QTextEdit(comment, this);
    layout->addRow(tr("&Comment:"), comment_edit);

    example_edit = new QTextEdit(example, this);
    layout->addRow(tr("&Example:"), example_edit);

    status = new QLabel(this);
    layout->addWidget(status);

    OK_button = new QPushButton(OK_button_value, this);
    connect(OK_button, SIGNAL(clicked()), this, SLOT(edit_word()));
    layout->addWidget(OK_button);

    cancel_button = new QPushButton(tr("Cancel"), this);
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(back()));
    layout->addWidget(cancel_button);
}

EditFrame::~EditFrame(){}

void EditFrame::edit_word(){
    status->setText(tr("Sending data..."));
	if (!test.isRemoteWork()) {
		//offline
		Parser* p = new Parser;
		// Will show confirmation when loading of reply is finished
		connect(p, SIGNAL(appendDone()), this, SLOT(show_confirmation()));
		QString line = word_edit->text() + "\t:\t" + meaning_edit->text() + endline;
		p->appendInFile(line);
	} else {
		QUrl post_data;
		post_data.addQueryItem("id", this->default_values.at(0));
		post_data.addQueryItem("word", ampersand_escape(word_edit->text()));
		post_data.addQueryItem("nature", nature_edit->itemData(nature_edit->currentIndex()).toString());
		post_data.addQueryItem("meaning", ampersand_escape(meaning_edit->text()));
		QStringList list;
		list << "ja" << "zh";
		bool asked_pronunciation = list.contains(test.getDst());
		if(asked_pronunciation){
			// Standardize pronunciation to save into database
			QString standardized_pronunciation;

			if(test.getDst() == "ja"){
				standardized_pronunciation = ampersand_escape(pronunciation_edit->text());
				standardized_pronunciation.replace(QString("ou"), QString("&#333;"));
				standardized_pronunciation.replace(QString("uu"), QString("&#363;"));
				standardized_pronunciation.replace(QString("aa"), QString("&#257;"));
				standardized_pronunciation.replace(QString("ee"), QString("&#275;"));
			}else if(test.getDst() == "zh"){
				standardized_pronunciation = numbers_to_accents(pronunciation_edit->text());
			}

			post_data.addQueryItem("pronunciation", standardized_pronunciation);
		}
		post_data.addQueryItem("comment", ampersand_escape(comment_edit->toPlainText()));
		post_data.addQueryItem("example", ampersand_escape(example_edit->toPlainText()));
		post_data.addQueryItem("lang", test.getSrc() + test.getDst());

		const QUrl url("http://neptilo.com/php/clemanglaise/"+this->php_filename+".php");
		QNetworkRequest request(url);
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
		QNetworkAccessManager* nam = new QNetworkAccessManager;

		// Will show confirmation when loading of reply is finished
		connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(show_confirmation(QNetworkReply*)));


		// Send the request
		nam->post(request, post_data.encodedQuery());
	}
	
}

void EditFrame::show_confirmation(QNetworkReply* reply){
    const QString reply_string(reply->readAll());
    reply->deleteLater();
    if(reply_string.compare("")){
        status->setText(reply_string);
    }else{
        status->setText(this->success_message);
    }
    delete OK_button;
    cancel_button->setText(tr("Back to test"));
}

void EditFrame::show_confirmation(){
    status->setText(this->success_message);
    delete OK_button;
    cancel_button->setText(tr("Back to test"));
}

void EditFrame::back(){
    delete this;
}
