#ifndef TESTFRAME_H
#define TESTFRAME_H

#include "questionframe.h"
#include "answerframe.h"
#include "editframe.h"
#include "searchframe.h"
#include "test.h"

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
