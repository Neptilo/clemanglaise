#include "import_wizard/BehaviorPage.h"
#include "import_wizard/ImportBehavior.h"

#include <QLabel>
#include <QRadioButton>

BehaviorPage::BehaviorPage(QWidget *parent) :
    QWizardPage(parent),
    layout(this),
    question(tr("During the import, duplicates of existing words can be detected. What do you want to do?"), this)
{
    layout.addWidget(&question);

    QList<QRadioButton *> radios;
    QRadioButton *radio;

    radio = new QRadioButton(tr("Don't check for duplicates"), this);
    radio->setToolTip(tr("Every word will be inserted as is."));
    signal_mapper.setMapping(radio, ImportBehavior::DontCheck);
    radios << radio;

    radio = new QRadioButton(tr("Don't import duplicates"), this);
    radio->setToolTip(tr("If a duplicate is found, only the original version will be kept."));
    signal_mapper.setMapping(radio, ImportBehavior::Discard);
    radios << radio;

    radio = new QRadioButton(tr("Replace duplicates"), this);
    radio->setToolTip(tr("If a duplicate is found, the new version will replace the existing word."));
    signal_mapper.setMapping(radio, ImportBehavior::Replace);
    radios << radio;

    radio = new QRadioButton(tr("Merge (recommended)"), this);
    radio->setToolTip(tr("If a duplicate is found, merge data from the original and the new version."));
    radio->setChecked(true);
    signal_mapper.setMapping(radio, ImportBehavior::Merge);
    radios << radio;

    radio = new QRadioButton(tr("Always ask"), this);
    radio->setToolTip(tr("If a duplicate is found, ask what to do."));
    signal_mapper.setMapping(radio, ImportBehavior::Ask);
    radios << radio;

    for(int i = 0; i < radios.length(); ++i)
    {
        QRadioButton *radio = radios.at(i);
        connect(radio, SIGNAL(clicked()), &signal_mapper, SLOT(map()));
        layout.addWidget(radio);
    }
    connect(&signal_mapper, SIGNAL(mapped(int)), this, SIGNAL(choose_behavior(int)));
}

void BehaviorPage::initializePage(){
    // If we enter this page, the default behavior is Merge.
    emit choose_behavior(3);
}
