#ifndef TESTFRAME_H
#define TESTFRAME_H

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
    QStringList* reply_list;
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
	Parser* parser;

public slots:
    void init();
    void read_reply(QNetworkReply* reply);
    void validate_question();
    void validate_answer();
    void add_theme();
    void add_word();
    void update_word();
    void search();
	void go_back();
};

#endif // TESTFRAME_H
