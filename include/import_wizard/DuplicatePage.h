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
    explicit DuplicatePage(const QHash<QString, QString> &word_data, QWidget *parent = 0);
    void initializePage();
    QStringList duplicate_keys;
    QList<QStringList> duplicate_values;

private:
    const QHash<QString, QString> &word_data;
    QVBoxLayout layout;
    QLabel info;
    QTableWidget duplicate_table;
    QPushButton import_button;
    QPushButton merge_button;

signals:
    void merge_word(const QHash<QString, QString> &);

public slots:
private slots:
    void select_word_to_merge();
};

#endif // DUPLICATEPAGE_H
