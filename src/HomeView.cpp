#include "HomeView.h"
#include "string_utils.h"

HomeView::HomeView(bool admin, QWidget *parent):
    QWidget(parent),
    add_offline_list_view(NULL),
    add_online_list_view(NULL),
    add_offline_list_button(tr("Create a local vocabulary list"), this),
    add_online_list_button(tr("Create an online vocabulary list"), this),
    admin(admin),
    database_manager(this),
    layout(NULL),
    offline_buttons(NULL),
    online_buttons(NULL),
    title(NULL),
    work_offline(NULL),
    work_remote(NULL)
{
    setWindowTitle("Clemanglaise");
    setWindowIcon(QIcon(":/clemanglaise-img.png"));
    layout = new QVBoxLayout(this);
    title = new QLabel(tr("<b>Choose your vocab test:</b>"), this);
    title->setAlignment(Qt::AlignHCenter);
    layout->addWidget(title);
    work_remote = new QLabel(tr("<b>Tests on remote server:</b>"), this);
    work_offline = new QLabel(tr("<b>Offline tests:</b>"), this);

    connect(&nam, SIGNAL(finished(QNetworkReply *)), this, SLOT(read_reply_lists(QNetworkReply *)));

    init();
}

HomeView::~HomeView()
{}

void HomeView::add_offline_list()
{
    add_offline_list_button.disconnect();
    add_offline_list_button.hide();
    add_offline_list_view = new AddListView(&database_manager, false, this);
    layout->addWidget(add_offline_list_view);
}

void HomeView::add_online_list()
{
    add_online_list_button.disconnect();
    add_online_list_button.hide();
    add_online_list_view = new AddListView(&database_manager, true, this); // database_manager is useless.
    layout->addWidget(add_online_list_view);
}

void HomeView::remove_add_list_view()
{
    /*if(add_offline_list_view)
        layout->removeWidget(add_offline_list_view);*/
    delete add_offline_list_view;
    add_offline_list_view = NULL;
    delete add_online_list_view;
    add_online_list_view = NULL;
    init_offline_test_buttons();
    add_offline_list_button.show();
    connect(&add_offline_list_button, SIGNAL(clicked()), this, SLOT(add_offline_list()));
    if(admin){
        add_online_list_button.show();
        connect(&add_online_list_button, SIGNAL(clicked()), this, SLOT(add_online_list()));
    }
}

void HomeView::init()
{
    title->show();

    // Offline part
    init_offline_test_buttons();
    connect(&add_offline_list_button, SIGNAL(clicked()), this, SLOT(add_offline_list()));

    // Online part
    if(work_remote)
        work_remote->show();
    if(online_buttons)
        online_buttons->show();
    if(admin){
        add_online_list_button.show();
        connect(&add_online_list_button, SIGNAL(clicked()), this, SLOT(add_online_list()));
    }
    // Request to PHP file to get the list of online vocabulary lists
    const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/get_lists.php");
    QNetworkRequest request(url);
    nam.get(request);
}

void HomeView::init_offline_test_buttons()
{
    QList<Test> offline_tests(database_manager.get_lists());
    delete offline_buttons;
    offline_buttons = new LanguageButtons(offline_tests, this);
    work_offline->show();
    layout->addWidget(work_offline);
    layout->addWidget(offline_buttons);
    layout->addWidget(&add_offline_list_button);
    add_offline_list_button.show();
}

void HomeView::read_reply_lists(QNetworkReply *reply)
{
    QString reply_string = reply->readAll();
    reply->deleteLater();
    QStringList reply_list(reply_string.split('\n', QString::SkipEmptyParts));
    QList<Test> online_tests;
    for(int i = 0; i < reply_list.count(); i+=4) {
        online_tests << Test(reply_list.at(i).toInt(), reply_list.at(i+1), reply_list.at(i+2), reply_list.at(i+3), this);
    }
    delete online_buttons;
    online_buttons = new LanguageButtons(online_tests, this);
    layout->addWidget(work_remote);
    layout->addWidget(online_buttons);
    if(admin){
        layout->addWidget(&add_online_list_button);
        connect(&add_online_list_button, SIGNAL(clicked()), this, SLOT(add_online_list()));
    }
}

void HomeView::start_test(QObject *obj){
    Test *test((Test *) obj);
    title->hide();

    work_offline->hide();
    offline_buttons->disconnect_all();
    offline_buttons->hide();

    if(work_remote)
        work_remote->hide();
    if(online_buttons)
        online_buttons->hide();
    add_online_list_button.disconnect();
    add_online_list_button.hide();
    add_offline_list_button.disconnect();
    add_offline_list_button.hide();
    delete add_offline_list_view;
    add_offline_list_view = NULL;
    delete add_online_list_view;
    add_online_list_view = NULL;

    /* No possible decomposition of the sentence, because of translations in
     * foreign languages that put words in a different order. */
    QString str_title(test->is_remote_work()?
                          tr("<b>You are now working on <br />tests from the remote server.</b>"):
                          tr("<b>You are now working on <br /> offline tests.</b>"));

    TestView* test_view = new TestView(test, &database_manager, str_title, admin, this);
    connect(test_view, SIGNAL(destroyed()), SLOT(init()));
    layout->addWidget(test_view);
}
