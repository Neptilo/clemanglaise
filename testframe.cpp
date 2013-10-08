#include "testframe.h"
#include "themeframe.h"
#include "networkreplyreader.h"

#include "iostream"

TestFrame::TestFrame(Test &test, QString str_title, bool admin, QWidget *parent):
    QWidget(parent),
	nam_themes(),
    test(test),
    add_button(NULL),
    update_button(NULL),
    add_theme_button(NULL),
    admin(admin)
{
	title = new QLabel(str_title,this);
    title->setAlignment(Qt::AlignHCenter);
    layout = new QVBoxLayout(this);
    answer_frame = new AnswerFrame(test, this);
    init();
    connect(&nam_themes, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply_themes(QNetworkReply*)));
}

TestFrame::~TestFrame(){
    delete request;
    delete nam;
    delete reply_list;
}

void TestFrame::init() {

	layout->addWidget(title); 

	theme = new QLabel(tr("<i>Choose a theme</i>"), this);
	layout->addWidget(theme);
	themes = new QComboBox(this);
    // set the ComboBox to that width.
	themes->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
	layout->addWidget(themes);

    back_button = new QPushButton(tr("Go back to tests list"), this);
    back_button->setIcon(QIcon::fromTheme("go-home", QIcon(":/go-home.png")));
    connect(back_button, SIGNAL(clicked()), this, SLOT(go_back()));
    layout->addWidget(back_button);

    if(!test.isRemoteWork() || admin){
        add_theme_button = new QPushButton(tr("Add a theme"), this);
        add_theme_button->setIcon(QIcon::fromTheme("list-add",QIcon(":/list-add.png")));
        connect(add_theme_button, SIGNAL(clicked()), this, SLOT(add_theme()));
        layout->addWidget(add_theme_button);

        add_button = new QPushButton(tr("Add a word"), this);
        add_button->setIcon(QIcon::fromTheme("list-add",QIcon(":/list-add.png")));
        connect(add_button, SIGNAL(clicked()), this, SLOT(add_word()));
        layout->addWidget(add_button);

        update_button = new QPushButton(tr("Edit this word entry"), this);
        update_button->setIcon(QIcon::fromTheme("accessories-text-editor", QIcon(":/accessories-text-editor.png")));
        connect(update_button, SIGNAL(clicked()), this, SLOT(update_word()));
        layout->addWidget(update_button);
    }

    search_button = new QPushButton(tr("Search for words"), this);
    search_button->setIcon(QIcon::fromTheme("edit-find", QIcon(":/edit-find.png")));
    connect(search_button, SIGNAL(clicked()), this, SLOT(search()));
    layout->addWidget(search_button);

    question_frame = new QuestionFrame(test, this);
    layout->addWidget(question_frame);
    update_request();
    nam = new QNetworkAccessManager;
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));
    nam->get(*request);
    find_themes();
}

void TestFrame::update_request() {
    // Request to PHP or local file
	QUrl url;
	int index = themes->currentIndex();
	QString root = test.getSrc() + test.getDst(); 
	if (test.isRemoteWork()) {
        url = QUrl("http://neptilo.com/php/clemanglaise/find_lowest.php?lang=" + root +"&id_theme="+themes->itemData(index).toString());
	} else {
		parser = new Parser(root);
		if (!index || (index && index < 0)) {
			parser->parse(parser->getFilein());
		} else {	
			parser->parse(root + "/" + themes->itemData(index).toString() + "_" + themes->itemText(index));
		}
		url = QUrl(Parser::get_working_path(parser->getFileout()));
	}
    request = new QNetworkRequest(url);
}

void TestFrame::read_reply(QNetworkReply* reply){

    // Store the lines of the reply in the "reply_list" attribute
    QString* reply_string = new QString(reply->readAll());
    reply->deleteLater();
    reply_list = test.isRemoteWork()?new QStringList(reply_string->split('\n')):new QStringList(reply_string->split(QRegExp(ENDL)));

    // Everything is ready for the question frame to ask the question.
    QString word = reply_list->at(1);
	if (test.isRemoteWork()) {
		QString theme = reply_list->at(9);
		question_frame->ask_question(word, theme);
	} else {
		int id_theme = reply_list->at(6).toInt();
		question_frame->ask_question(word, Parser::getTheme(id_theme));
	}
}

void TestFrame::validate_question(){

    // Create a new answer frame
    delete answer_frame;
    answer_frame = new AnswerFrame(*reply_list, question_frame->getAnswer(), test, this);
    layout->addWidget(answer_frame);
}

void TestFrame::validate_answer() {
	int index = themes->currentIndex();
	QString root = test.getSrc() + test.getDst(); 

    // Remove everything
    delete question_frame;
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

void TestFrame::add_theme() {
    // Remove everything
    delete question_frame;
    answer_frame->hide();
	theme->hide();
	themes->hide();
    if(back_button){
        disconnect(back_button);
        back_button->hide();
    }
    if(add_theme_button){
        disconnect(add_theme_button);
        add_theme_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }
    if(add_button){
        disconnect(add_button);
        add_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }
    if(update_button){
        disconnect(update_button);
        update_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }
    if(search_button){
        disconnect(search_button);
        search_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }

    // Create a new add frame
    QStringList default_values_list;
    default_values_list << "" << "";
    add_theme_frame = new ThemeFrame(test, tr("<b>Add a new theme</b>"), default_values_list, tr("Add"), "add_theme", tr("Theme successfully added!"), this);
    layout->addWidget(add_theme_frame);
    connect(add_theme_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::add_word(){

    // Remove everything
    delete question_frame;
    answer_frame->hide();
	theme->hide();
	themes->hide();
    if(back_button){
        disconnect(back_button);
        back_button->hide();
    }
    if(add_theme_button){
        disconnect(add_theme_button);
        add_theme_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }
    if(add_button){
        disconnect(add_button);
        add_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }
    if(update_button){
        disconnect(update_button);
        update_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }
    if(search_button){
        disconnect(search_button);
        search_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }

    // Create a new add frame
    QStringList default_values_list;
	//word << meaning << nature << comment << exple << id_theme << pronunciation << score<< theme
 
    default_values_list << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
    add_frame = new EditFrame(test, tr("<b>Add a new word</b>"), default_values_list, tr("Add"), "add", tr("Word successfully added!"), this);
    layout->addWidget(add_frame);
    connect(add_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::update_word(){

    // Remove everything
    delete question_frame;
    answer_frame->hide();
	theme->hide();
	themes->hide();
    if(back_button){
        disconnect(back_button);
        back_button->hide();
    }
    if(add_theme_button){
        disconnect(add_theme_button);
        add_theme_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }
    if(add_button){
        disconnect(add_button);
        add_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }
    if(update_button){
        disconnect(update_button);
        update_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }
    if(search_button){
        disconnect(search_button);
        search_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }

    // Create a new add frame
    update_frame = new EditFrame(test, tr("<b>Edit a word entry</b>"), *reply_list, tr("Edit"), "update", tr("Word successfully edited!"), this);
    layout->addWidget(update_frame);
    connect(update_frame, SIGNAL(destroyed()), this, SLOT(init()));
}

void TestFrame::search()
{
    // Remove everything
    delete question_frame;
    answer_frame->hide();
    theme->hide();
    themes->hide();
    if(back_button){
        disconnect(back_button);
        back_button->hide();
    }
    if(add_theme_button){
        add_theme_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
        disconnect(add_theme_button);
    }
    if(add_button){
        add_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
        disconnect(add_button);
    }
    if(update_button){
        update_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
        disconnect(update_button);
    }
    if(search_button){
        disconnect(search_button);
        search_button->hide(); // Careful! If I don't delete it, there's gonna be memory leaks.
    }

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
        Parser* p = new Parser(test.getSrc() + test.getDst());
		read_reply(p->search("", Parser::getThemeFile()));
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
	themes->addItem("");
	for(int i=0, l = reply_list.count(); i<l-1; i+=2) {
		themes->addItem(reply_list.at(i+1), QVariant(reply_list.at(i).toInt()));
    }
    connect(themes, SIGNAL(currentIndexChanged(int)), this, SLOT(update_question(int)));
}

