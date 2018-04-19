#include "AddListView.h"

#include <QDebug>
#include <QFormLayout>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#   include <QUrlQuery>
#endif

#include "NetworkReplyReader.h"
#include "iso_mapping.h"
#include "string_utils.h"

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
    title(tr("<b>Create a vocabulary list</b>")),
    test(NULL)
{
    lang_completer = new QCompleter(LANGUAGES, this);
    lang_completer->setCaseSensitivity(Qt::CaseInsensitive);
    src_edit.setCompleter(lang_completer);
    dst_edit.setCompleter(lang_completer);
    country_completer = new QCompleter(COUNTRIES, this);
    country_completer->setCaseSensitivity(Qt::CaseInsensitive);
    flag_edit.setCompleter(country_completer);
    QFormLayout* layout = new QFormLayout(this);
    layout->addWidget(&title);
    layout->addRow(tr("&Name: "), &name_edit);
    layout->addRow(tr("&Source language: "), &src_edit);
    layout->addRow(tr("&Destination language: "), &dst_edit);
    layout->addRow(tr("Illustration &flag country: "), &flag_edit);
    layout->addWidget(&status);
    status.hide();
    layout->addWidget(&create_button);
    layout->addWidget(&cancel_button);
    if(remote)
        connect(&create_button, SIGNAL(clicked()), this, SLOT(add_online_list()));
    else
        connect(&create_button, SIGNAL(clicked()), this, SLOT(add_offline_list()));
    connect(&cancel_button, SIGNAL(clicked()), this, SIGNAL(canceled()));
}

AddListView::~AddListView()
{
    delete test;
}

Test *AddListView::get_test()
{
    return test;
}

void AddListView::add_offline_list()
{
    int test_id = 0;
    QString src_test(""), dst_test(""), test_flag(""), error("");

    if (!check_inputs(src_test, dst_test, test_flag, error))
    {
        status.setText(tr("<b>Error: </b>") + error);
        status.show();
        return;
    }
    database_manager->add_list(name_edit.text(), src_test,  dst_test, test_flag, test_id);
    error = database_manager->pop_last_error();
    if(error.isEmpty()){
        test = new Test(test_id, name_edit.text(), src_test, dst_test, test_flag, false);
        emit created(test);
    }else{
        status.setText(tr("<b>SQLite error: </b>") + error);
        status.show();
    }
}

bool AddListView::check_inputs(QString& src_test, QString& dst_test, QString& test_flag, QString& error)
{
    src_test = LANG_MAP.key(src_edit.text().toUpper().trimmed());
    dst_test = LANG_MAP.key(dst_edit.text().toUpper().trimmed());
    test_flag = COUNTRY_MAP.key(flag_edit.text().toUpper().trimmed());
    error="";
    if(src_test.isEmpty())
        error = tr("Please select a valid source language from the provided list");
    else if (dst_test.isEmpty())
        error = tr("Please select a valid destination language from the provided list");
    else if(test_flag.isEmpty())
        error = tr("Please select a valid country flag from the provided list");
    else if(src_test == dst_test)
        error = tr("Source and destination languages cannot be the same");
    return error.isEmpty();
}

void AddListView::add_online_list()
{
    QString src_test(""), dst_test(""), test_flag(""), error("");

    if (!check_inputs(src_test, dst_test, test_flag, error))
    {
        status.setText(tr("<b>Error: </b>") + error);
        status.show();
        return;
    }

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        QUrl post_data;
#else
        QUrlQuery post_data;
#endif    
    post_data.addQueryItem("name", name_edit.text());
    post_data.addQueryItem("src", src_test);
    post_data.addQueryItem("dst", dst_test);
    post_data.addQueryItem("flag", test_flag);
    const QUrl url = QUrl("https://neptilo.com/php/clemanglaise/add_list");
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
    const QString error(reply->readAll().replace('\0', ""));
    if(error == "")
        emit created(NULL);
    else{
        status.setText(error);
        status.show();
    }
}
