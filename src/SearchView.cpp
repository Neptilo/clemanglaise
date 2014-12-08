#include <QBoxLayout>
#include <QDebug>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QRegExp>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QtNetwork>

#include "CheckableItemDelegate.h"
#include "EditView.h"
#include "InterfaceParameters.h"
#include "NetworkReplyReader.h"
#include "QuestionView.h"
#include "SearchView.h"
#include "string_utils.h"

SearchView::SearchView(Test *test, DatabaseManager *database_manager, bool modifiable, QWidget *parent) :
    QWidget(parent),
    nam(),
    tag_nam(),
    test(test),
    reply_list(),
    modifiable(modifiable),
    database_manager(database_manager),
    search_bar(NULL),
    OK_button(NULL),
    result(NULL),
    update_view(NULL),
    status(NULL)
{
    // has to be consistent with the actual content of reply_list
    word_keys << "id" << "word"  << "meaning" << "pronunciation" << "nature" << "comment" << "example" << "hint" << "score" << "tag_ids";

    layout = new QVBoxLayout(this);
    search_bar = new QLineEdit(this);
    tags_box = new QComboBox(this);
    tags_box->setFixedHeight(InterfaceParameters::widget_unit);
    CheckableItemDelegate *delegate = new CheckableItemDelegate(this);
    tags_box->setItemDelegate(delegate);
    tags_box->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    find_tags();

    OK_button = new QPushButton(tr("OK"), this);
    OK_button->setIcon(QIcon::fromTheme("emblem-default", QIcon(getImgPath("emblem-default.png"))));
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

    connect(search_bar, SIGNAL(returnPressed()), this, SLOT(search()));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(search()));
    connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
    connect(&tag_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply_tags(QNetworkReply*)));
}

SearchView::~SearchView() {
    if(result)
        result->clear(); // Because this QTableWidget contains pointers to items with no parent.
}

void SearchView::find_tags() {
    if (!test->is_remote()) {
		// Offline
        database_manager->find_used_tags(test->get_id(), reply_list_tag);
		read_reply_tags();
	} else { 
        // Request to PHP file
        const QUrl url = QUrl(QString("http://neptilo.com/php/clemanglaise/find_used_tags.php?list_id=%1").arg(test->get_id()));
        QNetworkRequest request(url);
        tag_nam.get(request);
	}
}



void SearchView::search() {
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
        const QUrl url = QUrl(QString("http://neptilo.com/php/clemanglaise/search.php?list_id=%1&string=%2&tag_ids=%3&untagged=%4")
                              .arg(test->get_id())
                              .arg(search_str)
                              .arg(selected_tags_str.join(","))
                              .arg(untagged));
        nam.get(QNetworkRequest(url));
    } else {
        // Offline
        database_manager->search(test->get_id(), search_str, selected_tags, reply_list);
        read_reply();
    }
}

void SearchView::read_reply_tags(QNetworkReply* reply)
{
    // store the lines of the reply in the "reply_list_tag" attribute
    reply_list_tag = QString(reply->readAll()).split('\n');
    reply->deleteLater();
    read_reply_tags();
}

void SearchView::read_reply_tags() {
    // fill the tag combo box
    tags_box->disconnect();
    tags_box->clear();
    int l = reply_list_tag.size()/2;
    QStandardItemModel *model = new QStandardItemModel(l+2, 1);
    QStandardItem* item = new QStandardItem(tr("Filter by tags"));
    model->setItem(0, 0, item);
    item = new QStandardItem(tr("Without any tags"));
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(Qt::Unchecked, Qt::CheckStateRole);
    model->setItem(1, 0, item);
    for (int i = 0; i < l; ++i) {
        item = new QStandardItem(reply_list_tag.at(2*i+1).trimmed());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        item->setData(QVariant(reply_list_tag.at(2*i).toInt()), Qt::UserRole);
        model->setItem(i+2, 0, item);
    }
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(update_selected_tags(QModelIndex,QModelIndex)));
    tags_box->setModel(model);
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

void SearchView::read_reply(QNetworkReply* reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(status_code.toInt() != 200){
        status->setText(reply->readAll());
        status->show();
        return;
    }
    status->hide();
    // Store the lines of the reply in the "reply_list" attribute
    QString reply_string = reply->readAll().replace('\0', "");
    reply->deleteLater();
    read_reply(reply_string); // FIXME: Memory leak
}

void SearchView::read_delete_reply(QNetworkReply* reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(status_code.toInt() != 200){
        status->setText(reply->readAll());
        status->show();
        return;
    }
    status->hide();
    nam.disconnect();
    connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
    refresh();
}

void SearchView::read_reply(QString reply_string) {
    int nb_cols = word_keys.size(); // the number of columns in the SQL query
    if (test->is_remote())
        reply_list = reply_string.split('\n');
    if(result){
        result->clear(); // Because this QTableWidget contains pointers to items with no parent.
        result->deleteLater();
        result = NULL;
    }
    QStringList header_labels;
    if(modifiable)
        header_labels << "" << "";
    header_labels << tr("Word") <<  tr("Meaning") << tr("Pronunciation") << tr("Nature") << tr("Comment") << tr("Example")  << tr("Hint") << tr("Score") << tr("Tags");
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
                item->setPixmap(QIcon::fromTheme("accessories-text-editor", QIcon(getImgPath("accessories-text-editor.png"))).pixmap(16));
                item->setToolTip(tr("Edit"));
                break;
            case 1:
                item->setPixmap(QIcon::fromTheme("edit-delete", QIcon(getImgPath("edit-delete.png"))).pixmap(16));
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
                QStringList tag_ids_str = reply_list.at(i).split(", ", QString::SkipEmptyParts);
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
    disconnect(result);
    connect(result, SIGNAL(cellClicked(int,int)), this, SLOT(action(int, int)));
}

void SearchView::action(int row, int col)
{
    int nb_cols = word_keys.size();

    if(col == 0){
        // Remove everything
        result->disconnect();
        result->hide();
        status->hide();

        // Create a new add frame
        QHash<QString, QString> default_values;
        for(int i = 0; i < nb_cols; ++i)
            default_values[word_keys.at(i)] = reply_list.at(i+row*nb_cols);
        update_view = new EditView(test, tr("<b>Edit a word entry</b>"), default_values, tr("Edit"), "update_word", tr("Word successfully edited!"), database_manager, this);
        search_bar->hide();
        OK_button->hide();
        tags_box->hide();
        layout->addWidget(update_view);
        connect(update_view, SIGNAL(destroyed()), this, SLOT(refresh()));
    }else if(col == 1){
        QMessageBox::StandardButton ret = QMessageBox::question(
                    this,
                    tr("Confirm deletion"),
                    tr("Are you sure you want to delete the entry \"<b>%1</b>\"?").arg(reply_list.at(row*nb_cols+word_keys.indexOf("word")))
            #if QT_VERSION < QT_VERSION_CHECK(5,0,0)
                    , QMessageBox::Yes | QMessageBox::No
            #endif
                    );
        if(ret == QMessageBox::Yes){
            result->disconnect();

            if (test->is_remote()) {

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
                QUrl post_data;
#else
                QUrlQuery post_data;
#endif
                post_data.addQueryItem("id", reply_list.at(row*nb_cols));
                const QUrl url("http://neptilo.com/php/clemanglaise/delete_word.php");
                QNetworkRequest request(url);
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
                nam.setCookieJar(NetworkReplyReader::cookie_jar); // By default, nam takes ownership of the cookie jar.
                nam.cookieJar()->setParent(0); // Unset the cookie jar's parent so it is not deleted when nam is deleted, and can still be used by other NAMs.
                nam.disconnect();
                connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_delete_reply(QNetworkReply*)));

                // Send the request
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
                nam.post(request, post_data.encodedQuery());
#else
                nam.post(request, post_data.query(QUrl::FullyEncoded).toUtf8());
#endif
            } else {
                int id = reply_list.at(row*nb_cols).toInt();
                if (!database_manager->delete_word(id)) {
                    status->setText(database_manager->pop_last_error());
                    status->show();
                }
                refresh();
            }
        }
    }
}

void SearchView::refresh(){
    find_tags();
    result->show();
    search();
}
