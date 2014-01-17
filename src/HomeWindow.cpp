#include "HomeWindow.h"
#include "string_utils.h"

HomeWindow::HomeWindow(bool admin, QWidget *parent):
    QWidget(parent),
    test_frame(NULL),
    title(NULL),
    layout(NULL),
    tests(),
    mapping_texts(NULL),
    button_texts(NULL),
    online_buttons(NULL),
    offline_buttons(NULL),
    workremote(NULL),
    workoffline(NULL),
    admin(admin)
{
    setWindowTitle("Clemanglaise"); 
    setWindowIcon(QIcon(":/clemanglaise-img.png"));
    layout = new QVBoxLayout(this);
	init();
}

void HomeWindow::init(){
    int test_id = 0;

    // No need to delete tests in a destructor because they are directly referenced, not by a pointer
	// remote is true by default
    QList<Test> online_tests;
    online_tests << Test(test_id++, tr("English to &French"), "en", "fr");
    online_tests << Test(test_id++, tr("English to &Japanese"), "en", "ja");
    online_tests << Test(test_id++, tr("English to &Chinese"), "en", "zh");
    online_tests << Test(test_id++, tr("English to C&roatian"), "en", "hr");

	QStringList online_flags;
    online_flags << ":/france-img.png" << ":/japan-img.png" << ":/china-img.png" <<":/croatia-img.png";

    QList<Test> offline_tests;
    offline_tests << Test(test_id++, tr("English to Fr&ench"), "en", "fr", false);
    offline_tests << Test(test_id++, tr("&German to French"), "de", "fr", false);
	QStringList offline_flags;
    offline_flags << ":/france-img.png" << ":/germany-img.png";
    tests << online_tests << offline_tests;

    title = new QLabel(tr("<b>Choose your vocab test:</b>"), this);
    title->setAlignment(Qt::AlignHCenter);
    layout->addWidget(title);
	workremote = new QLabel(tr("Tests on remote server:"), this);
    online_buttons = new LanguageButtons(online_tests, this, online_flags);
	workoffline = new QLabel(tr("Offline tests:"), this);
    offline_buttons = new LanguageButtons(offline_tests, this, offline_flags);

	layout->addWidget(workremote);
    layout->addWidget(online_buttons);
	layout->addWidget(workoffline);
    layout->addWidget(offline_buttons);
}

HomeWindow::~HomeWindow(){
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
    Test test = tests[i];
    // No possible decomposition of the sentence, because of translations in foreign languages that put words in a different order.
    QString str_title = test.isRemoteWork()?
                       tr("<b>You are now working on <br />tests from the remote server.</b>"):
                       tr("<b>You are now working on <br /> offline tests.</b>");
	
    TestView* test_frame = new TestView(test, str_title, admin, this);
    connect(test_frame, SIGNAL(destroyed()), SLOT(init()));
    layout->addWidget(test_frame);
}
