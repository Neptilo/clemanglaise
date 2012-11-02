#ifndef TESTFRAME_H
#define TESTFRAME_H

#include "questionframe.h"
#include "answerframe.h"

class TestFrame : public QWidget{
    Q_OBJECT
    
public:
    TestFrame(QWidget *parent = 0);
    ~TestFrame();

private:
    QuestionFrame* question_frame;
    AnswerFrame* answer_frame;
    QNetworkRequest* request;
    QNetworkAccessManager* nam;
    QStringList* reply_list;
    QVBoxLayout* layout;

public slots:
    void read_reply(QNetworkReply* reply);
    void validate_question();
    void validate_answer();
};

#endif // TESTFRAME_H
