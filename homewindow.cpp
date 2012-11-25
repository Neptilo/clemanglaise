#include "homewindow.h"

HomeWindow::HomeWindow(QWidget *parent) :
    QWidget(parent){

    layout = new QVBoxLayout(this);

    title = new QLabel(tr("Choose your vocab test:"), this);
    layout->addWidget(title);

    *buttons << new QPushButton("English to French", this);
    *buttons << new QPushButton("English to Japanese", this);

    for(int i=0; i<buttons->size(); ++i){
        layout->addWidget((*buttons)[i]);
        connect((*buttons)[i], SIGNAL(clicked()), this, SLOT(start_test()));
    }
}

void HomeWindow::start_test(){
    new TestFrame("enfr", this);
}
