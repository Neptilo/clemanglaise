#include "Switcher.h"

#include <QDebug>
#include <QLayout>

Switcher::Switcher(Qt::Orientation orientation, const QString &first_str, const QString &last_str, QWidget *parent):
    QWidget(parent),
    first_label(nullptr),
    last_label(nullptr),
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
        break;
    default:
        layout = nullptr;
        break;
    }

    // first label
    if(!first_str.isEmpty()){
        first_label = new ClickableLabel(first_str, this);
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
    connect(first_label, SIGNAL(clicked()), this, SLOT(set_to_first()));

    // slider
    slider.setMaximum(1);
    layout->addWidget(&slider);
    connect(&slider, SIGNAL(valueChanged(int)), this, SLOT(emit_value_changed(int)));

    // last label
    if(!last_str.isEmpty()){
        last_label = new ClickableLabel(last_str, this);
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
    connect(last_label, SIGNAL(clicked()), this, SLOT(set_to_last()));

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

bool Switcher::value() const{
    return slider.value();
}

void Switcher::emit_value_changed(int value){
    emit value_changed(value);
}

void Switcher::set_to_first()
{
    slider.setSliderPosition(0);
}

void Switcher::set_to_last()
{
    slider.setSliderPosition(1);
}
