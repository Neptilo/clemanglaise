#ifndef DUPLICATEPAGE_H
#define DUPLICATEPAGE_H

#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>
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
    QSignalMapper signal_mapper;

signals:
    void choose_behavior(int);

public slots:
    QHash<QString, QString> get_word_to_merge();
private slots:
};

#endif // DUPLICATEPAGE_H
