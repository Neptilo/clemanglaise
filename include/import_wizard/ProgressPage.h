#ifndef PROGRESSPAGE_H
#define PROGRESSPAGE_H

#include <QLabel>
#include <QProgressBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWizardPage>

class ProgressPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ProgressPage(QWidget *parent = nullptr);
    void set_status(QString text);
    void initializePage();
    bool isComplete() const;
    void set_max_progress(int maximum);
    void increase_progress(int value = 1);
    void append_log(const QString &message);
private:
    QVBoxLayout layout;
    QLabel status;
    QProgressBar progress_bar;
    QTextEdit log;

signals:
    void ready();

public slots:    
};

#endif // PROGRESSPAGE_H
