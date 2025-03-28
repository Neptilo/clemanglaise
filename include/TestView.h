#ifndef TESTVIEW_H
#define TESTVIEW_H

#include <QPushButton>
#include <QToolBar>
#include <queue>

#include "AnswerView.h"
#include "CheckableComboBox.h"
#ifndef Q_OS_WASM
#include "DatabaseManager.h"
#endif
#include "EditView.h"
#include "QuestionView.h"
#include "SearchView.h"
#include "Test.h"
#include "AddTagView.h"

class QNetworkRequest;

class TestView : public QWidget{
    Q_OBJECT
    
public:
    TestView(Test &test,
         #ifndef Q_OS_WASM
             DatabaseManager *database_manager,
         #endif
             bool admin = false, QWidget *parent = nullptr);
    ~TestView();

    void read_reply(QString reply_string);

private:
    EditView *add_view;
    AddTagView *add_tag_view;
    bool admin;
    AnswerView *answer_view;
#ifndef Q_OS_WASM
    DatabaseManager *database_manager;
#endif
    QuestionView *question_view;
    // store a small cache of the few next words to ask,
    // to avoid asking the same word too soon after it's been asked
    // In online mode, only contains one entry.
    std::queue<QHash<QString, QString>> word_data_queue;
    QSet<int> word_ids_in_queue; // ids of the words in word_data_queue, for fast access
    QStringList tag_reply_list;
    QNetworkRequest *request; // is a pointer because it cannot be initialized without a URL
    SearchView *search_view;
    QLabel status;
    Test test;
    QLabel *title;
    QPushButton *update_button;
    EditView *update_view;
    QStringList word_keys;
    QList<int> selected_tags;
    int list_size_limit;
    int asked_in_this_session = 0;

    // GUI
    QBoxLayout *layout;
    QBoxLayout *header_layout;
    QBoxLayout *tool_bar_layout;
    QToolButton *back_button;

    QToolButton *add_button;
    QToolButton *search_button;
    QToolButton *import_button;
    QToolButton *delete_button;
    CheckableComboBox *tags_box;

    // actions
    QAction *back_action;
    QAction *add_action;
    QAction *add_tag_action;
    QAction *search_action;
    QAction *import_action;
    QAction *delete_action;

    void remove_widgets();
    void create_actions();
    void create_interface();
    void init_button(QToolButton *button);

public slots:
    void init();
    void shrink();
    void update_request();
    void read_reply();
    void read_delete_list_reply();
    void read_reply_tags();
    void validate_question();
    void validate_answer();
    void find_tags();
    void add_tag();
    void add_word();
    void update_word();
    void search();
    void go_back();
    void update_question();
    void update_question(int);
    void delete_list();
#ifndef Q_OS_WASM
    void import_word();
    void import_list();
#endif

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void delete_word();
    void update_selected_tags(QModelIndex top_left, QModelIndex bottom_right);
};

#endif // TESTVIEW_H
