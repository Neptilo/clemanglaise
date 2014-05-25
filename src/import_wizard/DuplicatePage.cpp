#include "import_wizard/DuplicatePage.h"

#include <QButtonGroup>
#include <QDebug>
#include <QHeaderView>
#include <QLabel>
#include <QRadioButton>

#include "string_utils.h"

#include <import_wizard/SingleImportWizard.h>

DuplicatePage::DuplicatePage(const QHash<QString, QString> &word_data, QWidget *parent) :
    QWizardPage(parent),
    word_data(word_data),
    layout(this),
    info(this),
    duplicate_table(this)
{
    layout.addWidget(&info);

    layout.addWidget(&duplicate_table);

    QHBoxLayout *button_layout = new QHBoxLayout();
    layout.addLayout(button_layout); // layout is now button_layout's parent.

    // radio buttons
    QButtonGroup *button_group = new QButtonGroup(this);

    QRadioButton *radio;
    radio = new QRadioButton(tr("Import anyway"), this);
    radio->setToolTip(tr("The word will be inserted as is."));
    button_group->addButton(radio);

    radio = new QRadioButton(tr("Merge"), this);
    radio->setToolTip(tr("Merge data from the original and the new version of the word."));
    radio->setChecked(true);
    button_group->addButton(radio);
    // TODO: repeat previous block for each possible behavior

    QList<QAbstractButton *> radios = button_group->buttons();
    for(int i = 0; i < radios.length(); ++i)
    {
        QAbstractButton *radio = radios.at(i);
        signal_mapper.setMapping(radio, i);
        connect(radio, SIGNAL(clicked()), &signal_mapper, SLOT(map()));
        layout.addWidget(radio);
    }
    connect(&signal_mapper, SIGNAL(mapped(int)), this, SIGNAL(choose_behavior(int)));
}

void DuplicatePage::initializePage()
{
    if(duplicate_values.empty()){
        qDebug() << tr("This function should not be called if duplicates are empty.");
        return;
    }

    duplicate_table.clear();
    QStringList table_keys;
    table_keys << "word" << "meaning" << "nature" << "pronunciation" << "comment" << "example";
    QStringList header_labels;
    header_labels << tr("Word") << tr("Meaning") << tr("Nature") << tr("Pronunciation") << tr("Comment") << tr("Example");
    int nb_cols = header_labels.size(), nb_rows = duplicate_values.size();
    info.setText(tr("There are <b>%1</b> possible duplicate(s) in the chosen list.").arg(nb_rows));
    duplicate_table.setColumnCount(nb_cols);
    duplicate_table.setRowCount(nb_rows);
    duplicate_table.setHorizontalHeaderLabels(header_labels);
    duplicate_table.verticalHeader()->hide();
    for(int i = 0; i < nb_cols; ++i){
        QString cur_key = table_keys.at(i);
        int key_ind = duplicate_keys.indexOf(cur_key);

        for(int j = 0; j < nb_rows; ++j){
            QWidget* item;
            QString text = ampersand_unescape(duplicate_values.at(j).at(key_ind)).replace(QRegExp("[\r\n]+"), "<br />");
            if(i == 0)
                item = new QRadioButton(text, this);
            else
                item = new QLabel(text, this);
            duplicate_table.setCellWidget(j, i, item);
        }

    }
    duplicate_table.resizeColumnsToContents();
    duplicate_table.resizeRowsToContents();

    // guess the most probable duplicate and check its radio button by default
    QStringList relevant_keys; // keys of relevant fields to compute similarity between imported entry and possible duplicate
    relevant_keys << "word" << "meaning";
    int best_similarity = 0; // Similarity is defined as the number of words and meanings in common between two entries.
    for(int i = 0; i < nb_rows; ++i){
        int similarity = 0;
        for(int j = 0; j < relevant_keys.size(); ++j){
            // get word list of imported entry
            QStringList import_word_list = trimmed(word_data[relevant_keys.at(j)].split(","));
            int key_ind = duplicate_keys.indexOf(relevant_keys.at(j));
            // get the word list and count elements in common with input word list
            QStringList dup_word_list = duplicate_values.at(i).at(key_ind).split(",");
            for(int k = 0; k < dup_word_list.size(); ++k){
                if(import_word_list.contains(dup_word_list.at(k).trimmed(), Qt::CaseInsensitive))
                    ++similarity;
            }
        }
        if(similarity > best_similarity){
            best_similarity = similarity;
            QRadioButton *radio = (QRadioButton *) duplicate_table.cellWidget(i, 0);
            radio->setChecked(true);
        }
    }
}

QHash<QString, QString> DuplicatePage::get_word_to_merge(){
    QHash<QString, QString> word_to_merge_data;
    for(int i = 0; i < duplicate_values.size(); ++i){
        QRadioButton *radio = (QRadioButton *) duplicate_table.cellWidget(i, 0);
        if(radio->isChecked()){
            for(int j = 0; j < duplicate_keys.size(); ++j){
                word_to_merge_data[duplicate_keys.at(j)] = duplicate_values.at(i).at(j);
            }
            break; // We found the checked radio button.
        }
    }
    return word_to_merge_data;
}
