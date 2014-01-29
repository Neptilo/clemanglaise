#include <QtNetwork>
#include <QDebug>
#include <QTextDocument>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#   include <QUrlQuery>
#endif

#include "EditView.h"
#include "QuestionView.h"
#include "string_utils.h"
#include "NetworkReplyReader.h"

EditView::EditView(Test &test, const QString &title, const QStringList &default_values, const QString &OK_button_value, const QString &php_filename, const QString &success_message, DatabaseManager *database_manager, QWidget *parent) :
    QWidget(parent),
    title(NULL),
    status(NULL),
    nature_edit(NULL),
    themes(NULL),
    nam(),
    theme_nam(),
    word_edit(NULL),
    meaning_edit(NULL),
    pronunciation_edit(NULL),
    comment_edit(NULL),
    example_edit(NULL),
    OK_button(NULL),
    cancel_button(NULL),
    continue_button(NULL),
    layout(NULL),
    test(test),
	reply_list(),
    database_manager(database_manager)
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
    QString pronunciation = ampersand_unescape(default_values.at(7));
	int id_theme = default_values.at(6).toInt();

    word_edit = new QLineEdit(word, this);
    layout->addRow(tr("&Word: "), word_edit);

    nature_edit = new QComboBox(this);
    nature_edit->addItem("---");
    nature_edit->addItem(tr("Adjective"), QVariant("adj"));
    nature_edit->addItem(tr("Adverb"), QVariant("adv"));
    nature_edit->addItem(tr("Article"), QVariant("art"));
    nature_edit->addItem(tr("Classifier"), QVariant("clas"));
    nature_edit->addItem(tr("Conjunction"), QVariant("conj"));
    nature_edit->addItem(tr("Interjection"), QVariant("inter"));
    nature_edit->addItem(tr("Noun"), QVariant("n"));
    nature_edit->addItem(tr("Preposition"), QVariant("prep"));
    nature_edit->addItem(tr("Pronoun"), QVariant("pron"));
    nature_edit->addItem(tr("Verb"), QVariant("v"));
    nature_edit->setCurrentIndex(nature_edit->findData(QVariant(nature)));
    layout->addRow(tr("&Nature: "), nature_edit);

    meaning_edit = new QLineEdit(meaning, this);
    layout->addRow(tr("&Translation: "), meaning_edit);

	pronunciation_edit = new QLineEdit(pronunciation, this);
	layout->addRow(tr("&Pronunciation: "), pronunciation_edit);

	this->id_theme = id_theme;

    comment_edit = new QTextEdit(comment, this);
    layout->addRow(tr("&Comment: "), comment_edit);

    example_edit = new QTextEdit(example, this);
    layout->addRow(tr("&Example: "), example_edit);

    status = new QLabel(this);
    layout->addWidget(status);

    themes = new QComboBox(this);
    layout->addRow(tr("T&heme: "),themes);
    find_themes();
    connect(&theme_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));

    nam.setCookieJar(NetworkReplyReader::cookie_jar); // By default, nam takes ownership of the cookie jar.
    nam.cookieJar()->setParent(0); // Unset the cookie jar's parent so it is not deleted when nam is deleted, and can still be used by other NAMs.

	OK_button = new QPushButton(OK_button_value, this);
    OK_button->setIcon(QIcon::fromTheme("emblem-default", QIcon(getImgPath("emblem-default.png"))));

	connect(OK_button, SIGNAL(clicked()), this, SLOT(edit_word()));
	layout->addWidget(OK_button);

	cancel_button = new QPushButton(tr("Cancel"), this);
	cancel_button->setIcon(QIcon::fromTheme("process-stop",QIcon(getImgPath("process-stop.png"))));

	connect(cancel_button, SIGNAL(clicked()), this, SLOT(back()));
	layout->addWidget(cancel_button);
}

EditView::~EditView(){}

void EditView::edit_word(){
	status->setText(tr("Sending data..."));

    // Standardize pronunciation to save into database
    QString standardized_pronunciation;
    if(test.get_dst() == "ja"){
        standardized_pronunciation = ampersand_escape(pronunciation_edit->text());
        standardized_pronunciation.replace(QString("ou"), QString("&#333;"));
        standardized_pronunciation.replace(QString("uu"), QString("&#363;"));
        standardized_pronunciation.replace(QString("aa"), QString("&#257;"));
        standardized_pronunciation.replace(QString("ee"), QString("&#275;"));
    }else if(test.get_dst() == "zh"){
        standardized_pronunciation = numbers_to_accents(pronunciation_edit->text());
    } else {
        standardized_pronunciation = isKirshenbaum(pronunciation_edit->text())?ampersand_escape(kirshenbaum2IPA(pronunciation_edit->text())):ampersand_escape(pronunciation_edit->text());
    }

    // Define data to send
    QHash<QString, QString> word_data;
    word_data["id"] = this->default_values.at(0);
    word_data["word"] = ampersand_escape(word_edit->text());
    word_data["nature"] = nature_edit->itemData(nature_edit->currentIndex()).toString();
    word_data["meaning"] = ampersand_escape(meaning_edit->text());
    word_data["pronunciation"] = standardized_pronunciation;
    word_data["comment"] = ampersand_escape(comment_edit->toPlainText());
    word_data["example"] = ampersand_escape(example_edit->toPlainText());
    word_data["theme"] = themes->itemData(themes->currentIndex()).toString();
    word_data["name"] = test.get_name();

    if (!test.is_remote_work()) {
		bool success;

		// Offline
        if(word_data["id"].toInt() == 0) // Add word
            success = database_manager->add_word(word_data);
        else // Update word
             success = database_manager->update_word(word_data);

        // Show confirmation
        if(success)
            status->setText(success_message);
        else
            status->setText(tr("<b>SQLite error: </b>")+database_manager->pop_last_error());

        delete OK_button;
        continue_button = new QPushButton(tr("Add another word"), this);
        continue_button->setIcon(QIcon::fromTheme("list-add", QIcon(getImgPath("list-add.png"))));

        layout->addWidget(continue_button);
        connect(continue_button, SIGNAL(clicked()), this, SLOT(reset()));
        cancel_button->setText(tr("Back"));
	} else {
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
		QUrl post_data;
#else
		QUrlQuery post_data;
#endif
        for (QHash<QString, QString>::iterator i = word_data.begin(); i != word_data.end(); ++i)
            post_data.addQueryItem(i.key(), i.value());

		const QUrl url("http://neptilo.com/php/clemanglaise/"+this->php_filename+".php");
		QNetworkRequest request(url);
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

		// Will show confirmation when loading of reply is finished
		connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(show_confirmation(QNetworkReply*)));

		// Send the request
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
		nam.post(request, post_data.encodedQuery());
#else
		nam.post(request, post_data.query(QUrl::FullyEncoded).toUtf8());
#endif
	}
	disable_edition(true);
}

void EditView::show_confirmation(QNetworkReply* reply){
	const QString reply_string(reply->readAll());
	reply->deleteLater();
	if(reply_string.compare("")){
		status->setText(reply_string);
	}else{
        status->setText(success_message);
	}
	delete OK_button;
	continue_button = new QPushButton(tr("Add another word"), this);
	continue_button->setIcon(QIcon::fromTheme("list-add", QIcon(getImgPath("list-add.png"))));

	layout->addWidget(continue_button);
	connect(continue_button, SIGNAL(clicked()), this, SLOT(reset()));
	cancel_button->setText(tr("Back"));
}

void EditView::show_confirmation(){
    status->setText(success_message);
	delete OK_button;
	continue_button = new QPushButton(tr("Add another word"), this);
	continue_button->setIcon(QIcon::fromTheme("list-add", QIcon(getImgPath("list-add.png"))));

	layout->addWidget(continue_button);
	connect(continue_button, SIGNAL(clicked()), this, SLOT(reset()));
	cancel_button->setText(tr("Back"));
}

void EditView::back(){
	delete this;
}

void EditView::reset(){

	// Very dirty and non-reusable coding, but it works
	int i = 1;
	word_edit->setText(default_values.at(i++));
	meaning_edit->setText(default_values.at(i++));
	nature_edit->setCurrentIndex(nature_edit->findData(QVariant(default_values.at(i++))));
	comment_edit->setText(default_values.at(i++));
	example_edit->setText(default_values.at(i++));
	themes->setCurrentIndex(themes->findData(QVariant(default_values.at(i++))));
	pronunciation_edit->setText(default_values.at(i++));

	delete continue_button;

	OK_button = new QPushButton(tr("Add word"), this);
	OK_button->setIcon(QIcon::fromTheme("emblem-default", QIcon(":/emblem-default.png")));
	connect(OK_button, SIGNAL(clicked()), this, SLOT(edit_word()));
	layout->addWidget(OK_button);

	cancel_button->setText(tr("Cancel"));
	disable_edition(false);
}

void EditView::find_themes() {
	if (!test.is_remote_work()) {
		// Offline
		database_manager->find_themes(reply_list);
		read_reply();
    } else {
		// Request to PHP file
		const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/find_themes.php");
		QNetworkRequest request(url);
		theme_nam.get(request);
	}
}

void EditView::read_reply(QNetworkReply* reply)
{
	// Store the lines of the reply in the "reply_list" attribute
	QString reply_string = reply->readAll();
	reply->deleteLater();
	read_reply(reply_string);
}

void EditView::read_reply(QString reply_string) {
	if(test.is_remote_work())
		reply_list = reply_string.split('\n', QString::SkipEmptyParts);
	themes->addItem("");
	for(int i=0, l = reply_list.count(); i<l-1; i+=2) {
		themes->addItem(reply_list.at(i+1).trimmed(), QVariant(reply_list.at(i).toInt()));
	}
	themes->setCurrentIndex(themes->findData(QVariant(default_values.at(6).toInt())));
}

void EditView::disable_edition(bool ok) {
	word_edit->setEnabled(!ok);
	nature_edit->setEnabled(!ok);
	meaning_edit->setEnabled(!ok);
	pronunciation_edit->setEnabled(!ok);
	comment_edit->setEnabled(!ok);
	example_edit->setEnabled(!ok);
	themes->setEnabled(!ok); 
}
