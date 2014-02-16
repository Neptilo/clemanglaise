#include "HomeView.h"

#include "string_utils.h"

HomeView::HomeView(bool admin, QWidget *parent):
    QWidget(parent),
    add_list_view(NULL),
    admin(admin),
    database_manager(this),
    layout(NULL),
    test_buttons(NULL),
    test_source_switcher(Qt::Horizontal, tr("Local tests"), tr("Online tests"), this),
    title(NULL)
{
    setWindowTitle("Clemanglaise");
    setWindowIcon(QIcon(":/clemanglaise-img.png"));
    layout = new QVBoxLayout(this);

    // title
    title = new QLabel(tr("<b>Choose your vocab test:</b>"), this);
    title->setAlignment(Qt::AlignHCenter);
    layout->addWidget(title);

    // test source switcher
    layout->addWidget(&test_source_switcher);
    set_test_source(test_source_switcher.value());

    // info label
    layout->addWidget(&info_label);

    // add list button
    layout->addWidget(&add_list_button);
    add_list_button.hide();

    connect(&nam, SIGNAL(finished(QNetworkReply *)), this, SLOT(read_reply_lists(QNetworkReply *)));

    init();
}

HomeView::~HomeView()
{}

void HomeView::add_list()
{
    add_list_button.disconnect();
    add_list_button.hide();
    add_list_view = new AddListView(&database_manager, remote, this); // database_manager is useless for online tests
    layout->addWidget(add_list_view);
}

void HomeView::init()
{
    delete test_buttons;
    test_buttons = NULL;

    delete add_list_view;
    add_list_view = NULL;

    title->show();

    test_source_switcher.show();
    connect(&test_source_switcher, SIGNAL(value_changed(bool)), this, SLOT(set_test_source(bool)));

    info_label.setText(tr("Loading..."));
    info_label.show();

    if(remote){
        // Request to PHP file to get the list of online vocabulary lists
        const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/get_lists.php");
        QNetworkRequest request(url);
        nam.get(request);
    }else{
        // test buttons
        QList<Test> offline_tests(database_manager.get_lists());
        delete test_buttons;
        test_buttons = new LanguageButtons(offline_tests, this);
        layout->addWidget(test_buttons);

        // info label
        info_label.setText(tr("<b>Offline tests:</b>"));

        // add list buttons
        add_list_button.setText(tr("Create a local vocabulary list"));
        add_list_button.show();
        connect(&add_list_button, SIGNAL(clicked()), this, SLOT(add_list()));
    }
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

    // test buttons
    delete test_buttons;
    test_buttons = new LanguageButtons(online_tests, this);
    layout->addWidget(test_buttons);

    // info label
    info_label.setText(tr("<b>Tests on remote server:</b>"));

    // add list button
    if(admin){
        add_list_button.setText(tr("Create an online vocabulary list"));
        add_list_button.show();
        connect(&add_list_button, SIGNAL(clicked()), this, SLOT(add_list()));
    }
}

void HomeView::remove_add_list_view()
{
    delete add_list_view;
    add_list_view = NULL;

    // show add list button again
    if(admin || !remote){
        add_list_button.show();
        connect(&add_list_button, SIGNAL(clicked()), this, SLOT(add_list()));
    }
}

void HomeView::set_test_source(bool remote)
{
    this->remote = remote;
    init();
}

void HomeView::start_test(QObject *obj){
    Test *test((Test *) obj);

    title->hide();

    test_source_switcher.hide();
    test_source_switcher.disconnect();

    info_label.hide();

    test_buttons->disconnect_all();
    test_buttons->hide();

    add_list_button.disconnect();
    add_list_button.hide();

    delete add_list_view;
    add_list_view = NULL;

    /* No possible decomposition of the sentence, because of translations in
     * foreign languages that put words in a different order. */
    QString str_title(test->is_remote_work()?
                          tr("<b>You are now working on <br />tests from the remote server.</b>"):
                          tr("<b>You are now working on <br /> offline tests.</b>"));

    TestView* test_view = new TestView(test, &database_manager, str_title, admin, this);
    connect(test_view, SIGNAL(destroyed()), SLOT(init()));
    layout->addWidget(test_view);
}
