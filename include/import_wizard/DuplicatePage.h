#ifndef DUPLICATEPAGE_H
#define DUPLICATEPAGE_H

#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWizardPage>

#include <import_wizard/ImportBehavior.h>

class DuplicatePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DuplicatePage(QWidget *parent = nullptr);
    void initializePage();
    bool isComplete() const;
    const QHash<QString, QString> *word_data; // pointer to a constant object
    QStringList duplicate_keys;
    QList<QStringList> duplicate_values;
private:
    QVBoxLayout layout;
    QLabel info;
    QTableWidget duplicate_table;
    QButtonGroup button_group;
    QSignalMapper signal_mapper;

signals:
    void choose_behavior(ImportBehavior::Behavior);

public slots:
    QHash<QString, QString> get_word_to_merge();
    void choose_behavior_int(int behavior);
private slots:
};

#endif // DUPLICATEPAGE_H
