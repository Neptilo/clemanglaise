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
        message = (pronunciation == player_answer) ? "Right!" : "Wrong!";
    }else{
        message = (meaning.split(", ").contains(player_answer, Qt::CaseInsensitive)) ? "Right!" : "Wrong!";
    }

    // Left part
    if(handwriting){
        QLabel* handwriting_edit = new QLabel("<b>"+meaning+"</b>", this);
        QFont font;
        font.setPixelSize(100);
        handwriting_edit->setFont(font);
        right_vertical_layout->addWidget(handwriting_edit);
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
        vertical_layout->addWidget(new QLabel("<b>Example:</b> "+ example, this));
    }

    // Create the OK button
    OK_button = new QPushButton("OK", this);
    connect(OK_button, SIGNAL(clicked()), parent, SLOT(validate_answer()));
    OK_button->setDefault(true);
    OK_button->setFocus(); // Because the focus is still on the edit line.
    vertical_layout->addWidget(OK_button);
}
