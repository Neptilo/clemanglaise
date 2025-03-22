#include <QBoxLayout>
#include <QDebug>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QtNetwork>

#include "CheckableItemDelegate.h"
#include "EditView.h"
#include "InterfaceParameters.h"
#include "NetworkReplyReader.h"
#include "QuestionView.h"
#include "SearchView.h"
#include "resource_utils.h"
#include "string_utils.h"

SearchView::SearchView(Test *test,
                       #ifndef Q_OS_WASM
                       DatabaseManager *database_manager,
                       #endif
                       bool modifiable,
                       QWidget *parent) :
    QWidget(parent),
    test(test),
    reply_list(),
    modifiable(modifiable),
    #ifndef Q_OS_WASM
    database_manager(database_manager),
    #endif
    search_bar(nullptr),
    OK_button(nullptr),
    result(nullptr),
    update_view(nullptr),
    status(nullptr)
{
    // has to be consistent with the actual content of reply_list
    word_keys << "id" << "word"  << "meaning" << "pronunciation" << "nature" << "comment" << "example"
              << "hint";
    if (!test->is_remote())
        word_keys << "score";
    word_keys << "tag_ids";

    layout = new QVBoxLayout(this);
    search_bar = new QLineEdit(this);
    tags_box = new CheckableComboBox(this);
    tags_box->setFixedHeight(InterfaceParameters::widget_unit);
    tags_box->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    find_tags();

    OK_button = new QPushButton(tr("OK"), this);
    OK_button->setIcon(getIcon("emblem-default"));
    OK_button->setFixedSize(2*InterfaceParameters::widget_unit, InterfaceParameters::widget_unit);
    OK_button->setIconSize(QSize(InterfaceParameters::widget_unit/2, InterfaceParameters::widget_unit/2));

    status = new QLabel(this);

    QBoxLayout *input_layout = new QHBoxLayout();
    input_layout->addWidget(search_bar);
    input_layout->addWidget(OK_button);
    layout->addLayout(input_layout);
    layout->addWidget(tags_box);
    layout->addWidget(status);
    status->hide();

    connect(search_bar, SIGNAL(returnPressed()), this, SLOT(refresh()));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(refresh()));
}

SearchView::~SearchView() {
    if(result)
        result->clear(); // Because this QTableWidget contains pointers to items with no parent.
}

void SearchView::find_tags() {
    if (test->is_remote()) {
        // Request to PHP file
        const QUrl url = QUrl(QString("https://neptilo.com/php/clemanglaise/find_used_tags.php?list_id=%1").arg(test->get_id()));
        QNetworkRequest request(url);
        QNetworkReply* reply = NetworkReplyReader::nam->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(read_used_tags_reply()));
#ifndef Q_OS_WASM
    } else {
        // Offline
        database_manager->find_used_tags(test->get_id(), reply_list_tag);
        read_reply_tags();
#endif
    }
}

void SearchView::search() {
    if(update_view)
        layout->removeWidget(update_view);
    status->hide();    
    // Standardization of search string
    QString search_str = ampersand_unescape(search_bar->text());
    if (test->is_remote()) {
        // If 0 is in the list, remove it and set untagged to true to say we also want to look for untagged words, else set it to false.
        bool untagged = selected_tags.removeOne(0);
        QStringList selected_tags_str;
        for(int i = 0; i < selected_tags.length(); ++i)
            selected_tags_str << QString::number(selected_tags.at(i));
        // Request to PHP file
        const QUrl url = QUrl(QString("https://neptilo.com/php/clemanglaise/search.php?list_id=%1&string=%2&tag_ids=%3&untagged=%4")
                              .arg(test->get_id())
                              .arg(search_str)
                              .arg(selected_tags_str.join(","))
                              .arg(untagged));
        QNetworkReply* reply = NetworkReplyReader::nam->get(QNetworkRequest(url));
        connect(reply, SIGNAL(finished()), this, SLOT(read_reply()));
#ifndef Q_OS_WASM
    } else {
        // Offline
        database_manager->search(test->get_id(), search_str, selected_tags, reply_list);
        read_reply("");
#endif
    }
}

void SearchView::read_used_tags_reply()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());

    // store the lines of the reply in the "reply_list_tag" attribute
    reply_list_tag = QString(reply->readAll().replace('\0', "")).split('\n');
    reply->deleteLater();
    read_reply_tags();
}

void SearchView::read_reply_tags() {
    // fill the tag combo box
    tags_box->disconnect();
    tags_box->clear();
    int l = reply_list_tag.size()/2;
    QStandardItemModel *model = new QStandardItemModel(l+1, 1);
    tags_box->set_title(tr("Filter by tags"));
    auto item = new QStandardItem(tr("Without any tags"));
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(Qt::Unchecked, Qt::CheckStateRole);
    model->setItem(0, 0, item);
    for (int i = 0; i < l; ++i) {
        item = new QStandardItem(reply_list_tag.at(2*i+1).trimmed());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        item->setData(QVariant(reply_list_tag.at(2*i).toInt()), Qt::UserRole);
        model->setItem(i+1, 0, item);
    }
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(update_selected_tags(QModelIndex,QModelIndex)));
    tags_box->setModel(model);
}

/**
 * handles the go back action according to the widgets it contains
 * @return true if has handled the action or false if it hasn't to pass the action over to the caller
 */
bool SearchView::go_back()
{
    if (update_view) {
        delete update_view;
        update_view = nullptr;
        search_bar->show();
        OK_button->show();
        tags_box->show();
        refresh();
        return true;
    } else
        return false;
}

void SearchView::update_selected_tags(QModelIndex top_left, QModelIndex)
{
    QMap<int, QVariant> item_data = tags_box->model()->itemData(top_left);
    int map_size = item_data.size();
    int variant = item_data[Qt::UserRole].toInt();
    switch (item_data[Qt::CheckStateRole].toInt()) {
    case Qt::Checked:
        if (map_size > 2)
            selected_tags << variant ;
        break;
    case Qt::Unchecked:
        if (map_size > 2)
            selected_tags.removeOne(variant);
        break;
    default:
        qDebug() << tr("Wrong check state value");
        break;
    }
}

void SearchView::read_reply()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(status_code.toInt() != 200){
        status->setText(reply->readAll().replace('\0', ""));
        status->show();
        return;
    }
    status->hide();
    // Store the lines of the reply in the "reply_list" attribute
    QString reply_string = reply->readAll().replace('\0', "");
    reply->deleteLater();
    read_reply(reply_string); // FIXME: Memory leak
}

void SearchView::read_delete_reply()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(status_code.toInt() != 200){
        status->setText(reply->readAll().replace('\0', ""));
        status->show();
        return;
    }
    status->hide();
    refresh();
}

void SearchView::read_reply(QString reply_string) {
    int nb_cols = word_keys.size(); // the number of columns in the SQL query
    if (test->is_remote())
        reply_list = reply_string.split('\n');
    if(result){
        result->clear(); // Because this QTableWidget contains pointers to items with no parent.
        result->deleteLater();
        result = nullptr;
    }
    QStringList header_labels;
    if(modifiable)
        header_labels << "" << "";
    header_labels << tr("Word") <<  tr("Meaning") << tr("Pronunciation") << tr("Nature") << tr("Comment")
                  << tr("Example")  << tr("Hint");
    if (!test->is_remote())
        header_labels << tr("Score");
    header_labels << tr("Tags");
    int result_nb_rows(reply_list.count()/nb_cols), result_nb_cols(header_labels.size()); // the number of rows and columns in the displayed table
    result = new QTableWidget(result_nb_rows, result_nb_cols, this);
    result->setHorizontalHeaderLabels(header_labels);
    result->verticalHeader()->hide();
    layout->addWidget(result);
    for(int i=0, col_ind=0; i<reply_list.count()-1; ++i){ // -1 because the last string is an empty string.
        QLabel* item;
        if(modifiable && (col_ind==0 || col_ind==1)){
            item = new QLabel(this);
            switch(col_ind){
            case 0:
                item->setPixmap(getIcon("accessories-text-editor").pixmap(16));
                item->setToolTip(tr("Edit"));
                break;
            case 1:
                item->setPixmap(getIcon("edit-delete").pixmap(16));
                item->setToolTip(tr("Delete"));
                break;
            default:;
            }
            item->setAlignment(Qt::AlignCenter);
            result->setCellWidget(i/nb_cols, col_ind, item);
            col_ind = (col_ind+1)%result_nb_cols;
        }
        if (i%nb_cols != word_keys.indexOf("id")){ // We don't want to show the id.
            if (i%nb_cols == word_keys.indexOf("tag_ids")) {
                QComboBox* tags_box = new QComboBox();
                QStringList tag_ids_str = reply_list.at(i).split(", ", Qt::SkipEmptyParts);
                for (int j = 0, count = tag_ids_str.size(); j < count; ++j) {
                    int tag_id_ind = reply_list_tag.indexOf(tag_ids_str.at(j));
                    if (tag_id_ind >= 0) {
                        QString tag_name = reply_list_tag.at(tag_id_ind+1);
                        tags_box->addItem(tag_name);
                    } else {
                        tags_box->addItem("?"); // should not happen
                    }
                }
                result->setCellWidget(i/nb_cols, col_ind, tags_box);
            } else {
                item = new QLabel(ampersand_unescape(reply_list.at(i)), this);
                item->setTextFormat(Qt::RichText);
                result->setCellWidget(i/nb_cols, col_ind, item);
            }
            col_ind = (col_ind+1)%result_nb_cols;
        }
    }
    result->resizeColumnsToContents();
    result->resizeRowsToContents();
    connect(result, SIGNAL(cellClicked(int,int)), this, SLOT(action(int, int)));
}

void SearchView::action(int row, int col)
{
    int nb_cols = word_keys.size();

    if(col == 0){
        // Remove everything
        result->hide();
        status->hide();

        // Create a new add frame
        QHash<QString, QString> default_values;
        for(int i = 0; i < nb_cols; ++i)
            default_values[word_keys.at(i)] = reply_list.at(i+row*nb_cols);
        update_view = new EditView(test,
                                   tr("<b>Edit a word entry</b>"),
                                   default_values,
                                   tr("Edit"),
                                   "update_word",
                                   tr("Word successfully edited!"),
                           #ifndef Q_OS_WASM
                                   database_manager,
                           #endif
                                   this);
        search_bar->hide();
        OK_button->hide();
        tags_box->hide();
        layout->addWidget(update_view);
    }else if(col == 1){
        QMessageBox::StandardButton ret = QMessageBox::question(
            this,
            tr("Confirm deletion"),
            tr("Are you sure you want to delete the entry \"<b>%1</b>\"?")
                    .arg(reply_list.at(row*nb_cols+word_keys.indexOf("word"))));
        if(ret == QMessageBox::Yes){
            if (test->is_remote()) {
                QUrlQuery post_data;
                post_data.addQueryItem("id", reply_list.at(row*nb_cols));
                const QUrl url("https://neptilo.com/php/clemanglaise/delete_word.php");
                QNetworkRequest request(url);
                request.setHeader(QNetworkRequest::ContentTypeHeader,
                                  "application/x-www-form-urlencoded");

                // Send the request
                QNetworkReply* reply = NetworkReplyReader::nam->post(
                            request, post_data.query().toUtf8());
                connect(reply, SIGNAL(finished()), this, SLOT(read_delete_reply()));
#ifndef Q_OS_WASM
            } else {
                int id = reply_list.at(row*nb_cols).toInt();
                if (!database_manager->delete_word(id)) {
                    status->setText(database_manager->pop_last_error());
                    status->show();
                }
                refresh();
#endif
            }
        }
    }
}

void SearchView::refresh(){
    find_tags();
    if (result)
        result->show();
    search();
}
