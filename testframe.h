#ifndef TESTFRAME_H
#define TESTFRAME_H
#include <QToolButton>
#include "questionframe.h"
#include "answerframe.h"
#include "themeframe.h"
#include "editframe.h"
#include "searchframe.h"
#include "test.h"
#include "Parser.h" 

#if defined(Q_WS_WIN)
const QString ENDL="\\r\\n";
#elif defined(Q_WS_MACX)
const QString ENDL="\\r";
#else
const QString ENDL="\\n";
#endif

class TestFrame : public QWidget{
    Q_OBJECT
    
public:
    TestFrame(Test &test, QString str_title="", QWidget *parent = 0);
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
    QNetworkRequest* request;
    QNetworkAccessManager* nam;
    QNetworkAccessManager nam_themes;
    QStringList* reply_list;
    QLayout* layout;
	QLabel* theme;
	QLabel* title;
    QToolButton* back_button;
    QToolButton* add_theme_button;
    QToolButton* add_button;
    QToolButton* search_button;
    QToolButton* update_button;
    Test test;
	QComboBox *themes;
	Parser* parser;

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
