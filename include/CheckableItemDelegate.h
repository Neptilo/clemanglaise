#ifndef CHECKABLEITEMDELEGATE_H
#define CHECKABLEITEMDELEGATE_H

#include <QItemDelegate>

// This delegate allows the whole items to be checkable (not only the checkboxes)
// and displays the checkboxes that did not show up in some styles like gtk+.
class CheckableItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CheckableItemDelegate(QObject *parent = 0);

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // CHECKABLEITEMDELEGATE_H
