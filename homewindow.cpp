#include "homewindow.h"

HomeWindow::HomeWindow(QWidget *parent): QWidget(parent){

    setWindowTitle("Clemanglaise");

    QLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);

    title = new QLabel(tr("Choose your vocab test:"), this);
    title->setAlignment(Qt::AlignHCenter);
    layout->addWidget(title);

    tests = new QList<Test>;
    *tests << *new Test("English to French", "en", "fr");
    *tests << *new Test("English to Japanese", "en", "ja");
    *tests << *new Test("English to Chinese", "en", "zh");

    buttons = new LanguageButtons(*tests, this);
    layout->addWidget(buttons);
}

void HomeWindow::start_test(int i){
    title->hide();
    buttons->hide();
    buttons->disconnect_all();
    Test test = (*tests)[i];
    TestFrame* test_frame = new TestFrame(test, this);
    layout()->addWidget(test_frame);
}
