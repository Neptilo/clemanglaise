#ifndef TESTFRAME_H
#define TESTFRAME_H
#include <QPushButton>
#include "questionframe.h"
#include "answerframe.h"
#include "themeframe.h"
#include "editframe.h"
#include "searchframe.h"
#include "test.h"
#include "Parser.h" 

#if defined(Q_OS_WIN)
const QString ENDL="\\r\\n";
#elif defined(Q_OS_MAC)
const QString ENDL="\\r";
#else
const QString ENDL="\\n";
#endif

class TestFrame : public QWidget{
    Q_OBJECT
    
public:
    TestFrame(Test &test, QString str_title="", bool admin = false, QWidget *parent = 0);
    ~TestFrame();
    void read_reply(QString reply_string);

private:
    QuestionFrame* question_frame;
    AnswerFrame* answer_frame;
    ThemeFrame* add_theme_frame;
    ThemeFrame* update_theme_frame;
    EditFrame* add_frame;
    EditFrame* update_frame;
    SearchFrame* search_frame;
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
    Parser* parser; // pointer because for offline work it doesn't need to be defined.
    bool admin;
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
