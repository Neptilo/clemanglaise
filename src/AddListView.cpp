#include "AddListView.h"

#include <QDebug>
#include <QFormLayout>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#   include <QUrlQuery>
#endif

#include "NetworkReplyReader.h"

AddListView::AddListView(DatabaseManager *database_manager, bool remote, QWidget *parent) :
    QWidget(parent),
    cancel_button(tr("C&ancel"), this),
    create_button(tr("&Create"), this),
    database_manager(database_manager),
    dst_edit(this),
    nam(NULL),
    name_edit(this),
    src_edit(this),
    status(this),
    title(tr("<b>Create a vocabulary list</b>"))
{
    QFormLayout* layout = new QFormLayout(this);
    layout->addWidget(&title);
    layout->addRow(tr("&Name: "), &name_edit);
    layout->addRow(tr("&Source language: "), &src_edit);
    layout->addRow(tr("&Destination language: "), &dst_edit);
    layout->addWidget(&status);
    status.hide();
    layout->addWidget(&create_button);
    layout->addWidget(&cancel_button);
    if(remote)
        connect(&create_button, SIGNAL(clicked()), this, SLOT(add_online_list()));
    else
        connect(&create_button, SIGNAL(clicked()), this, SLOT(add_offline_list()));
    connect(this, SIGNAL(add_list_success()), parent, SLOT(remove_add_list_view()));
    connect(&cancel_button, SIGNAL(clicked()), parent, SLOT(remove_add_list_view()));
}

void AddListView::add_offline_list()
{
    database_manager->add_list(name_edit.text(), src_edit.text(), dst_edit.text());
    QString error(database_manager->pop_last_error());
    if(error == "")
        emit add_list_success();
    else{
        status.setText(tr("<b>SQLite error: </b>") + error);
        status.show();
    }
}

void AddListView::add_online_list()
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        QUrl post_data;
#else
        QUrlQuery post_data;
#endif
    post_data.addQueryItem("name", name_edit.text());
    post_data.addQueryItem("src", src_edit.text());
    post_data.addQueryItem("dst", dst_edit.text());
    const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/add_list");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    nam = new QNetworkAccessManager(this);
    nam->setCookieJar(NetworkReplyReader::cookie_jar); // By default, nam takes ownership of the cookie jar.
    nam->cookieJar()->setParent(0); // Unset the cookie jar's parent so it is not deleted when nam is deleted, and can still be used by other NAMs.
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(show_confirmation(QNetworkReply*)));
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        nam->post(request, post_data.encodedQuery());
#else
        nam->post(request, post_data.query(QUrl::FullyEncoded).toUtf8());
#endif
}

void AddListView::show_confirmation(QNetworkReply *reply)
{
    const QString error(reply->readAll());
    if(error == "")
        emit add_list_success();
    else{
        status.setText(error);
        status.show();
    }
}
