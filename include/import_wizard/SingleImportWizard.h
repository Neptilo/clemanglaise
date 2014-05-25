#ifndef SINGLEIMPORTWIZARD_H
#define SINGLEIMPORTWIZARD_H

#include <QWizard>

#include "import_wizard/DstListPage.h"
#include "import_wizard/DuplicatePage.h"
#include "import_wizard/Importer.h"

class SingleImportWizard : public QWizard, public Importer
{
    Q_OBJECT
public:
    explicit SingleImportWizard(DatabaseManager *database_manager, const QHash<QString, QString> &word_data, QWidget *parent = 0);    
    void showEvent(QShowEvent *);
private:
    QString merge_string(
            const QString &left_string,
            const QString &right_string,
            const QRegExp &split_sep,
            const QString &join_sep);
    QHash<QString, QString> word_data;
    DstListPage dst_list_page;
    DuplicatePage duplicate_page;
    int chosen_behavior;

signals:

public slots:
    bool import_word();
    bool merge_word(const QHash<QString, QString> &word_to_merge_data);
    void choose_behavior(int behavior);

private slots:
    void check_duplicates(Test *test);
    void accept();
};

#endif // SINGLEIMPORTWIZARD_H
