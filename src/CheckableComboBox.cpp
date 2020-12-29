#include "CheckableComboBox.h"

#include <QStylePainter>

#include "CheckableItemDelegate.h"

CheckableComboBox::CheckableComboBox(QWidget *parent):
    QComboBox(parent)
{
    CheckableItemDelegate *delegate = new CheckableItemDelegate(parent);
    setItemDelegate(delegate);
}

void CheckableComboBox::paintEvent(QPaintEvent*)
{
    QStylePainter painter(this);

    QStyleOptionComboBox opt;
    this->initStyleOption(&opt);
    opt.currentText = m_title;
    painter.drawComplexControl(QStyle::CC_ComboBox, opt);
    painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}

void CheckableComboBox::set_title(const QString& title)
{
    m_title = title;
}
