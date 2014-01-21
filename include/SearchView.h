#ifndef SEARCHFRAME_H
#define SEARCHFRAME_H

#include <QWidget>
#include <QtNetwork>
#include <QTableWidget>
#include <QPushButton>

#include "Test.h"
#include "EditView.h"

class SearchView : public QWidget
{
    Q_OBJECT
public:
    explicit SearchView(Test &test, DatabaseManager *database_manager, bool modifiable = false, QWidget *parent = 0);
    ~SearchView();
    void read_reply(QString reply_string);

private:
    QLineEdit* search_bar;
    QNetworkAccessManager nam;
    QTableWidget* result;
    Test test;
    QStringList reply_list;
    EditView* update_frame;
    bool modifiable;
    DatabaseManager *database_manager;

public slots:
    void search();
    void read_reply(QNetworkReply * reply);
    void back();
    void action(int row, int col);
    void refresh();
};

#endif // SEARCHFRAME_H
