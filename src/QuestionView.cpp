#include "QuestionView.h"

#include <QAction>
#include <QApplication>
#include <QTextDocument>
#include <QLineEdit>
#include <QPainter>
#include <QToolButton>
#include <QGuiApplication>
#include <QScreen>

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
    resizeEvent(nullptr);
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
    // List of languages for which we want a handwriting widget
    QStringList handwriting_langs;
    handwriting_langs << "ja" << "zh";

    // Left/upper part
    if (handwriting_langs.contains(test->get_dst())){
        QLabel *handwriting_label = new QLabel(tr("Draw the characters in the box below. (You can then check them for yourself.)"), this);
        handwriting_label->setWordWrap(true);
        handwriting_layout->addWidget(handwriting_label);

        QSize handwriting_area_size(QGuiApplication::primaryScreen()->geometry().width(), 128);
        handwriting_area = new HandwritingArea(handwriting_area_size, this);
        handwriting_area->setMinimumSize(128, 128);
        handwriting_layout->addWidget(handwriting_area);
    }

    // Right/lower part

    // Display question
    QString hint_text = (hint.isEmpty())? "" : "<i>Hint: " + hint+"</i><br />";
    QString formatted_word = QString("<b style='font-size: %1pt;'>%2</b>")
        .arg(InterfaceParameters::widget_unit / 2).arg(word);
    QString instruction;
    if(test->get_dst() == "ja")
        instruction = tr("Write the pronunciation of this in r&#333;maji.");
    else if(test->get_dst() == "zh")
        instruction = tr("Write the pronunciation of this in pinyin.");
    else if(test->get_dst() == "ar")
        instruction = tr("Write the pronunciation of this in ASCII.");
    else
        instruction = tr("Translate this into the target language.");
    label->setText(formatted_word + "<br />" + instruction + "<br />" + hint_text);

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
    OK_button = new QPushButton(
                getIcon("emblem-default"),
                tr("OK"),
                this);
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

    resizeEvent(nullptr);
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
    if (edit) {
        edit->setFixedHeight(InterfaceParameters::widget_unit);
        QFont font = edit->font();
        font.setPointSize(0.5 * InterfaceParameters::widget_unit);
        edit->setFont(font);
    }
    if (OK_button) {
        OK_button->setFixedSize(2*InterfaceParameters::widget_unit,
                                InterfaceParameters::widget_unit);
        OK_button->setIconSize(QSize(InterfaceParameters::widget_unit/2,
                                     InterfaceParameters::widget_unit/2));
    }
}
