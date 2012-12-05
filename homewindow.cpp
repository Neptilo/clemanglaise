#include "homewindow.h"

HomeWindow::HomeWindow(QWidget *parent): QWidget(parent){

    setWindowTitle("Clemanglaise");

    QLayout* layout = new QVBoxLayout(this);

    title = new QLabel(tr("Choose your vocab test:"), this);
    layout->addWidget(title);

    mapping_texts = new QStringList;
    *mapping_texts << "enfr";
    *mapping_texts << "enja";

    button_texts = new QStringList;
    *button_texts << "English to French";
    *button_texts << "English to Japanese";

    buttons = new LanguageButtons(*mapping_texts, *button_texts, this);
    layout->addWidget(buttons);
}

void HomeWindow::start_test(const QString & mapping_text){
    title->hide();
    buttons->hide();
    buttons->disconnect_all();
    TestFrame* test_frame = new TestFrame(mapping_text, this);
    layout()->addWidget(test_frame);
}
