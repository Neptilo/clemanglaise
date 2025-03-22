#include <QtNetwork>
#include <QTextDocument>
#include <QStandardItemModel>
#include <QListView>
#include <QTableView>
#include <QUrlQuery>

#include "AddTagView.h"
#include "CheckableItemDelegate.h"
#include "QuestionView.h"
#include "resource_utils.h"
#include "string_utils.h"
#include "NetworkReplyReader.h"

AddTagView::AddTagView(Test *test,
                       const QString &title,
                       const QStringList &default_values,
                       const QString &OK_button_value,
                       const QString &php_filename,
                       const QString &success_message,
                       #ifndef Q_OS_WASM
                       DatabaseManager* database_manager,
                       #endif
                       QWidget *parent) :
    QWidget(parent),
    title(nullptr),
    status(nullptr),
    tag_edit(nullptr),
    OK_button(nullptr),
    continue_button(nullptr),
    layout(nullptr),
    test(test),
    reply_list(),
    tag(nullptr),
    tags(nullptr)
  #ifndef Q_OS_WASM
  ,database_manager(database_manager)
  #endif
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

    this->title = new QLabel(title, this);
    layout->addWidget(this->title);

    // Define explicit variables for the default values of the fields.
    QString tag = ampersand_unescape(default_values.at(1));

    tag_edit = new QLineEdit(tag, this);
    layout->addRow(tr("&Tag:"), tag_edit);


    status = new QLabel(this);
    layout->addWidget(status);

    OK_button = new QPushButton(OK_button_value, this);
    OK_button->setIcon(getIcon("emblem-default"));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(edit_tag()));
    layout->addWidget(OK_button);
}

AddTagView::~AddTagView(){}

void AddTagView::edit_tag(){
    status->setText(tr("Sending data..."));
    if (test->is_remote()) {
        QUrlQuery post_data;
        post_data.addQueryItem("id", this->default_values.at(0));
        QString line = ampersand_escape(tag_edit->text().left(1).toUpper() + tag_edit->text().mid(1));
        post_data.addQueryItem("tag", line);
        const QUrl url("https://neptilo.com/php/clemanglaise/"+this->php_filename+".php");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        // Send the request
        QNetworkReply* reply = NetworkReplyReader::nam->post(
                    request, post_data.query().toUtf8());

        // Will show confirmation when loading of reply is finished
        connect(reply, SIGNAL(finished()), this, SLOT(read_edit_tag_reply()));
#ifndef Q_OS_WASM
    } else {
        // Offline
        // Will show confirmation when loading of reply is finished
        database_manager->add_tag(tag_edit->text().left(1).toUpper() + tag_edit->text().mid(1));
        show_confirmation();
#endif
    }

}

void AddTagView::read_edit_tag_reply(){
    auto reply = qobject_cast<QNetworkReply*>(sender());
    const QString reply_string(reply->readAll().replace('\0', ""));
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
    continue_button->setIcon(getIcon("list-add"));
    layout->addWidget(continue_button);
    connect(continue_button, SIGNAL(clicked()), this, SLOT(reset()));
}

void AddTagView::reset(){

    tag_edit->setText(default_values.at(1));
    delete continue_button;

    OK_button = new QPushButton(tr("Add Tag"), this);
    OK_button->setIcon(getIcon("emblem-default"));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(edit_tag()));
    layout->addWidget(OK_button);
}


void AddTagView::find_tags() {
    if (test->is_remote()) {
        // Request to PHP file
        const QUrl url = QUrl("https://neptilo.com/php/clemanglaise/find_tags.php");
        QNetworkRequest request(url);
        QNetworkReply* reply = NetworkReplyReader::nam->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(read_reply()));
#ifndef Q_OS_WASM
    } else {
        // Offline
        database_manager->find_tags(reply_list);
        read_reply("");
#endif
    }
}

void AddTagView::read_reply()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());

    // Store the lines of the reply in the "reply_list" attribute
    QString reply_string = reply->readAll().replace('\0', "");
    reply->deleteLater();
    read_reply(reply_string);
}

void AddTagView::read_reply(QString reply_string) {
    if (test->is_remote())
        reply_list = reply_string.split('\n', Qt::SkipEmptyParts);
    for(int i=0, l = reply_list.count(); i<l-1; i+=2)
        tags->addItem(reply_list.at(i+1), QVariant(reply_list.at(i).toInt()));
}
