#include "AnswerView.h"

#include <QRegExp>
#include <QtNetwork>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#   include <QUrlQuery>
#endif

#include "InterfaceParameters.h"
#include "QuestionView.h"
#include "string_utils.h"

AnswerView::AnswerView(Test *test, QWidget *parent):
    WordView(test, parent)
{}

AnswerView::AnswerView(const QHash<QString, QString> &word_data, const QString &player_answer, Test *test, DatabaseManager *database_manager, QWidget *parent):
    WordView(test, parent),
	database_manager(database_manager)
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
    bool correct;
    // remove whitespaces at start and end
    QString standardized_answer = player_answer.trimmed();
    QString correct_answers; // can be the correct meaning or pronunciation according to the language
    if(test->get_dst()=="ja" || test->get_dst()=="zh"){
        // Standardize player answer before checking
        if(test->get_dst() == "ja"){
            standardized_answer.replace(QString("ou"), QString("&#333;"));
            standardized_answer.replace(QString("uu"), QString("&#363;"));
            standardized_answer.replace(QString("aa"), QString("&#257;"));
            standardized_answer.replace(QString("ee"), QString("&#275;"));
            // not before because otherwise "o u" would be replaced by "ō"
            standardized_answer.replace(QRegExp("\\s"), QString());
        }else if(test->get_dst() == "zh"){
            standardized_answer = numbers_to_accents(standardized_answer, " ");
        }
        correct_answers = pronunciation;
    }else{
        standardized_answer = ampersand_unescape(standardized_answer);
        standardized_answer = remove_diacritics(standardized_answer);
        correct_answers = ampersand_unescape(meaning);
        correct_answers = remove_diacritics(correct_answers);
    }
    QStringList correct_answer_list = correct_answers.split(",");
    // remove whitespaces at start and end of each element in the list
    for (int i = 0; i < correct_answer_list.size(); ++i) {
        QString answer(correct_answer_list.at(i).trimmed());
        if (test->get_dst() == "ja")
            answer.replace(QRegExp("[^&#;a-zA-Z0-9]+"), QString(""));
        else if (test->get_dst() == "zh")
            answer = separate_pinyin(answer);
        correct_answer_list.replace(i, answer);
    }
    correct = correct_answer_list.contains(standardized_answer, Qt::CaseInsensitive);

    message = correct? tr("Right!"): tr("Wrong!");

    // Update score
    if (!test->is_remote()) {
		//Offline
        database_manager->set_score(word_data["id"].toInt(), correct);
	} else {
		const QUrl url("https://neptilo.com/php/clemanglaise/set_score.php");
		QNetworkRequest request(url);
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
		QNetworkAccessManager* nam = new QNetworkAccessManager(this); // has to be a pointer because otherwise it will be destroyed at the end of the function and not send the data.
		
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
		QUrl post_data;
        post_data.addQueryItem("id", word_data["id"]);
        post_data.addQueryItem("correct", QString::number(correct));
		nam->post(request, post_data.encodedQuery());
#else
		QUrlQuery post_data;
        post_data.addQueryItem("id", word_data["id"]);
		post_data.addQueryItem("correct", QString::number(correct));
        nam->post(request, post_data.query(QUrl::FullyEncoded).toUtf8());
#endif
	}
    // Left/upper part
    if(handwriting){
        QLabel* handwriting_label = new QLabel("<span>"+meaning+"</span>", this);
        QFont font("HanWang KaiBold-Gb5", 50);
        handwriting_label->setFont(font);
        handwriting_label->setAlignment(Qt::AlignRight);
        handwriting_layout->addWidget(handwriting_label);
    }

    // Right/lower part

    // Add labels 
    QLabel* display_icon_answer = new QLabel(this);
    QLabel* display_answer = new QLabel(this);
    if (correct) {
        display_icon_answer->setPixmap(QIcon::fromTheme("face-smile", QIcon(getImgPath("face-smile.png")))
                                       .pixmap(2*fontMetrics().height()));
        display_answer->setStyleSheet("QLabel {color : green; }");
	} else {
        display_icon_answer->setPixmap(QIcon::fromTheme("face-sad", QIcon(getImgPath("face-sad.png")))
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
    if (test->get_dst()=="ja" || test->get_dst()=="zh")
        answer_headline_label = new QLabel("<b>"+word+"</b> <i>"+nature+"</i>: "+pronunciation, this);
    else{
        QString answer = "<b>"+word+"</b> <i>"+nature+"</i>: "+meaning;
        if (!pronunciation.isEmpty())
            answer += "\n<b>["+ pronunciation +"]</b>";
        answer_headline_label = new QLabel(answer, this);
    }
    answer_headline_label->setWordWrap(true);
    answer_headline_layout->addWidget(answer_headline_label);

    // Create the OK button
    OK_button = new QPushButton(
                QIcon::fromTheme("emblem-default", QIcon(getImgPath("emblem-default.png"))),
                tr("OK"),
                this);
    OK_button->setFixedSize(2*InterfaceParameters::widget_unit, InterfaceParameters::widget_unit);
    OK_button->setIconSize(QSize(InterfaceParameters::widget_unit/2, InterfaceParameters::widget_unit/2));
    connect(OK_button, SIGNAL(clicked()), parent, SLOT(validate_answer()));
    OK_button->setDefault(true);
    OK_button->setFocus(); // Because the focus is still on the edit line.
    answer_headline_layout->addWidget(OK_button);

    QLabel *comment_label = new QLabel("<i>"+comment.replace(QRegExp("[\r\n]+"), "<br />")+"</i>", this);
    vertical_layout->addWidget(comment_label);
    if(example.compare("")){
        QTextBrowser *example_label = new QTextBrowser(this);
        example_label->setHtml("<b>Example:</b> "+ example.replace(QRegExp("[\r\n]+"), "<br />"));
        //example_label->setWordWrapMode(QTextOption::w);
        vertical_layout->addWidget(example_label);
    }
}
