#ifndef SWITCHER_H
#define SWITCHER_H

#include <QSlider>
#include <QString>
#include <QWidget>

#include "ClickableLabel.h"

class Switcher : public QWidget
{
    Q_OBJECT
public:
    explicit Switcher(Qt::Orientation orientation, const QString &first_str = "", const QString &last_str = "", QWidget *parent = nullptr);
    bool value() const;
private:
    ClickableLabel *first_label;
    ClickableLabel *last_label;
    QSlider slider;

signals:
    void value_changed(bool);

public slots:
    void emit_value_changed(int value);
    void set_to_first();
    void set_to_last();
};

#endif // SWITCHER_H
