#ifndef TESTFRAME_H
#define TESTFRAME_H

#include "questionframe.h"
#include "answerframe.h"
#include "addframe.h"
#include "test.h"

class TestFrame : public QWidget{
    Q_OBJECT
    
public:
    TestFrame(Test &test, QWidget *parent = 0);
    ~TestFrame();

private:
    QuestionFrame* question_frame;
    AnswerFrame* answer_frame;
    AddFrame* add_frame;
    QNetworkRequest* request;
    QNetworkAccessManager* nam;
    QStringList* reply_list;
    QLayout* layout;
    QPushButton* add_button;
    Test test;

public slots:
    void init();
    void read_reply(QNetworkReply* reply);
    void validate_question();
    void validate_answer();
    void add_word();
};

#endif // TESTFRAME_H
