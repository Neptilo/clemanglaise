#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QListWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QWidget>
#include <QtNetwork>

#include "Test.h"
#include "EditView.h"

class SearchView : public QWidget
{
    Q_OBJECT
public:
    explicit SearchView(Test *test, DatabaseManager *database_manager, bool modifiable = false, QWidget *parent = 0);
    ~SearchView();
    void read_reply(QString reply_string = "");
    void read_reply_tags();//(QString reply_string = "");

private:
    QLineEdit *search_bar;
    QListWidget *tags; 
    QNetworkAccessManager nam;
    QTableWidget *result;
    Test *test;
    QStringList reply_list;
    QStringList reply_list_tag;
    EditView *update_frame;
    bool modifiable;
    DatabaseManager *database_manager;

public slots:
    void find_tags();
    void search();
    void read_reply(QNetworkReply * reply);
    void back();
    void action(int row, int col);
    void refresh();
};

#endif // SEARCHVIEW_H
