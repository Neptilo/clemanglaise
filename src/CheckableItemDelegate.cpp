#include "CheckableItemDelegate.h"

#include <QDebug>
#include <QEvent>

CheckableItemDelegate::CheckableItemDelegate(QObject *parent) :
    QItemDelegate(parent),
    enabled(false)
{
}

bool CheckableItemDelegate::editorEvent(
        QEvent *event,
        QAbstractItemModel *model,
        const QStyleOptionViewItem &option,
        const QModelIndex &index)
{
    Q_UNUSED(option);
    if (event->type() != QEvent::MouseButtonRelease)
        return false;

    if (enabled) {
        QVariant value = index.data(Qt::CheckStateRole);
        Qt::CheckState state =
                (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked ?
                     Qt::Unchecked : Qt::Checked);
        return model->setData(index, state, Qt::CheckStateRole);
    } else {
        set_enabled(true);
        return false;
    }
}

void CheckableItemDelegate::set_enabled(bool on)
{
    enabled = on;
}
