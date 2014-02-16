#include "Switcher.h"

#include <QLayout>

Switcher::Switcher(Qt::Orientation orientation, const QString &first_str, const QString &last_str, QWidget *parent):
    QWidget(parent),
    first_label(NULL),
    last_label(NULL),
    slider(orientation, this)
{
    QLayout *layout;
    const int slider_max_length = 32;
    const int slider_min_length = 16;
    switch (orientation) {
    case Qt::Horizontal:
        layout = new QHBoxLayout(this);
        slider.setMaximumWidth(slider_max_length);
        slider.setMinimumWidth(slider_min_length);
        break;
    case Qt::Vertical:
        layout = new QVBoxLayout(this);
        slider.setMaximumHeight(slider_max_length);
        slider.setMinimumHeight(slider_min_length);
    default:
        layout = NULL;
        break;
    }
    if(!first_str.isEmpty()){
        first_label = new QLabel(first_str, this);
        switch (orientation) {
        case Qt::Horizontal:
            first_label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
            break;
        case Qt::Vertical:
            first_label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
            break;
        default:
            break;
        }
        if(orientation == Qt::Horizontal)
        layout->addWidget(first_label);
    }
    slider.setMaximum(1);
    layout->addWidget(&slider);
    if(!last_str.isEmpty()){
        last_label = new QLabel(last_str, this);
        switch (orientation) {
        case Qt::Horizontal:
            last_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            break;
        case Qt::Vertical:
            last_label->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
            break;
        default:
            break;
        }
        layout->addWidget(last_label);
    }
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(&slider, SIGNAL(valueChanged(int)), this, SLOT(emit_value_changed(int)));
}

bool Switcher::value() const{
    return slider.value();
}

void Switcher::emit_value_changed(int value){
    emit value_changed(value);
}
