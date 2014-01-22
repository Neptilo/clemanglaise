#ifndef TESTFRAME_H
#define TESTFRAME_H

#include <QPushButton>

#include "QuestionView.h"
#include "AnswerView.h"
#include "ThemeView.h"
#include "EditView.h"
#include "SearchView.h"
#include "Test.h"
#include "DatabaseManager.h"

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
    QuestionView* question_frame;
    AnswerView* answer_frame;
    ThemeView* add_theme_frame;
    ThemeView* update_theme_frame;
    EditView* add_frame;
    EditView* update_frame;
    SearchView* search_frame;
    QNetworkRequest* request; // is a pointer because it cannot be initialized without a URL
    QNetworkAccessManager* nam;
    QNetworkAccessManager nam_themes;
    QStringList reply_list;
    QLayout* layout;
	QLabel* theme;
	QLabel* title;
    QPushButton* back_button;
    QPushButton* add_theme_button;
    QPushButton* add_button;
    QPushButton* search_button;
    QPushButton* update_button;
    Test test;
	QComboBox *themes;
    bool admin;
    DatabaseManager *database_manager;
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
