#include <QLabel>
#include <QtNetwork>
#include <QBoxLayout>
#include <QLineEdit>
#include <QHeaderView>

#include "editframe.h"
#include "questionframe.h"
#include "searchframe.h"
#include "string_utils.h"
#include "Parser.h"

SearchFrame::SearchFrame(Test& test, QWidget *parent) :
    QWidget(parent),
    nam(),
    result(NULL),
    test(test),
    reply_list()
{
    QLayout* layout = new QVBoxLayout(this);
    search_bar = new QLineEdit(this);

    QPushButton* OK_button = new QPushButton(tr("OK"),this);
	OK_button->setIcon(QIcon("img/ok.png"));

    QPushButton* back_button = new QPushButton(tr("Cancel"),this);
	back_button->setIcon(QIcon::fromTheme("process-stop",QIcon("img/cancel.png")));

   	layout->addWidget(search_bar);
    layout->addWidget(OK_button);
    layout->addWidget(back_button);

    connect(search_bar, SIGNAL(returnPressed()), this, SLOT(search()));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(search()));
    connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
    connect(back_button, SIGNAL(clicked()), this, SLOT(back()));
}

SearchFrame::~SearchFrame() {
    if(result){
        disconnect(result);
        result->clear(); // Because this QTableWidget contains pointers to items with no parent.
    }
}

void SearchFrame::search() {
	if (!test.isRemoteWork()) {
		Parser* p = new Parser(test.getSrc() + test.getDst());
		//offline
		QString search_str = ampersand_unescape(search_bar->text());
		read_reply(p->search(search_str, p->getFilein()));
	} else {

		// Standardization of search string
		QString search_str = ampersand_escape(search_bar->text());

		// Request to PHP file
		const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/search.php?lang=" + test.getSrc() + test.getDst() + "&string=" + search_str);
		request = new QNetworkRequest(url);

		nam.get(*request);
	}
}

void SearchFrame::read_reply(QNetworkReply* reply)
{
    // Store the lines of the reply in the "reply_list" attribute
    QString reply_string = reply->readAll();
    reply->deleteLater();
	read_reply(reply_string);
}

void SearchFrame::read_reply(QString reply_string) {
    int nb_cols(10);
	reply_list = new QStringList(reply_string.split('\n'));
	if(result){
        disconnect(result);
		result->clear(); // Because this QTableWidget contains pointers to items with no parent.
		delete result;
	}
    int result_nb_rows(reply_list->count()/nb_cols), result_nb_cols(nb_cols-1);
    result = new QTableWidget(result_nb_rows, result_nb_cols, this);
	QStringList header_labels;
    header_labels << "" << tr("Word") << tr("Meaning") << tr("Nature") << tr("Comment") << tr("Example") << tr("Pronunciation") << tr("Score") << tr("Theme");
	result->setHorizontalHeaderLabels(header_labels);
	result->verticalHeader()->hide();
	layout()->addWidget(result);
    for(int i=0, col_ind=0; i<reply_list->count()-1; ++i){ // -1 because the last string is an empty string.
		QTableWidgetItem* item;
        if(col_ind == 0){
            QLabel* edit_label = new QLabel(this);
            edit_label->setPixmap(QIcon::fromTheme("accessories-text-editor", QIcon("img/textedit.png")).pixmap(16));
            edit_label->setAlignment(Qt::AlignCenter);
            edit_label->setToolTip(tr("Edit"));
            result->setCellWidget(i/nb_cols, col_ind, edit_label);
            col_ind = (col_ind+1)%result_nb_cols;
        } else if (i%nb_cols != 6){ // We don't want to show the theme id.
            if(col_ind == 8 && !test.isRemoteWork()){ // Theme
                item = new QTableWidgetItem(ampersand_unescape(Parser::getTheme(reply_list->at(i-3).toInt())));
            }else{
                item = new QTableWidgetItem(ampersand_unescape(reply_list->at(i)));
            }
            result->setItem(i/nb_cols, col_ind, item);
            col_ind = (col_ind+1)%result_nb_cols;
		}
	}
	result->resizeColumnsToContents();
	disconnect(result);
	connect(result, SIGNAL(cellClicked(int,int)), this, SLOT(edit(int, int)));
}


void SearchFrame::back()
{
	delete this;
}

void SearchFrame::edit(int row, int col)
{
	int nbcols(10);
	if(col == 0){

		// Remove everything
		result->disconnect();
		result->hide();

		// Create a new add frame
		QStringList default_values;
		for(int i=row*nbcols; i<(row+1)*nbcols; ++i){
			default_values << reply_list->at(i);
		}
		update_frame = new EditFrame(test, tr("<b>Edit a word entry</b>"), default_values, tr("Edit"), "update", tr("Word successfully edited!"), this);
		layout()->addWidget(update_frame);
        connect(update_frame, SIGNAL(destroyed()), this, SLOT(refresh()));
	}
}

void SearchFrame::refresh(){
	result->show();
    search();
}
