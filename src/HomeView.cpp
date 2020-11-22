#include "HomeView.h"

#include <QApplication>
#include <QTimer>
#include <QWidget>

#include "InterfaceParameters.h"
#include "string_utils.h"
#include "NetworkReplyReader.h"

HomeView::HomeView(bool admin, QWidget *parent):
    QWidget(parent),
    add_list_view(nullptr),
    admin(admin),
    database_manager(this),
    layout(nullptr),
    test_buttons(nullptr),
    test_source_switcher(Qt::Horizontal, tr("Local tests"), tr("Online tests"), this),
    title(nullptr)
{
    setWindowTitle("Clemanglaise");
    setWindowIcon(QIcon(":/clemanglaise-img.png"));
    layout = new QVBoxLayout(this);
#ifndef Q_OS_ANDROID
    layout->setSizeConstraint(QLayout::SetMinimumSize);
#endif

    // title
    title = new QLabel(tr("<b>Choose your vocab test:</b>"), this);
    title->setAlignment(Qt::AlignHCenter);
    layout->addWidget(title);

    // test source switcher
    layout->addWidget(&test_source_switcher);

    // info label
    layout->addWidget(&info_label);

    set_test_source(test_source_switcher.value()); // calls init()
}

HomeView::~HomeView()
{}

void HomeView::add_list()
{
    add_list_view = new AddListView(&database_manager, remote, this); // database_manager is useless for online tests
    connect(add_list_view, SIGNAL(created(Test *)), this, SLOT(list_created(Test *)));
    connect(add_list_view, SIGNAL(canceled()), this, SLOT(remove_add_list_view()));
    layout->addWidget(add_list_view);
}

void HomeView::init()
{
    delete test_buttons;
    test_buttons = nullptr;

    remove_add_list_view();

    title->show();

    test_source_switcher.show();
    connect(&test_source_switcher, SIGNAL(value_changed(bool)), this, SLOT(set_test_source(bool)), Qt::UniqueConnection);

    info_label.setText(tr("Loading..."));
    info_label.show();

    if(remote){
        // Request to PHP file to get the list of online vocabulary lists
        const QUrl url = QUrl("https://neptilo.com/php/clemanglaise/get_lists.php");
        QNetworkRequest request(url);
        QNetworkReply* reply = NetworkReplyReader::nam->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(read_reply_lists()));
    }else{
        // test buttons
        QList<Test> offline_tests(database_manager.get_lists());
        delete test_buttons;
        test_buttons = new LanguageButtons(offline_tests, true, this);
        connect(test_buttons, SIGNAL(clicked(Test *)), this, SLOT(start_test(Test *)));
        layout->addWidget(test_buttons);

        // info label
        if(offline_tests.isEmpty())
            info_label.setText(tr("You have not created any vocabulary list yet."));
        else
            info_label.setText(tr("<b>Offline tests:</b>"));
    }
}

void HomeView::shrink()
{
    resize(minimumSizeHint());
}

void HomeView::list_created(Test *test)
{
    // If the pointer test is not defined, it means that the database that created it didn't return its inserted ID.
    // Currently it means it was created in a remote database.
    if(test)
        start_test(test);
    else
        init();
}

void HomeView::read_reply_lists()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    if(remote){ // verification in case user clicked switch button before NAM's reply
        QString reply_string = reply->readAll().replace('\0', "");
        reply->deleteLater();
        if (reply_string.isEmpty())
            info_label.setText(tr("There is no vocabulary list on the server at the moment."));
        else {
            QStringList reply_list(reply_string.split('\n'));
            reply_list.removeLast();
            QList<Test> online_tests;
            for(int i = 0; i < reply_list.count(); i+=5) {
                online_tests << Test(reply_list.at(i).toInt(),
                                     reply_list.at(i+1),
                                     reply_list.at(i+2),
                                     reply_list.at(i+3),
                                     reply_list.at(i+4),
                                     true,
                                     this);
            }

            // test buttons
            delete test_buttons;

            test_buttons = new LanguageButtons(online_tests, admin, this);
            connect(test_buttons, SIGNAL(clicked(Test *)), this, SLOT(start_test(Test *)));
            layout->addWidget(test_buttons);

            // info label
            info_label.setText(tr("<b>Tests on remote server:</b>"));
        }
    }
}

void HomeView::remove_add_list_view()
{
    delete add_list_view;
    add_list_view = nullptr;
    // resize window to fit content
    // QTimer::singleShot(0, this, SLOT(shrink()));
}

void HomeView::set_test_source(bool remote)
{
    this->remote = remote;
    init();
}

void HomeView::start_test(Test *test){
    if(test){
        title->hide();

        test_source_switcher.hide();
        test_source_switcher.disconnect();

        info_label.hide();

        test_buttons->disconnect_all();
        test_buttons->hide();

        TestView* test_view = new TestView(*test, &database_manager, admin, this);

        // remove add_list_view only after creating test_view, because the Test, needed by test_view, will be deleted as child of add_list_view.
        remove_add_list_view();

        connect(test_view, SIGNAL(destroyed()), SLOT(init()));
        layout->addWidget(test_view);
    }else{
        add_list();
    }
}

void HomeView::resizeEvent(QResizeEvent *)
{
    InterfaceParameters::update_widget_unit(window());
    if (window()->width() < window()->height()) {
        if (InterfaceParameters::orientation == Qt::LandscapeOrientation) {
            InterfaceParameters::orientation = Qt::PortraitOrientation;
            if (test_buttons->isVisible())
                test_buttons->layout_buttons();
        }
    } else {
        if (InterfaceParameters::orientation == Qt::PortraitOrientation) {
            InterfaceParameters::orientation = Qt::LandscapeOrientation;
            if (test_buttons->isVisible())
                test_buttons->layout_buttons();
        }
    }
}
