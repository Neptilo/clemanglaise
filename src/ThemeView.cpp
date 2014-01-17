#include <QtNetwork>
#include <QTextDocument>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#   include <QUrlQuery>
#endif

#include "ThemeView.h"
#include "QuestionView.h"
#include "string_utils.h"
#include "Parser.h"
#include "NetworkReplyReader.h"

ThemeView::ThemeView(Test &test, const QString &title, const QStringList &default_values, const QString &OK_button_value, const QString &php_filename, const QString &success_message, QWidget *parent) :
    QWidget(parent),
    title(NULL),
    status(NULL),
    theme_edit(NULL),
    OK_button(NULL),
    nam(),
    cancel_button(NULL),
    continue_button(NULL),
    layout(NULL),
    test(test),
    theme(NULL),
    themes(NULL)
{
    this->php_filename = php_filename;
    this->default_values = default_values;
    this->success_message = success_message;

    layout = new QFormLayout(this);

	theme = new QLabel(tr("Available themes"), this);
	layout->addWidget(theme);
	themes = new QComboBox(this);
	layout->addWidget(themes);
	find_themes();

    connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));

    this->title = new QLabel(title, this);
    layout->addWidget(this->title);

    // Define explicit variables for the default values of the fields.
    QString theme = ampersand_unescape(default_values.at(1));

    theme_edit = new QLineEdit(theme, this);
    layout->addRow(tr("&Theme:"), theme_edit);


    status = new QLabel(this);
    layout->addWidget(status);

    OK_button = new QPushButton(OK_button_value, this);
    OK_button->setIcon(QIcon::fromTheme("emblem-default", QIcon(getImgPath("emblem-default.png"))));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(edit_theme()));
    layout->addWidget(OK_button);

    cancel_button = new QPushButton(tr("Cancel"), this);
	cancel_button->setIcon(QIcon::fromTheme("process-stop", QIcon(getImgPath("process-stop.png"))));
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(back()));
    layout->addWidget(cancel_button);
}

ThemeView::~ThemeView(){}

void ThemeView::edit_theme(){
    status->setText(tr("Sending data..."));
	if (!test.isRemoteWork()) {
        // Offline
        Parser p(test.getSrc() + test.getDst());

		// Will show confirmation when loading of reply is finished
        connect(&p, SIGNAL(appendDone()), this, SLOT(show_confirmation()));
		QString line = colon_unescape(theme_edit->text().left(1).toUpper() + theme_edit->text().mid(1)) + endline;
        p.appendInFile(line, Parser::getThemeFile());
        p.deleteLineId(default_values.at(0).toInt(), p.getFilein());
	} else {
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        QUrl post_data;
#else
        QUrlQuery post_data;
#endif
		post_data.addQueryItem("id", this->default_values.at(0));
		QString line = ampersand_escape(theme_edit->text().left(1).toUpper() + theme_edit->text().mid(1));
		post_data.addQueryItem("theme", line);
		const QUrl url("http://neptilo.com/php/clemanglaise/"+this->php_filename+".php");
		QNetworkRequest request(url);
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QNetworkAccessManager* nam2 = new QNetworkAccessManager(this);
        nam2->setCookieJar(NetworkReplyReader::cookie_jar); // By default, nam takes ownership of the cookie jar.
        nam2->cookieJar()->setParent(0); // Unset the cookie jar's parent so it is not deleted when nam is deleted, and can still be used by other NAMs.

		// Will show confirmation when loading of reply is finished
		connect(nam2, SIGNAL(finished(QNetworkReply*)), this, SLOT(show_confirmation(QNetworkReply*)));

		// Send the request
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        nam2->post(request, post_data.encodedQuery());
#else
        nam2->post(request, post_data.query(QUrl::FullyEncoded).toUtf8());
#endif

	}
	
}

void ThemeView::show_confirmation(QNetworkReply* reply){
    const QString reply_string(reply->readAll());
    reply->deleteLater();
    if(reply_string.compare("")){
        status->setText(reply_string);
    }else{
        status->setText(this->success_message);
    }
    delete OK_button;
    continue_button = new QPushButton(tr("Add another theme"), this);
	continue_button->setIcon(QIcon::fromTheme("list-add",QIcon(getImgPath("list-add.png"))));
    layout->addWidget(continue_button);
    connect(continue_button, SIGNAL(clicked()), this, SLOT(reset()));
    cancel_button->setText(tr("Back to test"));
}

void ThemeView::show_confirmation(){
    status->setText(this->success_message);
    delete OK_button;
    continue_button = new QPushButton(tr("Add another theme"), this);
	continue_button->setIcon(QIcon::fromTheme("list-add",QIcon(getImgPath("list-add.png"))));
    layout->addWidget(continue_button);
    connect(continue_button, SIGNAL(clicked()), this, SLOT(reset()));
    cancel_button->setText(tr("Back to test"));
}

void ThemeView::back(){
    delete this;
}

void ThemeView::reset(){

	theme_edit->setText(default_values.at(1));
    delete continue_button;

    OK_button = new QPushButton(tr("Add Theme"), this);
    OK_button->setIcon(QIcon::fromTheme("emblem-default", QIcon(getImgPath("emblem-default.png"))));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(edit_theme()));
    layout->addWidget(OK_button);

    cancel_button->setText(tr("Cancel"));
}


void ThemeView::find_themes() {
	if (!test.isRemoteWork()) {
        // Offline
        Parser p(test.getSrc() + test.getDst());
        read_reply(p.search("", Parser::getThemeFile()));
	} else { 
		// Request to PHP file
        const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/find_themes.php");
		QNetworkRequest request(url);
		nam.get(request);
	}
}

void ThemeView::read_reply(QNetworkReply* reply)
{
    // Store the lines of the reply in the "reply_list" attribute
    QString reply_string = reply->readAll();
    reply->deleteLater();
	read_reply(reply_string);
}

void ThemeView::read_reply(QString reply_string) {
    QStringList reply_list(reply_string.split('\n', QString::SkipEmptyParts));
	for(int i=0, l = reply_list.count(); i<l-1; i+=2) {
		themes->addItem(reply_list.at(i+1), QVariant(reply_list.at(i).toInt()));
	}
}
