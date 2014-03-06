#ifndef DUPLICATEPAGE_H
#define DUPLICATEPAGE_H

#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWizardPage>

class DuplicatePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DuplicatePage(QWidget *parent = 0);
    void initializePage();
    QStringList duplicate_keys;
    QList<QStringList> duplicate_values;

private:
    QVBoxLayout layout;
    QLabel info;
    QTableWidget duplicate_table;
    QPushButton import_button;

signals:

public slots:

};

#endif // DUPLICATEPAGE_H
