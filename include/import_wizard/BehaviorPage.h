#ifndef BEHAVIORPAGE_H
#define BEHAVIORPAGE_H

#include <QLabel>
#include <QRadioButton>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QWizardPage>

class BehaviorPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit BehaviorPage(QWidget *parent = nullptr);
    void initializePage();
private:
    QVBoxLayout layout;
    QLabel question;
    QSignalMapper signal_mapper;

signals:
    void choose_behavior(int);
    void next();
public slots:

private slots:
};

#endif // BEHAVIORPAGE_H
