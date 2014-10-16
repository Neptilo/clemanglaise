#ifndef LISTIMPORTWIZARD_H
#define LISTIMPORTWIZARD_H

#include <QNetworkAccessManager>
#include <QWizard>

#include "AddListView.h"
#include "DatabaseManager.h"
#include "DuplicatePage.h"
#include "import_wizard/BehaviorPage.h"
#include "import_wizard/Importer.h"
#include "import_wizard/DstListPage.h"
#include "import_wizard/ProgressPage.h"

class ListImportWizard : public QWizard, public Importer
{
    Q_OBJECT
    enum {Page_DstList, Page_Behavior, Page_Progress, Page_Duplicates};
public:
    explicit ListImportWizard(DatabaseManager *database_manager, Test *src_test, QWidget *parent = 0);
    void showEvent(QShowEvent *);
    int nextId() const;
private:
    Test *src_test;
    Test *dst_test;
    DstListPage dst_list_page;
    BehaviorPage behavior_page;
    ProgressPage progress_page;
    int chosen_behavior;
    QNetworkAccessManager nam;
    int nb_inserted;
    int nb_replaced;
    int nb_updated;
    int nb_discarded;
    int nb_failed;

signals:

public slots:

private slots:
    void import_list();
    void save_and_next(Test *test);
    void choose_behavior(int behavior);
    void read_reply(QNetworkReply *reply);
    void update_on_complete();
};

#endif // LISTIMPORTWIZARD_H
