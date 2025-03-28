#include "AddListView.h"

#include <QDebug>
#include <QFormLayout>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "NetworkReplyReader.h"
#include "iso_mapping.h"
#include "string_utils.h"

AddListView::AddListView(
        #ifndef Q_OS_WASM
        DatabaseManager *database_manager,
        #endif
        bool remote, QWidget *parent) :
    QWidget(parent),
    cancel_button(tr("C&ancel"), this),
    create_button(tr("&Create"), this),
    #ifndef Q_OS_WASM
    database_manager(database_manager),
    #endif
    dst_edit(this),
    name_edit(this),
    src_edit(this),
    status(this),
    title(tr("<b>Create a vocabulary list</b>")),
    test(nullptr)
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

#ifndef Q_OS_WASM
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
#endif

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

    QUrlQuery post_data;
    post_data.addQueryItem("name", name_edit.text());
    post_data.addQueryItem("src", src_test);
    post_data.addQueryItem("dst", dst_test);
    post_data.addQueryItem("flag", test_flag);
    const QUrl url = QUrl(NetworkReplyReader::api_url + "add_list");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");

    QNetworkReply* reply = NetworkReplyReader::nam->post(
                request, post_data.query().toUtf8());
    connect(reply, SIGNAL(finished()), this, SLOT(show_confirmation()));
}

void AddListView::show_confirmation()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    const QString error(reply->readAll().replace('\0', ""));
    if(error == "")
        emit created(nullptr);
    else{
        status.setText(error);
        status.show();
    }
}
