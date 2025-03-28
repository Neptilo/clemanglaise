#include "AnswerView.h"

#include <QRegularExpression>
#include <QUrlQuery>

#include "InterfaceParameters.h"
#include "QuestionView.h"
#include "resource_utils.h"
#include "string_utils.h"
#include "NetworkReplyReader.h"

AnswerView::AnswerView(Test *test, QWidget *parent):
    WordView(test, parent)
{}

AnswerView::AnswerView(const QHash<QString, QString> &word_data,
                       const QString &player_answer,
                       Test *test,
                       #ifndef Q_OS_WASM
                       DatabaseManager *database_manager,
                       #endif
                       QWidget *parent):
    WordView(test, parent)
  #ifndef Q_OS_WASM
  , database_manager(database_manager)
  #endif
{
    // Define explicit variables for the content of the label
    QString word = word_data["word"];
    QString meaning = word_data["meaning"];
    QString nature = word_data["nature"];
    QString comment = word_data["comment"];
    QString example = word_data["example"];
    QString pronunciation = word_data["pronunciation"];

    // Check answer
    QString message;
    // remove whitespaces at start and end
    QString standardized_answer = player_answer.trimmed();

    // can be the correct meaning or pronunciation depending to the language
    QString correct_answers;

    QStringList languages_asking_pronunciation;
    languages_asking_pronunciation << "ja" << "zh" << "ar";
    if(languages_asking_pronunciation.contains(test->get_dst())){
        // Standardize player answer before checking
        if(test->get_dst() == "ja"){
            standardized_answer.replace(QString("ou"), QString("&#333;"));
            standardized_answer.replace(QString("uu"), QString("&#363;"));
            standardized_answer.replace(QString("aa"), QString("&#257;"));
            standardized_answer.replace(QString("ee"), QString("&#275;"));
            // not before because otherwise "o u" would be replaced by "ō"
            static const QRegularExpression regex("\\s");
            standardized_answer.replace(regex, QString());
        }else if(test->get_dst() == "zh")
            standardized_answer = numbers_to_accents(standardized_answer, " ");
        else if(test->get_dst() == "ar")
            standardized_answer = ASCII_to_DIN(standardized_answer, false);

        // Standardize correct answers
        correct_answers = ampersand_unescape(pronunciation);
    }else{
        // Standardize player answer before checking
        standardized_answer = remove_diacritics(ampersand_unescape(standardized_answer));

        // Standardize correct answers
        correct_answers = remove_diacritics(ampersand_unescape(meaning));
    }
    QStringList correct_answer_list = correct_answers.split(",");
    for (int i = 0; i < correct_answer_list.size(); ++i) {
        // remove whitespaces at start and end of each element in the list
        QString answer(correct_answer_list.at(i).trimmed());

        if (test->get_dst() == "ja")
        {
            static const QRegularExpression regex("[^&#;a-zA-Z0-9]");
            answer.remove(regex);
        }
        else if (test->get_dst() == "zh")
            answer = separate_pinyin(answer);
        else if (test->get_dst() == "ar")
        {
            // remove punctuation
            // "\p{L}" matches all Unicode letters including accented characters
            static const QRegularExpression regex("[^\\p{L}\\dʾʿ]");
            answer.remove(regex);
        }

        correct_answer_list.replace(i, answer);
    }
    correct = correct_answer_list.contains(standardized_answer, Qt::CaseInsensitive);

    message = correct? tr("Right!"): tr("Wrong!");

#ifndef Q_OS_WASM
    // Update score, only if offline
    if (!test->is_remote())
        database_manager->set_score(word_data["id"].toInt(), correct);
#endif
    // Left/upper part
    QLabel* handwriting_label = new QLabel("<span>"+meaning+"</span>", this);
    QStringList non_alphabetic_langs;
    non_alphabetic_langs << "ja" << "zh";
    QStringList non_ASCII_langs;
    non_ASCII_langs << "ja" << "zh" << "ar";
    QFont font(non_alphabetic_langs.contains(test->get_dst()) ?
                   "HanWang KaiBold-Gb5" :
                   handwriting_label->font().family(),
               // print non-ASCII languages in big letters
               non_ASCII_langs.contains(test->get_dst()) ? 40 : 20);
    handwriting_label->setFont(font);
    handwriting_label->setAlignment(Qt::AlignHCenter);
    handwriting_layout->addWidget(handwriting_label);

    // Right/lower part

    // Add labels 
    QLabel* display_icon_answer = new QLabel(this);
    QLabel* display_answer = new QLabel(this);
    if (correct) {
        display_icon_answer->setPixmap(getIcon("face-smile")
                                       .pixmap(2*fontMetrics().height()));
        display_answer->setStyleSheet("QLabel {color : green; }");
    } else {
        display_icon_answer->setPixmap(getIcon("face-sad")
                                       .pixmap(2*fontMetrics().height()));
        display_answer->setStyleSheet("QLabel {color : red; }");
    }
    display_answer->setText("<b>" + message + "</b>");
    display_answer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QLayout *answer_message_layout = new QHBoxLayout;
    vertical_layout->addLayout(answer_message_layout);
    answer_message_layout->addWidget(display_icon_answer);
    answer_message_layout->addWidget(display_answer);

    QLayout *answer_headline_layout = new QHBoxLayout;
    vertical_layout->addLayout(answer_headline_layout);
    QLabel *answer_headline_label;
    QString answer = "<b>"+word+"</b> <i>"+nature+"</i>";

    if (!pronunciation.isEmpty()) {
        // Show pronunciation in brackets for IPA, or else between slashes.
        // The languages asking for pronunciation happen to be the ones
        // not written with the IPA.
        answer += ": " + (languages_asking_pronunciation.contains(test->get_dst()) ?
                    "<b>/"+ pronunciation +"/</b>" :
                    "<b>["+ pronunciation +"]</b>");
    }

    answer_headline_label = new QLabel(answer, this);
    answer_headline_label->setWordWrap(true);
    answer_headline_layout->addWidget(answer_headline_label);

    // Create the OK button
    OK_button = new QPushButton(
                getIcon("emblem-default.png"),
                tr("OK"),
                this);
    OK_button->setFixedSize(2*InterfaceParameters::widget_unit, InterfaceParameters::widget_unit);
    OK_button->setIconSize(QSize(InterfaceParameters::widget_unit/2, InterfaceParameters::widget_unit/2));
    connect(OK_button, SIGNAL(clicked()), parent, SLOT(validate_answer()));
    OK_button->setDefault(true);
    OK_button->setFocus(); // Because the focus is still on the edit line.
    answer_headline_layout->addWidget(OK_button);

    static const QRegularExpression regex("[\r\n]+");
    QLabel *comment_label = new QLabel(
        "<i>"+comment.replace(regex, "<br />")+"</i>", this);
    vertical_layout->addWidget(comment_label);
    if(example.compare("")){
        QTextBrowser *example_label = new QTextBrowser(this);
        example_label->setHtml("<b>Example:</b> "+ example.replace(regex, "<br />"));
        //example_label->setWordWrapMode(QTextOption::w);
        vertical_layout->addWidget(example_label);
    }
}

bool AnswerView::get_correct()
{
    return correct;
}
