#ifndef SINGLEIMPORTWIZARD_H
#define SINGLEIMPORTWIZARD_H

#include <QWizard>

#include "import_wizard/DstListPage.h"
#include "import_wizard/DuplicatePage.h"
#include "import_wizard/ImportBehavior.h"
#include "import_wizard/Importer.h"

class SingleImportWizard : public QWizard, public Importer
{
    Q_OBJECT
public:
    explicit SingleImportWizard(DatabaseManager *database_manager, const QHash<QString, QString> &word_data, Test *dst_test, QWidget *parent = 0);
    ImportBehavior::Behavior chosen_behavior;
    void showEvent(QShowEvent *);
    bool validateCurrentPage() const;
private:
    QHash<QString, QString> word_data;
    DstListPage *dst_list_page;
    DuplicatePage duplicate_page;
    Test *dst_test;

signals:

public slots:
    bool import_word();
    // merge word_data with word_to_merge_data and update it in database
    bool update_word(const QHash<QString, QString> &word_to_merge_data);
    void choose_behavior(ImportBehavior::Behavior behavior);

private slots:
    void check_duplicates(Test *test);
    void accept();
};

#endif // SINGLEIMPORTWIZARD_H
