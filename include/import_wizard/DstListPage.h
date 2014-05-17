#ifndef DSTLISTPAGE_H
#define DSTLISTPAGE_H

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWizardPage>

#include "DatabaseManager.h"
#include "LanguageButtons.h"

class DstListPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DstListPage(DatabaseManager *database_manager, QWidget *parent = 0);    
    bool isComplete();
private:
    QVBoxLayout layout;
    QLabel question;
    LanguageButtons test_buttons;

signals:
    void clicked(Test *test);

public slots:
    void forward_click(Test *test);
};

#endif // DSTLISTPAGE_H
