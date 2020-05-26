#ifndef SINGLEIMPORTWIZARD_H
#define SINGLEIMPORTWIZARD_H

#include <QWizard>

#include "import_wizard/DstListPage.h"
#include "import_wizard/DuplicatePage.h"
#include "import_wizard/ImportBehavior.h"
#include "import_wizard/Importer.h"
#include "import_wizard/ProgressPage.h"

class SingleImportWizard : public QWizard, public Importer
{
    enum {Page_DstList, Page_Progress, Page_Duplicates};
    Q_OBJECT
public:
    // dst_test: pointer to a predefined vocab test, nullptr otherwise
    explicit SingleImportWizard(DatabaseManager *database_manager, const QHash<QString, QString> &word_data, Test *dst_test, QWidget *parent = nullptr);
    ImportBehavior::Behavior chosen_behavior;
    void showEvent(QShowEvent *) override;
    bool validateCurrentPage() const;
private:
    QHash<QString, QString> word_data;
    DstListPage *dst_list_page;
    DuplicatePage duplicate_page;
    ProgressPage progress_page;
    Test *dst_test;
    void find_tags();
    int nextId() const override;
signals:

public slots:
    // update word_data in database
    bool update_word(const QHash<QString, QString> &word_data);
    void choose_behavior(ImportBehavior::Behavior behavior);

private slots:
    void check_duplicates(Test *test);
    void read_tag_reply();
    void import_tags_and_word();
    void update_on_complete();
};

#endif // SINGLEIMPORTWIZARD_H
