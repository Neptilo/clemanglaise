#ifndef TESTVIEW_H
#define TESTVIEW_H

#include <QPushButton>
#include <QToolBar>

#include "AnswerView.h"
#include "CheckableComboBox.h"
#include "DatabaseManager.h"
#include "EditView.h"
#include "QuestionView.h"
#include "SearchView.h"
#include "Test.h"
#include "AddTagView.h"

#if defined(Q_OS_WIN)
const QString ENDL="\\r\\n";
#elif defined(Q_OS_MAC)
const QString ENDL="\\r";
#else
const QString ENDL="\\n";
#endif

class TestView : public QWidget{
    Q_OBJECT
    
public:
    TestView(Test &test, DatabaseManager *database_manager, bool admin = false, QWidget *parent = nullptr);
    ~TestView();

    void read_reply(QString reply_string);

private:
    EditView *add_view;
    AddTagView *add_tag_view;
    bool admin;
    AnswerView *answer_view;
    DatabaseManager *database_manager;
    QuestionView *question_view;
    QHash<QString, QString> word_data;
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
    void import_word();
    void import_list();

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void delete_word();
    void update_selected_tags(QModelIndex top_left, QModelIndex bottom_right);
};

#endif // TESTVIEW_H
