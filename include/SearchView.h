#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QListWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QWidget>
#include <QtNetwork>

#include "CheckableComboBox.h"
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
    bool go_back();
    void refresh();

private:
    QNetworkAccessManager nam;
    QNetworkAccessManager tag_nam;
    Test *test;
    QStringList reply_list;
    QStringList reply_list_tag;
    bool modifiable;
    DatabaseManager *database_manager;
    QStringList word_keys;
    QList<int> selected_tags;

    // GUI
    QBoxLayout *layout;
    QLineEdit *search_bar;
    QPushButton *OK_button;
    QTableWidget *result;
    EditView *update_view;
    QLabel *status;
    CheckableComboBox *tags_box;

public slots:
    void find_tags();
    void search();
    void read_reply(QNetworkReply * reply);
    void action(int row, int col);

private slots:
    void read_reply_tags(QNetworkReply *reply);
    void read_delete_reply(QNetworkReply *reply);
    void update_selected_tags(QModelIndex top_left, QModelIndex bottom_right);
};

#endif // SEARCHVIEW_H
