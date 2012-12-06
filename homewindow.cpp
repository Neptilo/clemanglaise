#include "homewindow.h"

HomeWindow::HomeWindow(QWidget *parent): QWidget(parent){

    setWindowTitle("Clemanglaise");

    QLayout* layout = new QVBoxLayout(this);

    title = new QLabel(tr("Choose your vocab test:"), this);
    layout->addWidget(title);

    tests = new QList<Test>;
    *tests << *new Test("English to French", "en", "fr");
    *tests << *new Test("English to Japanese", "en", "ja");

    buttons = new LanguageButtons(*tests, this);
    layout->addWidget(buttons);
}

void HomeWindow::start_test(const QString & mapping_text){
    title->hide();
    buttons->hide();
    buttons->disconnect_all();
    TestFrame* test_frame = new TestFrame(mapping_text, this);
    layout()->addWidget(test_frame);
}
