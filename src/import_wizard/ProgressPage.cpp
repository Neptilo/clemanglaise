#include <QDebug>

#include "import_wizard/ProgressPage.h"

ProgressPage::ProgressPage(QWidget *parent) :
    QWizardPage(parent),
    layout(this),
    status(this),
    progress_bar(this)
{
    layout.addWidget(&status);
    progress_bar.setValue(0);
    layout.addWidget(&progress_bar);
}

void ProgressPage::set_status(QString text)
{
    status.setText(text);
}

void ProgressPage::initializePage()
{
    // start import
    status.setText(tr("Sending request to server"));
    emit import_list();
}

void ProgressPage::set_max_progress(int maximum)
{
    progress_bar.setMaximum(maximum);
}

void ProgressPage::increase_progress(int value)
{
    progress_bar.setValue(progress_bar.value()+value);
}