#include "string_utils.h"
#include "testframe.h"
#include "themeframe.h"
#include "networkreplyreader.h"

#include "iostream"

TestFrame::TestFrame(Test &test, QString str_title, bool admin, QWidget *parent):
    QWidget(parent),
    question_frame(NULL),
    answer_frame(NULL),
    add_theme_frame(NULL),
    update_theme_frame(NULL),
    add_frame(NULL),
    update_frame(NULL),
    search_frame(NULL),
    request(NULL),
    nam(NULL),
    nam_themes(),
    reply_list(),
    layout(NULL),
    theme(NULL),
    title(NULL),
    back_button(NULL),
    add_theme_button(NULL),
    add_button(NULL),
    search_button(NULL),
    update_button(NULL),
    test(test),
    themes(NULL),
    parser(NULL),
    admin(admin)
{
    title = new QLabel(str_title, this);
    title->setAlignment(Qt::AlignHCenter);
    layout = new QVBoxLayout(this);
    answer_frame = new AnswerFrame(test, this);

    layout->addWidget(title);

    theme = new QLabel(tr("<i>Choose a theme</i>"), this);
    layout->addWidget(theme);
    themes = new QComboBox(this);
    // set the ComboBox to that width.
    themes->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    layout->addWidget(themes);

    back_button = new QPushButton(tr("Go &back to tests list"), this);
    back_button->setIcon(QIcon::fromTheme("go-home", QIcon(getImgPath("go-home.png"))));
    connect(back_button, SIGNAL(clicked()), this, SLOT(go_back()));
    layout->addWidget(back_button);

    if(!test.isRemoteWork() || admin){
        add_theme_button = new QPushButton(tr("Add a &theme"), this);
        add_theme_button->setIcon(QIcon::fromTheme("list-add",QIcon(getImgPath("list-add.png"))));
        connect(add_theme_button, SIGNAL(clicked()), this, SLOT(add_theme()));
        layout->addWidget(add_theme_button);

        add_button = new QPushButton(tr("Add a &word"), this);
        add_button->setIcon(QIcon::fromTheme("list-add",QIcon(getImgPath("list-add.png"))));
        connect(add_button, SIGNAL(clicked()), this, SLOT(add_word()));
        layout->addWidget(add_button);

        update_button = new QPushButton(tr("&Edit this word entry"), this);
        update_button->setIcon(QIcon::fromTheme("accessories-text-editor", QIcon(getImgPath("accessories-text-editor.png"))));
        connect(update_button, SIGNAL(clicked()), this, SLOT(update_word()));
        layout->addWidget(update_button);
    }

    search_button = new QPushButton(tr("&Search for words"), this);
    search_button->setIcon(QIcon::fromTheme("edit-find", QIcon(getImgPath("edit-find.png"))));
    connect(search_button, SIGNAL(clicked()), this, SLOT(search()));
    layout->addWidget(search_button);

    if (!test.isRemoteWork())
        parser = new Parser(test.getSrc() + test.getDst());

    init();
    connect(&nam_themes, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply_themes(QNetworkReply*)));
}

TestFrame::~TestFrame(){
    delete request;
    delete parser;
}

// This function is called every time the user comes back from another view.
void TestFrame::init()
{
    question_frame = new QuestionFrame(test, this);
    layout->addWidget(question_frame);
    update_request();
    nam = new QNetworkAccessManager(this);
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
    nam->get(*request);
    find_themes();

    // Show everything
    if(!test.isRemoteWork() || admin){
        update_button->show();
        add_theme_button->show();
        add_button->show();
    }
    theme->show();
    themes->show();
    back_button->show();
    search_button->show();
}

void TestFrame::update_request() {
    // Request to PHP or local file
	QUrl url;
	int index = themes->currentIndex();
    QString root = test.getSrc() + test.getDst();
	if (test.isRemoteWork()) {
        url = QUrl("http://neptilo.com/php/clemanglaise/find_lowest.php?lang=" + root +"&id_theme="+themes->itemData(index).toString());
	} else {
		if (!index || (index && index < 0)) {
			parser->parse(parser->getFilein());
        } else {
			parser->parse(root + "/" + themes->itemData(index).toString() + "_" + themes->itemText(index));
		}
		url = QUrl(Parser::get_working_path(parser->getFileout()));
	}
    delete request; // It cannot be deleted before because it still has to be available when a new question is loaded. (The request stays the same.)
    request = new QNetworkRequest(url);
}

void TestFrame::read_reply(QNetworkReply* reply){
    if(question_frame){ // If question_frame is deleted, that means the user has changed the frame to another one before the NAM request was finished, so we want to ignore the NAM's reply.
        // Store the lines of the reply in the "reply_list" attribute
        QString reply_string(reply->readAll());
        reply->deleteLater();
        reply_list = test.isRemoteWork()?reply_string.split('\n'):reply_string.split(QRegExp(ENDL));

        // Everything is ready for the question frame to ask the question.
        QString word = reply_list.at(1);
        if (test.isRemoteWork()) {
            QString theme = reply_list.at(9);
            question_frame->ask_question(word, theme);
        } else {
            int id_theme = reply_list.at(6).toInt();
            question_frame->ask_question(word, Parser::getTheme(id_theme));
        }
    }
}

void TestFrame::validate_question(){

    // Create a new answer frame
    delete answer_frame;
    answer_frame = new AnswerFrame(reply_list, question_frame->getAnswer(), test, this);
    layout->addWidget(answer_frame);
}

void TestFrame::validate_answer() {
	int index = themes->currentIndex();
	QString root = test.getSrc() + test.getDst(); 

    // Remove everything
    delete question_frame;
    question_frame = NULL;
    if(answer_frame)
        answer_frame->hide();

    // Create a new question frame
    question_frame = new QuestionFrame(test, this); // Is it deleted somewhere? It should because of "new".
    layout->addWidget(question_frame);

    // Request for a new question
	if (!test.isRemoteWork()) {
		if (!index || (index && index < 0)) {
			parser->parse(parser->getFilein());
        } else {
			parser->parse(root + "/" + themes->itemData(index).toString() + "_" + themes->itemText(index));
		}
    }
    nam->get(*request);
}

void TestFrame::update_question(int){
    update_request();
    validate_answer();
}

void TestFrame::add_theme()
{
    remove_widgets();

    // Create a new add frame
    QStringList default_values_list;
    default_values_list << "" << "";
    add_theme_frame = new ThemeFrame(test, tr("<b>Add a new theme</b>"), default_values_list, tr("Add"), "add_theme", tr("Theme successfully added!"), this);
    layout->addWidget(add_theme_frame);
    connect(add_theme_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::add_word()
{
    remove_widgets();

    // Create a new add frame
    QStringList default_values_list;
	//word << meaning << nature << comment << exple << id_theme << pronunciation << score<< theme

    default_values_list << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
    add_frame = new EditFrame(test, tr("<b>Add a new word</b>"), default_values_list, tr("Add"), "add", tr("Word successfully added!"), this);
    layout->addWidget(add_frame);
    connect(add_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::update_word()
{
    remove_widgets();

    // Create a new add frame
    update_frame = new EditFrame(test, tr("<b>Edit a word entry</b>"), reply_list, tr("Edit"), "update", tr("Word successfully edited!"), this);
    layout->addWidget(update_frame);
    connect(update_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::search()
{
    remove_widgets();

    // Create a new search frame
    search_frame = new SearchFrame(test, !test.isRemoteWork()||admin, this);
    layout->addWidget(search_frame);
    connect(search_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::go_back() {
	delete this;
}

void TestFrame::find_themes() {
	if (!test.isRemoteWork()) {
        // Offline
        Parser p(test.getSrc() + test.getDst());
        read_reply(p.search("", Parser::getThemeFile()));
	} else { 
		// Request to PHP file
        const QUrl url = QUrl("http://neptilo.com/php/clemanglaise/find_used_themes.php?lang=" + test.getSrc() + test.getDst());
		QNetworkRequest request(url);
		nam_themes.get(request);
	}
}

void TestFrame::read_reply_themes(QNetworkReply* reply)
{
    // Store the lines of the reply in the "reply_list" attribute
    QString reply_string = reply->readAll();
    reply->deleteLater();
	read_reply(reply_string);
}

void TestFrame::read_reply(QString reply_string) {
    QStringList reply_list(reply_string.split('\n', QString::SkipEmptyParts));
    themes->disconnect();
    themes->clear();
    themes->addItem("---");
	for(int i=0, l = reply_list.count(); i<l-1; i+=2) {
		themes->addItem(reply_list.at(i+1), QVariant(reply_list.at(i).toInt()));
    }    
    connect(themes, SIGNAL(currentIndexChanged(int)), this, SLOT(update_question(int)));
}

void TestFrame::remove_widgets()
{
    delete question_frame;
    question_frame = NULL;
    delete answer_frame;
    answer_frame = NULL;
    if(!test.isRemoteWork() || admin){
        update_button->hide();
        add_theme_button->hide();
        add_button->hide();
    }
    theme->hide();
    themes->hide();
    back_button->hide();
    search_button->hide();
}
