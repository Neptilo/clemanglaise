#include "homewindow.h"
HomeWindow::HomeWindow(QWidget *parent): QWidget(parent){

    setWindowTitle("Clemanglaise");

    QLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);

    int test_id = 0;

    // No need to delete tests in a destructor because they are directly referenced, not by a pointer
	// remote is true by default
    QList<Test> online_tests;
    online_tests << *new Test(test_id++, "English to French", "en", "fr");
    online_tests << *new Test(test_id++, "English to Japanese", "en", "ja");
    online_tests << *new Test(test_id++, "English to Chinese", "en", "zh");

    QList<Test> offline_tests;
    offline_tests << *new Test(test_id++, "English to French", "en", "fr", false);

    tests = new QList<Test>(online_tests+offline_tests);

    title = new QLabel(tr("Choose your vocab test:"), this);
    title->setAlignment(Qt::AlignHCenter);
    layout->addWidget(title);
	workremote = new QLabel(tr("Tests on remote server:"), this);
    online_buttons = new LanguageButtons(online_tests, this);
	workoffline = new QLabel(tr("Offline tests:"), this);
    offline_buttons = new LanguageButtons(offline_tests, this);

	layout->addWidget(workremote);
    layout->addWidget(online_buttons);
	layout->addWidget(workoffline);
    layout->addWidget(offline_buttons);
}

HomeWindow::~HomeWindow(){
    delete tests;
    delete mapping_texts;
    delete button_texts;
}


void HomeWindow::start_test(int i){
	workoffline->hide();
    offline_buttons->disconnect_all();
	offline_buttons->hide();
	workremote->hide();
    online_buttons->disconnect_all();
	online_buttons->hide();
    Test test = (*tests)[i];
	QString rL = test.isRemoteWork()?"remote server":"offline";
	title->setText("You work on " + rL  + " tests");
    TestFrame* test_frame = new TestFrame(test, this);
    layout()->addWidget(test_frame);
}
