#include <QTextDocument>
#include <QLineEdit>
#include <QPainter>

#include "QuestionView.h"
#include "string_utils.h"

QuestionView::QuestionView(Test *test, QWidget *parent):
    WordView(test, parent),
    label(NULL),
    import_button(NULL),
    edit(NULL),
    handwriting_area(NULL)
{
    label = new QLabel(tr("Loading..."), this);
    vertical_layout->addWidget(label);
}

QuestionView::~QuestionView(){
    delete label;
    if(edit)
        delete edit;
}

QString QuestionView::get_answer(){
    return ampersand_escape(edit->text());
}

void QuestionView::ask_question(const QString& word, const QString & theme) {

    // Left part
    if(handwriting){
        right_vertical_layout->addWidget(new QLabel(tr("Draw the characters in the box below. (You can then check them for yourself.)"), this));

        QSize handwriting_area_size(500, 150);
        handwriting_area = new HandwritingArea(handwriting_area_size, this);
        handwriting_area->setMinimumSize(handwriting_area_size);
        right_vertical_layout->addWidget(handwriting_area);
    }

    // Right part

    // Display question
	QString context("");
    context = (theme.isEmpty())? "" : "<i>context: " + theme+"</i><br />";
    if(test->get_dst() == "fr") {
        label->setText(tr("Translate <b>") + word + tr("</b> into French. <br />") + context);
    }else if(test->get_dst() == "ja"){
        label->setText("<b>"+word + "</b><br />" + context + tr("<br/>Write the pronunciation of this word in r&#333;maji."));
    }else if(test->get_dst() == "zh"){
        label->setText("<b>"+word + "</b><br />" + context + tr("<br/>Write the pronunciation of this word in pinyin."));
    }else if(test->get_dst() == "hr"){
        label->setText(tr("Translate <b>") + word + tr("</b> into Croatian.<br />") + context);
    }else{
        label->setText(tr("Translate <b>") + word + tr("</b>.<br />")+ context);
    }

    // Create import button
    if(test->is_remote()){
        import_button = new QPushButton(tr("Import this word"), this);
        vertical_layout->addWidget(import_button);
        connect(import_button, SIGNAL(clicked()), parent(), SLOT(import_word()));
    }

    // Create edit field
    edit = new QLineEdit(this);
    vertical_layout->addWidget(edit);

    // Create OK button
    OK_button = new QPushButton(tr("OK"), this);
    OK_button->setIcon(QIcon::fromTheme("emblem-default", QIcon(getImgPath("emblem-default.png"))));

	vertical_layout->addWidget(OK_button);

	// Connections
	connect(edit, SIGNAL(returnPressed()), parent(), SLOT(validate_question()));
	connect(edit, SIGNAL(returnPressed()), this, SLOT(disable_validation()));
	connect(OK_button, SIGNAL(clicked()), parent(), SLOT(validate_question()));
	connect(OK_button, SIGNAL(clicked()), this, SLOT(disable_validation()));
	OK_button->setDefault(true);
	edit->setFocus();
    OK_button->setEnabled(true);
}

void QuestionView::show_error(const QString &error)
{
    delete label;
    label = new QLabel(error, this);
    vertical_layout->addWidget(label);
}

void QuestionView::disable_validation(){
	OK_button->disconnect();
	OK_button->hide();
	edit->disconnect();
    edit->setEnabled(false);
}
