#include "answerframe.h"

AnswerFrame::AnswerFrame(QWidget *parent, const QStringList &reply_list, const QString &player_answer)
    : QWidget(parent){

    // Create the layout
    layout = new QVBoxLayout(this);

    // Define explicit variables for the content of the label
    QString word = reply_list.at(0);
    QString meaning = reply_list.at(1);
    QString nature = reply_list.at(2);
    QString comment = reply_list.at(3);
    QString example = reply_list.at(4);

    // Check answer
    QString message = (meaning.split(", ").contains(player_answer, Qt::CaseInsensitive)) ? "Right!" : "Wrong!";

    // Create the string for the label
    QString string =
            (QString)
            "<b>"+message+"</b><br/>"
            + "<b>"+word+"</b> "
            + "<i>"+nature+"</i>: "
            + meaning+"<br/>"
            + "<i>"+comment+"</i><br/>";
    if(example.compare("")){
        string += "<b>Example:</b> "+example;
    }

    // Create the label
    label = new QLabel(string, this);
    layout->addWidget(label);

    // Create the OK button
    OK_button = new QPushButton("OK", this);
    connect(OK_button, SIGNAL(clicked()), parent, SLOT(validate_answer()));
    OK_button->setDefault(true);
    OK_button->setFocus(); // Because the focus is still on the edit line.
    layout->addWidget(OK_button);
}

AnswerFrame::~AnswerFrame(){
    delete label;
    delete layout;
    delete OK_button;
}
