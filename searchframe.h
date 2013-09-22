#ifndef SEARCHFRAME_H
#define SEARCHFRAME_H

#include <QWidget>
#include <QtNetwork>
#include <QTableWidget>
#include <QPushButton>

#include "test.h"
#include "editframe.h"

class SearchFrame : public QWidget
{
    Q_OBJECT
public:
    explicit SearchFrame(Test &test, QWidget *parent = 0);
    ~SearchFrame();
    void read_reply(QString reply_string);

private:
    QLineEdit* search_bar;
    QNetworkRequest* request;
    QNetworkAccessManager nam;
    QTableWidget* result;
    Test test;
    QStringList* reply_list;
    EditFrame* update_frame;

public slots:
    void search();
    void read_reply(QNetworkReply * reply);
    void back();
    void edit(int row, int col);
    void show_widgets();
};

#endif // SEARCHFRAME_H
