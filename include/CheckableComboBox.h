#ifndef CHECKABLECOMBOBOX_H
#define CHECKABLECOMBOBOX_H

#include <QComboBox>

class CheckableComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit CheckableComboBox(QWidget *parent = 0);
    virtual void hidePopup();
};

#endif // CHECKABLECOMBOBOX_H
