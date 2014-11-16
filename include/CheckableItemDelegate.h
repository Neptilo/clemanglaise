#ifndef CHECKABLEITEMDELEGATE_H
#define CHECKABLEITEMDELEGATE_H

#include <QStyledItemDelegate>

class CheckableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CheckableItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // CHECKABLEITEMDELEGATE_H
