#include "import_wizard/DuplicatePage.h"

#include <QDebug>
#include <QHeaderView>
#include <QLabel>

#include "string_utils.h"

DuplicatePage::DuplicatePage(QWidget *parent) :
    QWizardPage(parent),
    layout(this),
    info(this),
    duplicate_table(this),
    import_button(tr("Import anyway"), this)
{
    layout.addWidget(&info);
    layout.addWidget(&duplicate_table);
    connect(&import_button, SIGNAL(clicked()), this->parent(), SLOT(import_word()));
    layout.addWidget(&import_button);
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
    int nb_cols = table_keys.size(), nb_rows = duplicate_values.size();
    info.setText(tr("There are <b>%1</b> possible duplicates in the chosen list.").arg(nb_rows));
    duplicate_table.setColumnCount(nb_cols);
    duplicate_table.setRowCount(nb_rows);
    QStringList header_labels;
    header_labels << tr("Word") << tr("Meaning") << tr("Nature") << tr("Pronunciation") << tr("Comment") << tr("Example");
    duplicate_table.setHorizontalHeaderLabels(header_labels);
    duplicate_table.verticalHeader()->hide();
    for(int i = 0; i < nb_cols; ++i){
        int key_ind = duplicate_keys.indexOf(table_keys.at(i));
        for(int j = 0; j < nb_rows; ++j){
            QLabel* item = new QLabel(ampersand_unescape(duplicate_values.at(j).at(key_ind)).replace("\n", "<br />"), this); // TODO: what about \r?
            duplicate_table.setCellWidget(j, i, item);
        }
    }
    duplicate_table.resizeColumnsToContents();
    duplicate_table.resizeRowsToContents();
}
