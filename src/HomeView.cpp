#include "HomeView.h"
#include "string_utils.h"

HomeView::HomeView(bool admin, QWidget *parent):
    QWidget(parent),
    add_list_button(tr("Create a vocabulary list"), this),
    add_list_view(NULL),
    admin(admin),
    database_manager(this),
    layout(NULL),
    offline_buttons(NULL),
    online_buttons(NULL),
    tests(),
    title(NULL),
    work_offline(NULL),
    work_remote(NULL)
{
    setWindowTitle("Clemanglaise"); 
    setWindowIcon(QIcon(":/clemanglaise-img.png"));
    layout = new QVBoxLayout(this);
	init();
}

HomeView::~HomeView()
{}

void HomeView::add_list()
{
    add_list_button.hide();
    add_list_view = new AddListView(&database_manager, this);
    layout->addWidget(add_list_view);
}

void HomeView::remove_add_list_view()
{
    delete add_list_view;
    add_list_view = NULL;
    add_list_button.show();
}

void HomeView::init(){
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
    work_remote = new QLabel(tr("Tests on remote server:"), this);
    online_buttons = new LanguageButtons(online_tests, this, online_flags);
    work_offline = new QLabel(tr("Offline tests:"), this);
    offline_buttons = new LanguageButtons(offline_tests, this, offline_flags);

    layout->addWidget(work_remote);
    layout->addWidget(online_buttons);
    layout->addWidget(work_offline);
    layout->addWidget(offline_buttons);
    layout->addWidget(&add_list_button);
    connect(&add_list_button, SIGNAL(clicked()), this, SLOT(add_list()));
}

void HomeView::start_test(int i){
	title->hide();
    work_offline->hide();
    offline_buttons->disconnect_all();
    offline_buttons->hide();
    work_remote->hide();
    online_buttons->disconnect_all();
    online_buttons->hide();
    add_list_button.hide();
    delete add_list_view;
    Test test = tests[i];
    // No possible decomposition of the sentence, because of translations in foreign languages that put words in a different order.
    QString str_title = test.is_remote_work()?
                tr("<b>You are now working on <br />tests from the remote server.</b>"):
                tr("<b>You are now working on <br /> offline tests.</b>");

    TestView* test_view = new TestView(test, &database_manager, str_title, admin, this);
    connect(test_view, SIGNAL(destroyed()), SLOT(init()));
    layout->addWidget(test_view);
}
