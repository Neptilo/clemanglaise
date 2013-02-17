#include <QTextDocument>
#include <QLineEdit>

#include "questionframe.h"
#include "string_utils.h"

QuestionFrame::QuestionFrame(Test &test, QWidget *parent):
    WordFrame(test, parent)
{
    label = new QLabel(tr("Loading..."), this);
    vertical_layout->addWidget(label);
}

QuestionFrame::~QuestionFrame(){
    delete label;
    delete edit;
}

QString QuestionFrame::getAnswer(){
    return ampersand_escape(edit->text());
}

void QuestionFrame::ask_question(const QString& word){

    // Left part
    if(handwriting){
        right_vertical_layout->addWidget(new QLabel(tr("You can write the characters here if you have an IME.<br/>(Later you'll be able to draw them here.)"), this));

        QLineEdit* handwriting_edit = new QLineEdit(this);
        QFont font;
        font.setPixelSize(100);
        handwriting_edit->setFont(font);
        right_vertical_layout->addWidget(handwriting_edit);
    }

    // Right part

    // Display question
    if(test.getDst() == "fr"){
        label->setText(tr("Translate <b>") + word + tr("</b> into French."));
    }else if(test.getDst() == "ja"){
        label->setText("<b>"+word+tr("</b><br/>Write the pronunciation of this word in r&#333;maji."));
    }else if(test.getDst() == "zh"){
        label->setText("<b>"+word+tr("</b><br/>Write the pronunciation of this word in pinyin."));
    }

    // Create edit field
    edit = new QLineEdit(this);
    vertical_layout->addWidget(edit);

    // Create OK button
    OK_button = new QPushButton(tr("OK"), this);
    vertical_layout->addWidget(OK_button);

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
