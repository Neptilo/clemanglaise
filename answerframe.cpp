#include <QtNetwork>
#include "answerframe.h"
#include "string_utils.h"
AnswerFrame::AnswerFrame(Test &test, QWidget *parent):
    WordFrame(test, parent)
{}

AnswerFrame::AnswerFrame(const QStringList &reply_list, const QString &player_answer, Test &test, QWidget *parent):
    WordFrame(test, parent)
{
    // Define explicit variables for the content of the label
    QString word = reply_list.at(1);
    QString meaning = reply_list.at(2);
    QString nature = reply_list.at(3);
    QString comment = reply_list.at(4);
    QString example = reply_list.at(5);
    QString pronunciation = reply_list.at(6);
    int score = reply_list.at(7).toInt();

    // List of languages for which we asked for the pronunciation
    QStringList list;
    list << "ja" << "zh";
    bool asked_pronunciation = list.contains(test.getDst());

    // Check answer
    QString message;
    bool correct;
    if(asked_pronunciation){
        // Standardize player answer before checking
        QString standardized_answer = QString(player_answer);

        if(test.getDst() == "ja"){
            standardized_answer.replace(QString("ou"), QString("&#333;"));
            standardized_answer.replace(QString("uu"), QString("&#363;"));
            standardized_answer.replace(QString("aa"), QString("&#257;"));
            standardized_answer.replace(QString("ee"), QString("&#275;"));
        }else if(test.getDst() == "zh"){
            standardized_answer = numbers_to_accents(standardized_answer);
        }

        correct = (pronunciation.split(", ").contains(standardized_answer, Qt::CaseInsensitive));
    }else{
        QString standard_answer = ampersand_unescape(player_answer);
        QString meaning_standard_answer = ampersand_unescape(meaning);
        correct = (meaning_standard_answer.split(", ").contains(standard_answer, Qt::CaseInsensitive));
    }
    message = correct ? tr("Right!") : tr("Wrong!");

    // Update score
    const QUrl url("http://neptilo.com/php/clemanglaise/set_score.php");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkAccessManager* nam = new QNetworkAccessManager;
    QUrl post_data;
    post_data.addQueryItem("id", reply_list.at(0));
    post_data.addQueryItem("lang", test.getSrc() + test.getDst());
    post_data.addQueryItem("score", QString::number(score+(correct?1:-1)));
    //connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
    nam->post(request, post_data.encodedQuery());

    // Left part
    if(handwriting){
        QLabel* handwriting_label = new QLabel("<b>"+meaning+"</b>", this);
        QFont font;
        font.setPixelSize(100);
        handwriting_label->setFont(font);
        handwriting_label->setAlignment(Qt::AlignRight);
        right_vertical_layout->addWidget(handwriting_label);
    }

    // Right part

    // Add labels 
    vertical_layout->addWidget(new QLabel("<b>"+message+"</b>", this));

    if(asked_pronunciation){
        vertical_layout->addWidget(new QLabel("<b>"+word+"</b> <i>"+nature+"</i>: "+pronunciation, this));
    }else{
        vertical_layout->addWidget(new QLabel("<b>"+word+"</b> <i>"+nature+"</i>: "+meaning, this));
    }
    vertical_layout->addWidget(new QLabel("<i>"+comment+"</i>", this));
    if(example.compare("")){
        QTextBrowser * qtb = new QTextBrowser(this);
        qtb->setHtml("<b>Example:</b> "+ example);
        vertical_layout->addWidget(qtb);
    }

    // Create the OK button
    OK_button = new QPushButton(tr("OK"), this);
    connect(OK_button, SIGNAL(clicked()), parent, SLOT(validate_answer()));
    OK_button->setDefault(true);
    OK_button->setFocus(); // Because the focus is still on the edit line.
    vertical_layout->addWidget(OK_button);
}
