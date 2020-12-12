#include "WordView.h"

#include <QApplication>
#include <QDesktopWidget>
#include <InterfaceParameters.h>

WordView::WordView(Test *test, QWidget *parent) :
    QWidget(parent),
    test(test),
    main_layout(nullptr),
    handwriting_layout(nullptr),
    vertical_layout(nullptr),
    OK_button(nullptr)
{
    // We want a place for a character handwriting widget.
    // Layout is a horizontal one composed of two vertical layouts. The one on the right is for character handwriting.
    // The widget is set as horizontal_layout's parent so it will use this layout as default.
    delete main_layout;
    if (window()->width() > window()->height())
        main_layout = new QHBoxLayout(this);
    else
        main_layout = new QVBoxLayout(this);
    handwriting_layout = new QVBoxLayout;
    vertical_layout = new QVBoxLayout;
    // The sub-layouts are automatically parented to the main layout.
    main_layout->addLayout(handwriting_layout);
    main_layout->addLayout(vertical_layout);
    connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(update_layouts(int)));
}

/*
 No need for a destructor to delete the layouts pointed by the widget's attributes because:
    * Either pointers do not point to anything, we don't use them, so they will be destroyed at the same time as the widget.
    * Or they point to a layout whose parent is the widget or another layout, so Qt will destroy them at the same time as their parents.
*/

void WordView::update_layouts(int)
{
    delete main_layout;
    if (window()->width() > window()->height())
        main_layout = new QHBoxLayout(this);
    else
        main_layout = new QVBoxLayout(this);
}

void WordView::resizeEvent(QResizeEvent *)
{
    if (OK_button)
        OK_button->setFixedSize(2*InterfaceParameters::widget_unit, InterfaceParameters::widget_unit);
}
