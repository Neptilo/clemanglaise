#ifndef TESTFRAME_H
#define TESTFRAME_H

#include <QPushButton>

#include "AnswerView.h"
#include "DatabaseManager.h"
#include "EditView.h"
#include "QuestionView.h"
#include "SearchView.h"
#include "Test.h"
#include "ThemeView.h"

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
    void read_reply(QString reply_string);

private:
    QPushButton* add_button;
    EditView* add_frame;
    QPushButton* add_theme_button;
    ThemeView* add_theme_frame;
    bool admin;
    AnswerView* answer_frame;
    QPushButton* back_button;
    DatabaseManager *database_manager;
    QLayout* layout;
    QNetworkAccessManager* nam;
    QNetworkAccessManager nam_themes;
    QuestionView* question_frame;
    QStringList reply_list;
    QNetworkRequest* request; // is a pointer because it cannot be initialized without a URL
    QPushButton* search_button;
    SearchView* search_frame;
    QLabel status;
    Test test;
    QLabel* theme;
    QComboBox *themes;
    QLabel* title;
    QPushButton* update_button;
    EditView* update_frame;
    ThemeView* update_theme_frame;

    void remove_widgets();

public slots:
    void init();
    void update_request();
    void read_reply(QNetworkReply* reply);
    void read_reply_themes(QNetworkReply* reply);
    void validate_question();
    void validate_answer();
	void find_themes(); 
    void add_theme();
    void add_word();
    void update_word();
    void search();
	void go_back();
    void update_question(int);
};

#endif // TESTFRAME_H
