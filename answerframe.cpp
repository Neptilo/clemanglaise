#include "answerframe.h"

AnswerFrame::AnswerFrame(Test &test, QWidget *parent):
    WordFrame(test, parent)
{}

AnswerFrame::AnswerFrame(const QStringList &reply_list, const QString &player_answer, Test &test, QWidget *parent):
    WordFrame(test, parent)
{
    // Define explicit variables for the content of the label
    QString word = reply_list.at(0);
    QString meaning = reply_list.at(1);
    QString nature = reply_list.at(2);
    QString comment = reply_list.at(3);
    QString example = reply_list.at(4);
    QString pronunciation = reply_list.at(5);

    // List of languages for which we asked for the pronunciation
    QStringList list;
    list << "ja" << "zh";
    bool asked_pronunciation = list.contains(test.getDst());

    // Check answer
    QString message;
    if(asked_pronunciation){
        // Uniformize player answer before checking
        QString uniformized_answer = QString(player_answer);
        uniformized_answer.replace(QString("ou"), QString("&#333;"));
        uniformized_answer.replace(QString("uu"), QString("&#363;"));
        uniformized_answer.replace(QString("aa"), QString("&#257;"));
        uniformized_answer.replace(QString("ei"), QString("&#275;"));
        message = (pronunciation == uniformized_answer) ? tr("Right!") : tr("Wrong!");
    }else{
        message = (meaning.split(", ").contains(player_answer, Qt::CaseInsensitive)) ? tr("Right!") : tr("Wrong!");
    }

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
