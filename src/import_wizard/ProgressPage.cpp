#include <QDebug>

#include "import_wizard/ProgressPage.h"

ProgressPage::ProgressPage(QWidget *parent) :
    QWizardPage(parent),
    layout(this),
    status(this),
    progress_bar(this),
    log(this)
{
    status.setWordWrap(true);
    layout.addWidget(&status);
    progress_bar.setValue(0);
    layout.addWidget(&progress_bar);
    layout.addWidget(&log);
    log.setTextInteractionFlags(Qt::TextSelectableByMouse);
}

void ProgressPage::set_status(QString text)
{
    status.setText(text);
}

void ProgressPage::initializePage()
{
    // start import
    status.setText(tr("Starting up"));
    emit ready();
}

bool ProgressPage::isComplete() const
{
    return progress_bar.value() == progress_bar.maximum();
}

void ProgressPage::set_max_progress(int maximum)
{
    progress_bar.setMaximum(maximum);
}

void ProgressPage::increase_progress(int value)
{
    progress_bar.setValue(progress_bar.value()+value);
    emit completeChanged();
}

void ProgressPage::append_log(const QString &message)
{
    log.setText(log.toHtml()+message+"<br/>");
}
