#include <QLabel>
#include <QDebug>
#include <QtNetwork>
#include <QBoxLayout>
#include <QLineEdit>
#include <QHeaderView>

#include "EditView.h"
#include "QuestionView.h"
#include "SearchView.h"
#include "string_utils.h"
#include "Parser.h"
#include "NetworkReplyReader.h"

SearchView::SearchView(Test& test, DatabaseManager *database_manager, bool modifiable, QWidget *parent) :
    QWidget(parent),
    search_bar(NULL),
    nam(),
    result(NULL),
    test(test),
    reply_list(),
    update_frame(NULL),
    modifiable(modifiable),
    database_manager(database_manager)
{
    QLayout* layout = new QVBoxLayout(this);
    search_bar = new QLineEdit(this);

    QPushButton* OK_button = new QPushButton(tr("OK"),this);
    OK_button->setIcon(QIcon::fromTheme("emblem-default", QIcon(getImgPath("emblem-default.png"))));

    QPushButton* back_button = new QPushButton(tr("Back"),this);
	back_button->setIcon(QIcon::fromTheme("process-stop",QIcon(getImgPath("process-stop.png"))));

   	layout->addWidget(search_bar);
    layout->addWidget(OK_button);
    layout->addWidget(back_button);

    connect(search_bar, SIGNAL(returnPressed()), this, SLOT(search()));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(search()));
    connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
    connect(back_button, SIGNAL(clicked()), this, SLOT(back()));
}

SearchView::~SearchView() {
    if(result)
        result->clear(); // Because this QTableWidget contains pointers to items with no parent.
}

void SearchView::search() {
	if (!test.is_remote_work()) {
        Parser p(test.get_src() + test.get_dst());

        // Offline
		QString search_str = ampersand_unescape(search_bar->text());
        read_reply(p.search(search_str, p.getFilein()));
	} else {

		// Standardization of search string
		QString search_str = ampersand_escape(search_bar->text());

		// Request to PHP file
		const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/search.php?lang=" + test.get_src() + test.get_dst() + "&string=" + search_str);
        nam.get(QNetworkRequest(url));
	}
}

void SearchView::read_reply(QNetworkReply* reply)
{
    // Store the lines of the reply in the "reply_list" attribute
    QString reply_string = reply->readAll();
    reply->deleteLater();
    read_reply(reply_string); // FIXME: Memory leak
}

void SearchView::read_reply(QString reply_string) {
    int nb_cols(10);
    reply_list = QStringList(reply_string.split('\n'));
	if(result){
		result->clear(); // Because this QTableWidget contains pointers to items with no parent.
		delete result;
        result = NULL;
	}
    int result_nb_rows(reply_list.count()/nb_cols), result_nb_cols(modifiable?nb_cols:(nb_cols-2));
    result = new QTableWidget(result_nb_rows, result_nb_cols, this);
	QStringList header_labels;
    if(modifiable)
        header_labels << "" << "";
    header_labels << tr("Word") << tr("Meaning") << tr("Nature") << tr("Comment") << tr("Example") << tr("Pronunciation") << tr("Score") << tr("Theme");
	result->setHorizontalHeaderLabels(header_labels);
	result->verticalHeader()->hide();
	layout()->addWidget(result);
    for(int i=0, col_ind=0; i<reply_list.count()-1; ++i){ // -1 because the last string is an empty string.
		QTableWidgetItem* item;
        if(modifiable && (col_ind==0 || col_ind==1)){
            QLabel* action_label = new QLabel(this);
            switch(col_ind){
            case 0:
                action_label->setPixmap(QIcon::fromTheme("accessories-text-editor", QIcon(getImgPath("accessories-text-editor.png"))).pixmap(16));
                action_label->setToolTip(tr("Edit"));
                break;
            case 1:
                action_label->setPixmap(QIcon::fromTheme("edit-delete", QIcon(getImgPath("edit-delete.png"))).pixmap(16));
                action_label->setToolTip(tr("Delete"));
                break;
            default:;
            }
            action_label->setAlignment(Qt::AlignCenter);
            result->setCellWidget(i/nb_cols, col_ind, action_label);
            col_ind = (col_ind+1)%result_nb_cols;
        }
        if (i%nb_cols != 0 && i%nb_cols != 6){ // We don't want to show the id or the theme id.
            if(col_ind == 9 && !test.is_remote_work()){ // Theme
                item = new QTableWidgetItem(ampersand_unescape(Parser::getTheme(reply_list.at(i-3).toInt())));
            }else{
                item = new QTableWidgetItem(ampersand_unescape(reply_list.at(i)));
            }
            result->setItem(i/nb_cols, col_ind, item);
            col_ind = (col_ind+1)%result_nb_cols;
        }
	}
	result->resizeColumnsToContents();
	disconnect(result);
    connect(result, SIGNAL(cellClicked(int,int)), this, SLOT(action(int, int)));
}


void SearchView::back()
{
	delete this;
}

void SearchView::action(int row, int col)
{
    int nb_cols(10);
    if(col == 0){
		// Remove everything
		result->disconnect();
		result->hide();

        // Create a new add frame
        QStringList default_values;
        for(int i=row*nb_cols; i<(row+1)*nb_cols; ++i){
            default_values << reply_list.at(i);
		}
        update_frame = new EditView(test, tr("<b>Edit a word entry</b>"), default_values, tr("Edit"), "update", tr("Word successfully edited!"), database_manager, this);
		layout()->addWidget(update_frame);
        connect(update_frame, SIGNAL(destroyed()), this, SLOT(refresh()));
    }else if(col == 1){
        result->disconnect();

		if (test.is_remote_work()) {

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        QUrl post_data;
#else
        QUrlQuery post_data;
#endif
        post_data.addQueryItem("lang", test.get_src() + test.get_dst());
        post_data.addQueryItem("id", reply_list.at(row*nb_cols));
        const QUrl url("http://neptilo.com/php/clemanglaise/delete.php");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        nam.setCookieJar(NetworkReplyReader::cookie_jar); // By default, nam takes ownership of the cookie jar.
        nam.cookieJar()->setParent(0); // Unset the cookie jar's parent so it is not deleted when nam is deleted, and can still be used by other NAMs.

        // Send the request
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        nam.post(request, post_data.encodedQuery());
#else
        nam.post(request, post_data.query(QUrl::FullyEncoded).toUtf8());
#endif
		} else {
            Parser p(test.get_src() + test.get_dst()); 
			int id_theme =  reply_list.at(row*nb_cols+6).toInt(); //id_theme
			int id = reply_list.at(row*nb_cols).toInt();
            p.deleteLineId( id, p.getFilein());
            p.deleteLineId(id, p.getScoreFile());
			if(id_theme>0) {
				QString theme_file = p.getSrcDst() + "/" + QString::number(id_theme) + "_" + p.getTheme(id_theme);
				p.deleteLineId(id, theme_file);
			}
		}

        refresh();
    }
}

void SearchView::refresh(){
	result->show();
    search();
}
