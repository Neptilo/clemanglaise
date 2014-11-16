#include "CheckableItemDelegate.h"

CheckableItemDelegate::CheckableItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void CheckableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem new_option(option);
    new_option.showDecorationSelected = false;
    QStyledItemDelegate::paint(painter, new_option, index);
}
