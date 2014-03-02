#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "DatabaseManager.h"
#include "LanguageButtons.h"

class ImportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ImportDialog(DatabaseManager *database_manager, QWidget *parent = 0);
    int dst_test_id;

private:
    QVBoxLayout layout;
    QLabel question;
    LanguageButtons test_buttons;
    QPushButton cancel_button;

signals:

public slots:
    void choose_test_list(QObject *obj);

};

#endif // IMPORTDIALOG_H
