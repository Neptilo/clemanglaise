#ifndef TESTVIEW_H
#define TESTVIEW_H

#include <QPushButton>

#include "AnswerView.h"
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
    TestView(Test &test, DatabaseManager *database_manager, QString str_title = "", bool admin = false, QWidget *parent = 0);
    ~TestView();
    void read_reply(QString reply_string="");

private:
    QPushButton* add_button;
    EditView* add_view;
    QPushButton* add_theme_button;
    AddTagView* add_tag_view;
    bool admin;
    AnswerView* answer_view;
    QPushButton* back_button;
    DatabaseManager *database_manager;
    QPushButton * delete_list_button;
    QLayout* layout;
    QNetworkAccessManager* nam;
    QNetworkAccessManager nam_tags;
    QuestionView* question_view;
    QHash<QString, QString> word_data;
	QStringList reply_list_theme; 
    QNetworkRequest* request; // is a pointer because it cannot be initialized without a URL
    QPushButton* search_button;
    QPushButton* import_button;
    SearchView* search_view;
    QLabel status;
    Test test;
    QLabel* tags_label;
    QListWidget *tags;
    QLabel* title;
    QPushButton* update_button;
    EditView* update_view;
    AddTagView* update_theme_view;
    QStringList word_keys;

    void remove_widgets();

public slots:
    void init();
    void shrink();
    void update_request();
    void read_reply(QNetworkReply* reply);
    void read_delete_list_reply(QNetworkReply* reply);
    void read_reply_themes(QNetworkReply* reply);
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
};

#endif // TESTVIEW_H
