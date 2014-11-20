#include "MultilinePushButton.h"

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QStylePainter>

MultilinePushButton::MultilinePushButton(QWidget *parent) :
    QPushButton(parent),
    tool_button_style(Qt::ToolButtonIconOnly), // default in QToolButton
    icon(NULL),
    layout(NULL),
    icon_label(NULL),
    text_label(NULL)
{
    refresh();
}

MultilinePushButton::MultilinePushButton(const QString &text, QWidget *parent) :
    QPushButton(parent),
    tool_button_style(Qt::ToolButtonTextOnly),
    icon(NULL),
    layout(NULL),
    icon_label(NULL)
{
    text_label = new QLabel(text, this);
    text_label->setWordWrap(true);
    text_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    text_label->setAlignment(Qt::AlignCenter);
    refresh();
}

MultilinePushButton::MultilinePushButton(const QIcon &icon, const QString &text, QWidget *parent) :
    QPushButton(parent),
    tool_button_style(Qt::ToolButtonTextBesideIcon),
    layout(NULL)
{
    this->icon = new QIcon(icon);
    icon_label = new QLabel(this);
    icon_label->setPixmap(icon.pixmap(fontMetrics().height()));
    icon_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    text_label = new QLabel(text, this);
    text_label->setWordWrap(true);
    text_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    text_label->setAlignment(Qt::AlignCenter);
    refresh();
}

void MultilinePushButton::set_icon(const QIcon &icon)
{
    delete this->icon;
    this->icon = new QIcon(icon);
    delete icon_label;
    icon_label = new QLabel(this);
    icon_label->setPixmap(icon.pixmap(fontMetrics().height()));
    icon_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    refresh();
}

void MultilinePushButton::set_text(const QString &text)
{
    delete text_label;
    text_label = new QLabel(text, this);
    text_label->setWordWrap(true);
    text_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    text_label->setAlignment(Qt::AlignCenter);
    refresh();
}

void MultilinePushButton::refresh()
{
    delete layout;
    if (tool_button_style == Qt::ToolButtonTextUnderIcon) {
        layout = new QVBoxLayout(this);
        layout->addWidget(icon_label);
        layout->addWidget(text_label);
    } else {
        layout = new QHBoxLayout(this);
        switch (tool_button_style) {
        case Qt::ToolButtonIconOnly:
            layout->addWidget(icon_label);
            break;
        case Qt::ToolButtonTextOnly:
            layout->addWidget(text_label);
            break;
        case Qt::ToolButtonTextBesideIcon:
            layout->addWidget(icon_label);
            layout->addWidget(text_label);
            break;
        default:
            qDebug() << tr("Tool button style code unknown");
            break;
        }
    }
    layout->setSizeConstraint(QLayout::SetMinimumSize);
}

void MultilinePushButton::setToolButtonStyle(Qt::ToolButtonStyle style)
{
    tool_button_style = style;
    refresh();
}

void MultilinePushButton::set_icon_size(const QSize &size)
{
    icon_label->setPixmap(icon->pixmap(size));
}
