#ifndef TESTFRAME_H
#define TESTFRAME_H

#include "questionframe.h"
#include "answerframe.h"
#include "editframe.h"
#include "searchframe.h"
#include "test.h"
#include "Parser.h" 

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
static const std::string slash="\\";
#else
static const std::string slash="/";
#endif

class TestFrame : public QWidget{
    Q_OBJECT
    
public:
    TestFrame(Test &test, QWidget *parent = 0);
    ~TestFrame();

private:
    QuestionFrame* question_frame;
    AnswerFrame* answer_frame;
    EditFrame* add_frame;
    EditFrame* update_frame;
    SearchFrame* search_frame;
    QNetworkRequest* request;
    QNetworkAccessManager* nam;
    QStringList* reply_list;
    QLayout* layout;
	QLabel* label;
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
    void add_word();
    void update_word();
    void search();
};

#endif // TESTFRAME_H
