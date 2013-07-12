#include "homewindow.h"
HomeWindow::HomeWindow(QWidget *parent): QWidget(parent){

    setWindowTitle("Clemanglaise"); 
    layout = new QVBoxLayout(this);
	init();

}

void HomeWindow::init(){
    int test_id = 0;

    // No need to delete tests in a destructor because they are directly referenced, not by a pointer
	// remote is true by default
    QList<Test> online_tests;
    online_tests << *new Test(test_id++, tr("English to French"), "en", "fr");
    online_tests << *new Test(test_id++, tr("English to Japanese"), "en", "ja");
    online_tests << *new Test(test_id++, tr("English to Chinese"), "en", "zh");

    QList<Test> offline_tests;
    offline_tests << *new Test(test_id++, tr("English to French"), "en", "fr", false, false);

    tests = new QList<Test>(online_tests+offline_tests);

    title = new QLabel(tr("<b>Choose your vocab test:</b>"), this);
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
	title->hide();
    workoffline->hide();
    offline_buttons->disconnect_all();
    offline_buttons->hide();
    workremote->hide();
    online_buttons->disconnect_all();
    online_buttons->hide();
    Test test = (*tests)[i];
    // No possible decomposition of the sentence, because of translations in foreign languages that put words in a different order.
    QString str_title = test.isRemoteWork()?
                       tr("<b>You are now working on tests from the remote server.</b>"):
                       tr("<b>You are now working on offline tests.</b>");
	
    TestFrame* test_frame = new TestFrame(test,str_title, this);
	connect(test_frame,SIGNAL(destroyed()) ,SLOT(init()));
    layout->addWidget(test_frame);
}
