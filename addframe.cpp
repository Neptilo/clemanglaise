#include "addframe.h"

AddFrame::AddFrame(QWidget *parent) :
    QWidget(parent){

    layout = new QVBoxLayout(this);

    title = new QLabel("Add a new word", this);
    layout->addWidget(title);

    word = new QLabel("Word: ", this);
    layout->addWidget(word);

    word_edit = new QLineEdit(this);
    layout->addWidget(word_edit);

    meaning = new QLabel("Meaning: ", this);
    layout->addWidget(meaning);

    meaning_edit = new QLineEdit(this);
    layout->addWidget(meaning_edit);

    comment = new QLabel("Comment: ", this);
    layout->addWidget(comment);

    comment_edit = new QLineEdit(this);
    layout->addWidget(comment_edit);

    example = new QLabel("Example: ", this);
    layout->addWidget(example);

    example_edit = new QLineEdit(this);
    layout->addWidget(example_edit);

    OK_button = new QPushButton("Add", this);
    layout->addWidget(OK_button);
}

AddFrame::~AddFrame(){
    delete title;
    delete word;
    delete meaning;
    delete comment;
    delete example;
    delete word_edit;
    delete meaning_edit;
    delete comment_edit;
    delete example_edit;
    delete OK_button;
    delete layout;
}
