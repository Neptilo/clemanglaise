#include <algorithm>
#include <QtNetwork>
#include <QDebug>
#include <QTextDocument>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#   include <QUrlQuery>
#endif
#include <QStandardItem>
#include <QStandardItemModel>

#include "EditView.h"
#include "InterfaceParameters.h"
#include "QuestionView.h"
#include "string_utils.h"
#include "NetworkReplyReader.h"

EditView::EditView(Test *test, const QString &title, const QHash<QString, QString> &default_values, const QString &OK_button_value, const QString &php_filename, const QString &success_message, DatabaseManager *database_manager, QWidget *parent) :
    QWidget(parent),
    title(nullptr),
    status(nullptr),
    nature_edit(nullptr),
    gender_edit(nullptr),
    nam(),
    tag_nam(),
    word_edit(nullptr),
    meaning_edit(nullptr),
    pronunciation_edit(nullptr),
    comment_edit(nullptr),
    example_edit(nullptr),
    hint_edit(nullptr),
    OK_button(nullptr),
    continue_button(nullptr),
    layout(nullptr),
    php_filename(php_filename),
    default_values(default_values),
    reply_list(),
    success_message(success_message),
    database_manager(database_manager),
    test(test)
{
    layout = new QFormLayout(this);

    this->title = new QLabel(title, this);
    layout->addWidget(this->title);

    // Define explicit variables for the default values of the fields.
    QString word = ampersand_unescape(default_values["word"]);
    QString meaning = ampersand_unescape(default_values["meaning"]);
    QString nature = default_values["nature"];
    QString comment = default_values["comment"];
    QString example = default_values["example"];
    QString hint = default_values["hint"];
    QString pronunciation = ampersand_unescape(default_values["pronunciation"]);

    word_edit = new QLineEdit(word, this);
    layout->addRow(tr("&Word: "), word_edit);

    nature_edit = new QComboBox(this);
    nature_edit->addItem("---");
    nature_edit->addItem(tr("Adjective"), QVariant("adj"));
    nature_edit->addItem(tr("Adverb"), QVariant("adv"));
    nature_edit->addItem(tr("Article"), QVariant("art"));
    nature_edit->addItem(tr("Classifier"), QVariant("clas"));
    nature_edit->addItem(tr("Conjunction"), QVariant("conj"));
    nature_edit->addItem(tr("Interjection"), QVariant("inter"));
    nature_edit->addItem(tr("Noun"), QVariant("n"));
    nature_edit->addItem(tr("Particle"), QVariant("part"));
    nature_edit->addItem(tr("Phrase"), QVariant("phras"));
    nature_edit->addItem(tr("Preposition"), QVariant("prep"));
    nature_edit->addItem(tr("Pronoun"), QVariant("pron"));
    nature_edit->addItem(tr("Verb"), QVariant("v"));
    QChar gender;
    QString part_of_speech;
    if (nature.size() > 1 && nature.at(0) == QChar('n')) {
        part_of_speech = nature.at(0);
        gender = nature.at(1);
    } else
        part_of_speech = nature;
    nature_edit->setCurrentIndex(nature_edit->findData(QVariant(part_of_speech)));

    layout->addRow(tr("Part of &speech: "), nature_edit);
    connect(nature_edit, SIGNAL(currentIndexChanged(int)), this, SLOT(update_gender(int)));

    QList<QChar> genders;
    if (
            test->get_dst() == "de" ||
            test->get_dst() == "hr" ||
            test->get_dst() == "ru")
        genders << QChar('m') << QChar('f') << QChar('n');
    else if (
             test->get_dst() == "ar" ||
             test->get_dst() == "ca" ||
             test->get_dst() == "es" ||
             test->get_dst() == "fr")
        genders << QChar('m') << QChar('f');
    if (!genders.isEmpty()) {
        gender_edit = new QComboBox(this);
        gender_edit->addItem("---");
        for (QList<QChar>::iterator i = genders.begin(); i != genders.end(); ++i) {
            switch ((*i).cell()) {
            case 'm':
                gender_edit->addItem(tr("Masculine"), QVariant(*i));
                break;
            case 'f':
                gender_edit->addItem(tr("Feminine"), QVariant(*i));
                break;
            case 'n':
                gender_edit->addItem(tr("Neuter"), QVariant(*i));
                break;
            }
        }
        if (!gender.isNull())
            gender_edit->setCurrentIndex(gender_edit->findData(QVariant(gender)));
        update_gender(nature_edit->currentIndex());
    }

    meaning_edit = new QLineEdit(meaning, this);
    layout->addRow(tr("&Translation: "), meaning_edit);

    if (
            test->get_dst() != "de" &&
            test->get_dst() != "ca" &&
            test->get_dst() != "es" &&
            test->get_dst() != "hr" &&
            test->get_dst() != "ru") {
        pronunciation_edit = new QLineEdit(pronunciation, this);
        layout->addRow(tr("&Pronunciation: "), pronunciation_edit);
    }

    comment_edit = new QTextEdit(this);
    comment_edit->setPlainText(comment); // so the text will be interpreted as plain text, not HTML
    layout->addRow(tr("&Comment: "), comment_edit);

    example_edit = new QTextEdit(this);
    example_edit->setPlainText(example);
    layout->addRow(tr("&Example: "), example_edit);

    hint_edit = new QTextEdit(this);
    hint_edit->setPlainText(hint);
    layout->addRow(tr("&Hint: "), hint_edit);

    tags_box = new CheckableComboBox(this);
    tags_box->setFixedHeight(InterfaceParameters::widget_unit);
    tags_box->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    layout->addRow(tr("T&ags: "), tags_box);
    find_tags();
    connect(&tag_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_reply(QNetworkReply*)));

    status = new QLabel(this);
    layout->addWidget(status);

    nam.setCookieJar(NetworkReplyReader::cookie_jar); // By default, nam takes ownership of the cookie jar.
    nam.cookieJar()->setParent(nullptr); // Unset the cookie jar's parent so it is not deleted when nam is deleted, and can still be used by other NAMs.

	OK_button = new QPushButton(OK_button_value, this);
    OK_button->setIcon(QIcon::fromTheme("emblem-default", QIcon(getImgPath("emblem-default.png"))));

	connect(OK_button, SIGNAL(clicked()), this, SLOT(edit_word()));
	layout->addWidget(OK_button);
}

EditView::~EditView(){}

void EditView::edit_word(){
	status->setText(tr("Sending data..."));

    // Standardize pronunciation to save into database
    QString standardized_pronunciation;
    if (pronunciation_edit) {
        if(test->get_dst() == "ja"){
            standardized_pronunciation = ampersand_escape(pronunciation_edit->text());
            standardized_pronunciation.replace(QString("ou"), QString("&#333;"));
            standardized_pronunciation.replace(QString("uu"), QString("&#363;"));
            standardized_pronunciation.replace(QString("aa"), QString("&#257;"));
            standardized_pronunciation.replace(QString("ee"), QString("&#275;"));
        }else if(test->get_dst() == "zh"){
            standardized_pronunciation = numbers_to_accents(pronunciation_edit->text());
        } else {
            standardized_pronunciation = isKirshenbaum(pronunciation_edit->text())?ampersand_escape(kirshenbaum2IPA(pronunciation_edit->text())):ampersand_escape(pronunciation_edit->text());
        }
    }

    // Define data to send
    QHash<QString, QString> word_data;
    word_data["id"] = default_values["id"];
    word_data["list_id"] = QString::number(test->get_id());
    word_data["word"] = ampersand_escape(word_edit->text());
    QString nature(nature_edit->itemData(nature_edit->currentIndex()).toString());
    if (nature == "n" && gender_edit) {
        QVariant gender = gender_edit->itemData(gender_edit->currentIndex());
        if (gender.isValid())
            nature += gender.toChar();
    }
    word_data["nature"] = nature;
    word_data["meaning"] = ampersand_escape(meaning_edit->text());
    word_data["pronunciation"] = standardized_pronunciation;
    // toPlainText() because we don't want to save a too much unnecessary information like an HTML header.
    word_data["comment"] = ampersand_escape(comment_edit->toPlainText());
    word_data["example"] = ampersand_escape(example_edit->toPlainText());
    word_data["hint"] = ampersand_escape(hint_edit->toPlainText());

    if (!test->is_remote()) {
		bool success;

        // Offline
        if(word_data["id"].toInt() == 0) // Add word
            success = database_manager->add_word(word_data, selected_tags);
        else // Update word
            success = database_manager->update_word(word_data, selected_tags);

        // Show confirmation
        show_confirmation(success);
	} else {
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
		QUrl post_data;
#else
		QUrlQuery post_data;
#endif
        post_data.addQueryItem("list_id", QString::number(test->get_id()));
        for (QHash<QString, QString>::iterator i = word_data.begin(); i != word_data.end(); ++i)
            post_data.addQueryItem(i.key(), i.value());
        for (int i = 0; i < selected_tags.size(); ++i)
            post_data.addQueryItem("tag_ids[]", QString::number(selected_tags.at(i)));
		const QUrl url("https://neptilo.com/php/clemanglaise/"+this->php_filename+".php");
		QNetworkRequest request(url);
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

		// Will show confirmation when loading of reply is finished
		connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(show_confirmation(QNetworkReply*)));

		// Send the request
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
		nam.post(request, post_data.encodedQuery());
#else
		nam.post(request, post_data.query(QUrl::FullyEncoded).toUtf8());
#endif
	}
	disable_edition(true);
}

void EditView::show_confirmation(QNetworkReply* reply){
    const QString reply_string(reply->readAll().replace('\0', ""));
    reply->deleteLater();
    if(reply_string.compare(""))
        status->setText(reply_string);
    else{
        status->setText(success_message);
        prepare_to_continue();
    }
}

void EditView::show_confirmation(bool success){
    if(success){
        status->setText(success_message);
        prepare_to_continue();
    }else
        status->setText(tr("<b>SQLite error: </b>")+database_manager->pop_last_error());
}


void EditView::update_selected_tags(QModelIndex top_left, QModelIndex)
{
    QMap<int, QVariant> item_data = tags_box->model()->itemData(top_left);
    int variant = item_data[Qt::UserRole].toInt();
    switch (item_data[Qt::CheckStateRole].toInt()) {
    case Qt::Checked:
        if (variant != 0)
            selected_tags << variant;
        break;
    case Qt::Unchecked:
        selected_tags.removeOne(variant);
        break;
    default:
        qDebug() << tr("Wrong check state value");
        break;
    }
}

void EditView::reset(){
    word_edit->setText(default_values["word"]);
    meaning_edit->setText(default_values["meaning"]);
    QString nature = default_values["nature"];
    QChar gender;
    QString part_of_speech;
    if (nature.size() > 1 && nature.at(0) == QChar('n')) {
        part_of_speech = nature.at(0);
        gender = nature.at(1);
    } else
        part_of_speech = nature;
    nature_edit->setCurrentIndex(nature_edit->findData(QVariant(part_of_speech)));
    if (gender_edit && !gender.isNull())
        gender_edit->setCurrentIndex(gender_edit->findData(QVariant(gender)));
    comment_edit->setPlainText(default_values["comment"]);
    example_edit->setPlainText(default_values["example"]);
    hint_edit->setPlainText(default_values["hint"]);
    if (pronunciation_edit)
        pronunciation_edit->setText(default_values["pronunciation"]);

	delete continue_button;

    title->setText(tr("<b>Add a new word</b>"));
	OK_button = new QPushButton(tr("Add word"), this);
	OK_button->setIcon(QIcon::fromTheme("emblem-default", QIcon(":/emblem-default.png")));
	connect(OK_button, SIGNAL(clicked()), this, SLOT(edit_word()));
	layout->addWidget(OK_button);
	disable_edition(false);
}

void EditView::find_tags() {
    if (!test->is_remote()) {
		// Offline
		database_manager->find_tags(reply_list);
		read_reply();
    } else {
		// Request to PHP file
        const QUrl url = QUrl("https://neptilo.com/php/clemanglaise/find_tags.php");
		QNetworkRequest request(url);
        tag_nam.get(request);
    }
}

void EditView::update_gender(int index)
{
    if (gender_edit) {
        if (nature_edit->itemData(index) == QVariant("n")) {
            int index;
            layout->getWidgetPosition(nature_edit, &index, nullptr);
            layout->insertRow(index+1, tr("&Gender: "), gender_edit);
            gender_edit->show();
        } else {
            gender_edit->hide();
            if (QWidget *w = layout->labelForField(gender_edit))
                w->deleteLater();
        }
    }
}

void EditView::read_reply(QNetworkReply* reply)
{
	// Store the lines of the reply in the "reply_list" attribute
    QString reply_string = reply->readAll().replace('\0', "");
	reply->deleteLater();
	read_reply(reply_string);
}


void EditView::read_reply(QString reply_string) {
    if(test->is_remote())
		reply_list = reply_string.split('\n', QString::SkipEmptyParts);

    //Initialiaze selected tags context
    QStringList tag_ids = default_values["tag_ids"].split(", ", QString::SkipEmptyParts);
    for (QStringList::const_iterator it = tag_ids.constBegin(); it != tag_ids.constEnd(); ++it)
        selected_tags << it->toInt();
    tags_box->disconnect();
    tags_box->clear();
    int l = reply_list.size()/2;
    QStandardItemModel *model = new QStandardItemModel(l+1, 1);
    QStandardItem* item = new QStandardItem(tr("---"));
    item->setSizeHint(QSize(0, InterfaceParameters::widget_unit));
    model->setItem(0, 0, item);
    for (int i = 0; i < l; ++i) {
        item = new QStandardItem(reply_list.at(2*i+1).trimmed());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        if(tag_ids.contains(reply_list.at(2*i))) // select tags that belong to the word
            item->setData(Qt::Checked, Qt::CheckStateRole);
        else
            item->setData(Qt::Unchecked, Qt::CheckStateRole);
        item->setData(QVariant(reply_list.at(2*i).toInt()), Qt::UserRole);
        model->setItem(i+1, 0, item);
    }
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(update_selected_tags(QModelIndex,QModelIndex)));
    tags_box->setModel(model);
}

void EditView::disable_edition(bool ok) {
	word_edit->setEnabled(!ok);
	nature_edit->setEnabled(!ok);
    if (gender_edit)
        gender_edit->setEnabled(!ok);
	meaning_edit->setEnabled(!ok);
    if (pronunciation_edit)
        pronunciation_edit->setEnabled(!ok);
	comment_edit->setEnabled(!ok);
	example_edit->setEnabled(!ok);
    hint_edit->setEnabled(!ok);
    tags_box->setEnabled(!ok);
}

void EditView::prepare_to_continue()
{
    // Once a word has been updated, the default values are those of the next word we might add, ie, empty values.
    QStringList word_keys;
    word_keys << "id" << "word" << "meaning" << "nature" << "comment" << "example" << "pronunciation" << "score" << "hint" << "tag_ids";
    for(int i = 0; i < word_keys.size(); ++i)
        default_values[word_keys.at(i)] = "";
    php_filename = "add_word";
    success_message = tr("Word successfully added!");
    delete OK_button;
    continue_button = new QPushButton(tr("Add another word"), this);
    continue_button->setIcon(QIcon::fromTheme("list-add", QIcon(getImgPath("list-add.png"))));

    layout->addWidget(continue_button);
    connect(continue_button, SIGNAL(clicked()), this, SLOT(reset()));
}
