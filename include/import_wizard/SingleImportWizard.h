#ifndef SINGLEIMPORTWIZARD_H
#define SINGLEIMPORTWIZARD_H

#include <QWizard>

#include "DatabaseManager.h"
#include "import_wizard/DstListPage.h"
#include "import_wizard/DuplicatePage.h"

class SingleImportWizard : public QWizard
{
    Q_OBJECT
public:
    explicit SingleImportWizard(DatabaseManager *database_manager, QStringList &word_entry, QWidget *parent = 0);

private:
    DatabaseManager *database_manager;
    QStringList word_entry;
    int dst_test_id;
    DstListPage dst_list_page;
    DuplicatePage duplicate_page;

signals:

public slots:
    void import_word();

private slots:
    void check_duplicates(Test *test);
};

#endif // SINGLEIMPORTWIZARD_H