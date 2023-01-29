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
    explicit SearchView(Test *test,
                    #ifndef Q_OS_WASM
                        DatabaseManager *database_manager,
                    #endif
                        bool modifiable = false,
                        QWidget *parent = nullptr);
    ~SearchView();

    void read_reply(QString reply_string);
    void read_reply_tags();//(QString reply_string = "");
    bool go_back();

private:
    void search();

    Test *test;
    QStringList reply_list;
    QStringList reply_list_tag;
    bool modifiable;
#ifndef Q_OS_WASM
    DatabaseManager *database_manager;
#endif
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
    void read_reply();
    void action(int row, int col);

private slots:
    void read_used_tags_reply();
    void read_delete_reply();
    void update_selected_tags(QModelIndex top_left, QModelIndex bottom_right);
    void refresh();
};

#endif // SEARCHVIEW_H
