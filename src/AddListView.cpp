#include "AddListView.h"

#include <QDebug>
#include <QFormLayout>

AddListView::AddListView(DatabaseManager *database_manager, QWidget *parent) :
    QWidget(parent),
    cancel_button(tr("C&ancel"), this),
    create_button(tr("&Create"), this),
    database_manager(database_manager),
    dst_edit(this),
    name_edit(this),
    src_edit(this),
    status(this),
    title(tr("<b>Create a vocabulary list</b>"))
{
    QFormLayout* layout = new QFormLayout(this);
    layout->addWidget(&title);
    layout->addRow(tr("&Name: "), &name_edit);
    layout->addRow(tr("&Source language: "), &src_edit);
    layout->addRow(tr("&Destination language: "), &dst_edit);
    layout->addWidget(&status);
    status.hide();
    layout->addWidget(&create_button);
    layout->addWidget(&cancel_button);
    connect(&create_button, SIGNAL(clicked()), this, SLOT(add_list()));
    connect(this, SIGNAL(add_list_success()), parent, SLOT(remove_add_list_view()));
    connect(&cancel_button, SIGNAL(clicked()), parent, SLOT(remove_add_list_view()));
}

void AddListView::add_list()
{
    database_manager->add_list(name_edit.text(), src_edit.text(), dst_edit.text());
    QString error(database_manager->pop_last_error());
    if(error == "")
        emit add_list_success();
    else{
        status.setText(tr("<b>SQLite error: </b>") + error);
        status.show();
    }
}
