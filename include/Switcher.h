#ifndef SWITCHER_H
#define SWITCHER_H

#include <QLabel>
#include <QSlider>
#include <QString>
#include <QWidget>

class Switcher : public QWidget
{
    Q_OBJECT
public:
    explicit Switcher(Qt::Orientation orientation, const QString &first_str = "", const QString &last_str = "", QWidget *parent = 0);
    bool value() const;

private:
    QLabel *first_label;
    QLabel *last_label;
    QSlider slider;

signals:
    void value_changed(bool);

public slots:
    void emit_value_changed(int value);

};

#endif // SWITCHER_H
