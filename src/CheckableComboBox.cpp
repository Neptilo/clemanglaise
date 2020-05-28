#include "CheckableComboBox.h"

#include "CheckableItemDelegate.h"

CheckableComboBox::CheckableComboBox(QWidget *parent):
    QComboBox(parent)
{
    CheckableItemDelegate *delegate = new CheckableItemDelegate(parent);
    setItemDelegate(delegate);
}

void CheckableComboBox::hidePopup()
{
    QComboBox::hidePopup();
    dynamic_cast<CheckableItemDelegate *>(itemDelegate())->set_enabled(false);
}
