#include <QDebug>

#include "import_wizard/ProgressPage.h"

ProgressPage::ProgressPage(QWidget *parent) :
    QWizardPage(parent),
    layout(this),
    status(this),
    progress_bar(this)
{
    layout.addWidget(&status);
    layout.addWidget(&progress_bar);
    progress_bar.hide(); // TODO: show it and update its content
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
