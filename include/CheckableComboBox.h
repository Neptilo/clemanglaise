#ifndef CHECKABLECOMBOBOX_H
#define CHECKABLECOMBOBOX_H

#include <QComboBox>

class CheckableComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit CheckableComboBox(QWidget *parent = nullptr);
    void hidePopup() override;
    void paintEvent(QPaintEvent*) override;
    void set_title(const QString& title);

private:
    QString m_title;
};

#endif // CHECKABLECOMBOBOX_H
