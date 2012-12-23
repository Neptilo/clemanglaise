#include <QTextDocument>

#include "questionframe.h"
#include "ampersand_escape.h"

QuestionFrame::QuestionFrame(QWidget *parent)
    : QWidget(parent)
{
    // GUI
    layout = new QVBoxLayout(this);

    label = new QLabel("Loading...", this);
    layout->addWidget(label);
}

QuestionFrame::~QuestionFrame(){
    delete label;
    delete edit;
    delete layout;
    delete OK_button;
}

QString QuestionFrame::getAnswer(){
    return ampersand_escape(edit->text());
}

void QuestionFrame::ask_question(const QString& word){

    // Display question
    label->setText("Translate <b>" + word + "</b> into French.");

    // Create edit field
    edit = new QLineEdit(this);
    layout->addWidget(edit);

    // Create OK button
    OK_button = new QPushButton("OK", this);
    layout->addWidget(OK_button);

    // Connections
    connect(edit, SIGNAL(returnPressed()), parent(), SLOT(validate_question()));
    connect(edit, SIGNAL(returnPressed()), this, SLOT(disable_validation()));
    connect(OK_button, SIGNAL(clicked()), parent(), SLOT(validate_question()));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(disable_validation()));
    OK_button->setDefault(true);
    edit->setFocus();
    OK_button->setEnabled(true);
}

void QuestionFrame::disable_validation(){
    OK_button->disconnect();
    OK_button->hide();
    edit->disconnect();
}
