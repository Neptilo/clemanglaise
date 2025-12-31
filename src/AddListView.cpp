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
        bool remote, Test *existing_test, QWidget *parent) :
    QWidget(parent),
    cancel_button(tr("C&ancel"), this),
    create_button(existing_test ? tr("&Update") : tr("&Create"), this),
    #ifndef Q_OS_WASM
    database_manager(database_manager),
    #endif
    dst_edit(this),
    name_edit(this),
    src_edit(this),
    status(this),
    title(existing_test ? tr("<b>Edit vocabulary list</b>") : tr("<b>Create a vocabulary list</b>")),
    test(existing_test)  // Store pointer to test being edited, or nullptr if creating
{
    lang_completer = new QCompleter(LANGUAGES, this);
    lang_completer->setCaseSensitivity(Qt::CaseInsensitive);
    src_edit.setCompleter(lang_completer);
    dst_edit.setCompleter(lang_completer);
    country_completer = new QCompleter(COUNTRIES, this);
    country_completer->setCaseSensitivity(Qt::CaseInsensitive);
    flag_edit.setCompleter(country_completer);
    
    // If editing, populate the fields with existing data
    if (existing_test) {
        name_edit.setText(existing_test->get_name());
        src_edit.setText(LANG_MAP[existing_test->get_src()]);
        dst_edit.setText(LANG_MAP[existing_test->get_dst()]);
        flag_edit.setText(COUNTRY_MAP[existing_test->get_flag()]);
    }
    
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
    
    void (AddListView::*slot)() = remote ?
        (existing_test ? &AddListView::update_online_list : &AddListView::add_online_list) :
        (existing_test ? &AddListView::update_offline_list : &AddListView::add_offline_list);
    connect(&create_button, &QPushButton::clicked, this, slot);
    
    connect(&cancel_button, SIGNAL(clicked()), this, SIGNAL(canceled()));
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
        Test *new_test = new Test(test_id, name_edit.text(), src_test, dst_test, test_flag, false);
        emit created(new_test);
    }else{
        status.setText(tr("<b>SQLite error: </b>") + error);
        status.show();
    }
}

void AddListView::update_offline_list()
{
    QString src_test(""), dst_test(""), test_flag(""), error("");

    if (!check_inputs(src_test, dst_test, test_flag, error))
    {
        status.setText(tr("<b>Error: </b>") + error);
        status.show();
        return;
    }
    
    if (!test) {
        status.setText(tr("<b>Error: </b>No list to update"));
        status.show();
        return;
    }
    
    database_manager->update_list(test->get_id(), name_edit.text(), src_test, dst_test, test_flag);
    error = database_manager->pop_last_error();
    if(error.isEmpty()){
        emit created(nullptr);
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
    const QUrl url = QUrl(NetworkReplyReader::api_url + "add_list.php");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");

    QNetworkReply* reply = NetworkReplyReader::nam->post(
                request, post_data.query().toUtf8());
    connect(reply, SIGNAL(finished()), this, SLOT(show_confirmation()));
}

void AddListView::update_online_list()
{
    QString src_test(""), dst_test(""), test_flag(""), error("");

    if (!check_inputs(src_test, dst_test, test_flag, error))
    {
        status.setText(tr("<b>Error: </b>") + error);
        status.show();
        return;
    }
    
    if (!test) {
        status.setText(tr("<b>Error: </b>No list to update"));
        status.show();
        return;
    }

    QUrlQuery post_data;
    post_data.addQueryItem("list_id", QString::number(test->get_id()));
    post_data.addQueryItem("name", name_edit.text());
    post_data.addQueryItem("src", src_test);
    post_data.addQueryItem("dst", dst_test);
    post_data.addQueryItem("flag", test_flag);
    const QUrl url = QUrl(NetworkReplyReader::api_url + "update_list.php");
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
