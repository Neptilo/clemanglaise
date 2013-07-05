#include "homewindow.h"
HomeWindow::HomeWindow(QWidget *parent): QWidget(parent){

    setWindowTitle("Clemanglaise");

    QLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);

    title = new QLabel(tr("Choose your vocab test:"), this);
    title->setAlignment(Qt::AlignHCenter);
    layout->addWidget(title);
	radioLocal = new QRadioButton(tr("Work in localhost"), this);
	radioRemote = new QRadioButton(tr("Work on remote server"), this);
	radioRemote->setChecked(true);
	layout->addWidget(radioLocal);
	layout->addWidget(radioRemote);

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
	radioRemote->hide();
	radioLocal->hide();
    Test test = (*tests)[i];
	test.setRemoteWork(radioRemote->isChecked());
    TestFrame* test_frame = new TestFrame(test, this);
    layout()->addWidget(test_frame);
}

