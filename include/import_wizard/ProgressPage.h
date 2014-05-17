#ifndef PROGRESSPAGE_H
#define PROGRESSPAGE_H

#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QWizardPage>

class ProgressPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ProgressPage(QWidget *parent = 0);
    void set_status(QString text);
    void initializePage();
private:
    QVBoxLayout layout;
    QLabel status;
    QProgressBar progress_bar;

signals:
    void import_list();

public slots:

};

#endif // PROGRESSPAGE_H
