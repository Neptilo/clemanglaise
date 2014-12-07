#include <QtNetwork>
#include <QTextDocument>
#include <QStandardItemModel>
#include <QListView>
#include <QTableView>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#   include <QUrlQuery>
#endif

#include "AddTagView.h"
#include "CheckableItemDelegate.h"
#include "QuestionView.h"
#include "string_utils.h"
#include "NetworkReplyReader.h"

AddTagView::AddTagView(Test *test, const QString &title, const QStringList &default_values, const QString &OK_button_value, const QString &php_filename, const QString &success_message, DatabaseManager* database_manager, QWidget *parent) :
    QWidget(parent),
    title(NULL),
    status(NULL),
    tag_edit(NULL),
    OK_button(NULL),
    nam(),
    continue_button(NULL),
    layout(NULL),
    test(test),
	reply_list(),
    tag(NULL),
    tags(NULL),
	database_manager(database_manager)
{
    this->php_filename = php_filename;
    this->default_values = default_values;
    this->success_message = success_message;

    layout = new QFormLayout(this);

    tag = new QLabel(tr("Available tags"), this);
    layout->addWidget(tag);
    tags = new QComboBox(this);
    layout->addWidget(tags);

    find_tags();

    connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));

    this->title = new QLabel(title, this);
    layout->addWidget(this->title);

    // Define explicit variables for the default values of the fields.
    QString tag = ampersand_unescape(default_values.at(1));

    tag_edit = new QLineEdit(tag, this);
    layout->addRow(tr("&Tag:"), tag_edit);


    status = new QLabel(this);
    layout->addWidget(status);

    OK_button = new QPushButton(OK_button_value, this);
    OK_button->setIcon(QIcon::fromTheme("emblem-default", QIcon(getImgPath("emblem-default.png"))));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(edit_tag()));
    layout->addWidget(OK_button);
}

AddTagView::~AddTagView(){}

void AddTagView::edit_tag(){
    status->setText(tr("Sending data..."));
    if (!test->is_remote()) {
        // Offline
		// Will show confirmation when loading of reply is finished
        database_manager->add_tag(tag_edit->text().left(1).toUpper() + tag_edit->text().mid(1));
        show_confirmation();
	} else {
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        QUrl post_data;
#else
        QUrlQuery post_data;
#endif
		post_data.addQueryItem("id", this->default_values.at(0));
        QString line = ampersand_escape(tag_edit->text().left(1).toUpper() + tag_edit->text().mid(1));
        post_data.addQueryItem("tag", line);
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

void AddTagView::show_confirmation(QNetworkReply* reply){
    const QString reply_string(reply->readAll());
    reply->deleteLater();
    if(reply_string.compare("")){
        status->setText(reply_string);
    }else{
        status->setText(this->success_message);
    }
	show_confirmation();
}

void AddTagView::show_confirmation(){
	status->setText(this->success_message);
    delete OK_button;
    continue_button = new QPushButton(tr("Add another tag"), this);
	continue_button->setIcon(QIcon::fromTheme("list-add",QIcon(getImgPath("list-add.png"))));
    layout->addWidget(continue_button);
    connect(continue_button, SIGNAL(clicked()), this, SLOT(reset()));
}

void AddTagView::reset(){

    tag_edit->setText(default_values.at(1));
    delete continue_button;

    OK_button = new QPushButton(tr("Add Tag"), this);
    OK_button->setIcon(QIcon::fromTheme("emblem-default", QIcon(getImgPath("emblem-default.png"))));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(edit_tag()));
    layout->addWidget(OK_button);
}


void AddTagView::find_tags() {
    if (!test->is_remote()) {
        // Offline
		database_manager->find_tags(reply_list);
		read_reply();
	} else { 
		// Request to PHP file
        const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/find_tags.php");
		QNetworkRequest request(url);
		nam.get(request);
	}
}

void AddTagView::read_reply(QNetworkReply* reply)
{
    // Store the lines of the reply in the "reply_list" attribute
    QString reply_string = reply->readAll();
    reply->deleteLater();
	read_reply(reply_string);
}

void AddTagView::read_reply(QString reply_string) {
    if (test->is_remote())
		reply_list = reply_string.split('\n', QString::SkipEmptyParts);
    for(int i=0, l = reply_list.count(); i<l-1; i+=2)///
        tags->addItem(reply_list.at(i+1), QVariant(reply_list.at(i).toInt()));
}
