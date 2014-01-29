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
    add_list_button.disconnect();
    add_list_button.hide();
    add_list_view = new AddListView(&database_manager, this);
    layout->addWidget(add_list_view);qDebug() << "added";
}

void HomeView::remove_add_list_view()
{
    if(add_list_view)
        layout->removeWidget(add_list_view);
    delete add_list_view;
    add_list_view = NULL;
    init_test_buttons();
    add_list_button.show();
}

void HomeView::init(){

    title = new QLabel(tr("<b>Choose your vocab test:</b>"), this);
    title->setAlignment(Qt::AlignHCenter);
    layout->addWidget(title);
    work_remote = new QLabel(tr("<b>Tests on remote server:</b>"), this);
    work_offline = new QLabel(tr("<b>Offline tests:</b>"), this);

    init_test_buttons();

    layout->addWidget(work_remote);
    layout->addWidget(online_buttons);
    layout->addWidget(work_offline);
    layout->addWidget(offline_buttons);
    layout->addWidget(&add_list_button);
    add_list_button.show();
    connect(&add_list_button, SIGNAL(clicked()), this, SLOT(add_list()));
}

void HomeView::init_test_buttons()
{
    int test_id = 0;

    /* No need to delete tests in a destructor because they are directly
     * referenced, not by a pointer */
    // remote is true by default
    QList<Test> online_tests;
    // TODO: Make it dynamic: call a "get_lists.php" file
    online_tests << Test(test_id++, tr("French"), "en", "fr");
    online_tests << Test(test_id++, tr("Japanese"), "en", "ja");
    online_tests << Test(test_id++, tr("Chinese"), "en", "zh");
    online_tests << Test(test_id++, tr("Croatian"), "en", "hr");

    QList<Test> offline_tests(database_manager.get_lists());

    tests.clear();
    tests << online_tests << offline_tests;
    delete online_buttons;
    online_buttons = new LanguageButtons(online_tests, this);
    delete offline_buttons;
    offline_buttons = new LanguageButtons(offline_tests, this);

}

void HomeView::start_test(int i){
	title->hide();
    work_offline->hide();
    offline_buttons->disconnect_all();
    offline_buttons->hide();
    work_remote->hide();
    online_buttons->disconnect_all();
    online_buttons->hide();
    add_list_button.disconnect();
    add_list_button.hide();
    delete add_list_view;
    Test test = tests[i];
    /* No possible decomposition of the sentence, because of translations in
     * foreign languages that put words in a different order. */
    QString str_title = test.is_remote_work()?
                tr("<b>You are now working on <br />tests from the remote server.</b>"):
                tr("<b>You are now working on <br /> offline tests.</b>");

    TestView* test_view = new TestView(test, &database_manager, str_title, admin, this);
    connect(test_view, SIGNAL(destroyed()), SLOT(init()));
    layout->addWidget(test_view);
}
