#include <QLabel>
#include <QtNetwork>
#include <QBoxLayout>
#include <QLineEdit>
#include <QHeaderView>
#include "searchframe.h"
#include "string_utils.h"
#include "Parser.h"

SearchFrame::SearchFrame(Test& test, QWidget *parent) :
    QWidget(parent),
    nam(),
    result(NULL),
    test(test)
{
    QLayout* layout = new QVBoxLayout(this);
    search_bar = new QLineEdit(this);
    QPushButton* OK_button = new QPushButton(tr("OK"), this);
    QPushButton* back_button = new QPushButton(tr("Back to test"), this);
   	layout->addWidget(search_bar);
    layout->addWidget(OK_button);
    layout->addWidget(back_button);

    connect(search_bar, SIGNAL(returnPressed()), this, SLOT(search()));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(search()));
    connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
    connect(back_button, SIGNAL(clicked()), this, SLOT(back()));
}

SearchFrame::~SearchFrame() {
    result->clear(); // Because this QTableWidget contains pointers to items with no parent.
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
    QStringList reply_list(reply_string.split('\n'));
    if(result){
        result->clear(); // Because this QTableWidget contains pointers to items with no parent.
        delete result;
    }
    result = new QTableWidget(reply_list.count()/7, 6, this);
    QStringList header_labels;
    header_labels << tr("Word") << tr("Meaning") << tr("Nature") << tr("Comment") << tr("Example") << tr("Pronunciation");
    result->setHorizontalHeaderLabels(header_labels);
    result->verticalHeader()->hide();
    layout()->addWidget(result);
    for(int i=0; i<reply_list.count()-1; ++i){ // -1 because the last string is an empty string.
        if(i%7 != 0){ // We don't want to show the ID.
            QTableWidgetItem* item = new QTableWidgetItem(ampersand_unescape(reply_list.at(i))); // Need to delete this later
            result->setItem(i/7, i%7-1, item);
        }
    }
    result->resizeColumnsToContents();
}

void SearchFrame::back()
{
    delete this;
}
