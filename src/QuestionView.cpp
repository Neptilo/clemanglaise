#include "QuestionView.h"

#include <QAction>
#include <QApplication>
#include <QDesktopWidget>
#include <QTextDocument>
#include <QLineEdit>
#include <QPainter>
#include <QToolButton>

#include "InterfaceParameters.h"
#include "resource_utils.h"
#include "string_utils.h"

QuestionView::QuestionView(Test *test, bool admin, QWidget *parent):
    WordView(test, parent),
    admin(admin),
    handwriting_area(nullptr),
    label(nullptr),
    edit_button(nullptr),
    import_button(nullptr),
    delete_button(nullptr),
    edit(nullptr)
{
    create_actions();

    // GUI
    top_layout = new QHBoxLayout;
    label = new QLabel(tr("Loading..."), this);
    label->setWordWrap(true);
    vertical_layout->addLayout(top_layout);
    top_layout->addWidget(label);
}

QuestionView::~QuestionView(){
    delete label;
    if(edit)
        delete edit;
}

void QuestionView::create_actions()
{
    if (!test->is_remote() || admin) {
        edit_action = new QAction(
                    getIcon("accessories-text-editor"),
                    tr("&Edit this word entry"),
                    this);
        connect(edit_action, SIGNAL(triggered()), parent(), SLOT(update_word()));
        delete_action = new QAction(
                    getIcon("edit-delete"), tr("&Delete this word"), this);
        delete_action->setShortcut(QKeySequence::Delete);
        connect(delete_action, SIGNAL(triggered()), parent(), SLOT(delete_word()));
    }
    if (test->is_remote()) {
        import_action = new QAction(
                    getIcon("document-save"), tr("&Import this word"), this);
        import_action->setShortcut(QKeySequence::Save);
        connect(import_action, SIGNAL(triggered()), parent(), SLOT(import_word()));
    }
}

QString QuestionView::get_answer(){
    return ampersand_escape(edit->text());
}

void QuestionView::ask_question(const QString& word, const QString &hint) {

    // Left/upper part
    if(handwriting){
        QLabel *handwriting_label = new QLabel(tr("Draw the characters in the box below. (You can then check them for yourself.)"), this);
        handwriting_label->setWordWrap(true);
        handwriting_layout->addWidget(handwriting_label);

        QSize handwriting_area_size(QApplication::desktop()->width(), 128);
        handwriting_area = new HandwritingArea(handwriting_area_size, this);
        handwriting_area->setMinimumSize(128, 128);
        handwriting_layout->addWidget(handwriting_area);
    }

    // Right/lower part

    // Display question
    QString hint_text = (hint.isEmpty())? "" : "<i>Hint: " + hint+"</i><br />";
    if(test->get_dst() == "fr")
        label->setText(tr("Translate <b>") + word + tr("</b> into French. <br />") + hint_text);
    else if(test->get_dst() == "ja")
        label->setText("<b>"+word + "</b><br />" + hint_text + tr("<br/>Write the pronunciation of this word in r&#333;maji."));
    else if(test->get_dst() == "zh")
        label->setText("<b>"+word + "</b><br />" + hint_text + tr("<br/>Write the pronunciation of this word in pinyin."));
    else if(test->get_dst() == "ar")
        label->setText("<b>"+word + "</b><br />" + hint_text + tr("<br/>Write the pronunciation of this word in ASCII."));
    else if(test->get_dst() == "hr")
        label->setText(tr("Translate <b>") + word + tr("</b> into Croatian.<br />") + hint_text);
    else
        label->setText(tr("Translate <b>") + word + tr("</b>.<br />")+ hint_text);

    // Populate top layout with buttons
    if (!test->is_remote() || admin) {
        edit_button = new QToolButton;
        edit_button->setDefaultAction(edit_action);
        init_button(edit_button);
        delete_button = new QToolButton;
        delete_button->setDefaultAction(delete_action);
        init_button(delete_button);
        top_layout->addWidget(edit_button);
        top_layout->addWidget(delete_button);
    }
    if (test->is_remote()) {
        import_button = new QToolButton;
        import_button->setDefaultAction(import_action);
        init_button(import_button);
        top_layout->addWidget(import_button);
    }

    // user input
    input_layout = new QHBoxLayout;
    edit = new QLineEdit(this);
    edit->setFixedHeight(InterfaceParameters::widget_unit);
    OK_button = new QPushButton(
                getIcon("emblem-default"),
                tr("OK"),
                this);
    OK_button->setFixedSize(2*InterfaceParameters::widget_unit, InterfaceParameters::widget_unit);
    OK_button->setIconSize(QSize(InterfaceParameters::widget_unit/2, InterfaceParameters::widget_unit/2));
    vertical_layout->addLayout(input_layout);
    input_layout->addWidget(edit);
    input_layout->addWidget(OK_button);

    // Connections
    connect(edit, SIGNAL(returnPressed()), parent(), SLOT(validate_question()));
    connect(edit, SIGNAL(returnPressed()), this, SLOT(disable_validation()));
    connect(OK_button, SIGNAL(clicked()), parent(), SLOT(validate_question()));
    connect(OK_button, SIGNAL(clicked()), this, SLOT(disable_validation()));
    OK_button->setDefault(true);
    edit->setFocus();
    OK_button->setEnabled(true);
}

void QuestionView::init_button(QToolButton *button)
{
    if (button) {
        button->setFixedSize(InterfaceParameters::widget_unit, InterfaceParameters::widget_unit);
        button->setIconSize(QSize(InterfaceParameters::widget_unit/2, InterfaceParameters::widget_unit/2));
    }
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

void QuestionView::resizeEvent(QResizeEvent *event)
{
    WordView::resizeEvent(event);
    init_button(edit_button);
    init_button(delete_button);
    init_button(import_button);
    if (edit)
        edit->setFixedHeight(InterfaceParameters::widget_unit);
    if (OK_button) {
        OK_button->setFixedSize(2*InterfaceParameters::widget_unit, InterfaceParameters::widget_unit);
        OK_button->setIconSize(QSize(InterfaceParameters::widget_unit/2, InterfaceParameters::widget_unit/2));
    }
}
