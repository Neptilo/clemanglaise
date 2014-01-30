#include "WordView.h"

WordView::WordView(Test *test, QWidget *parent) :
    QWidget(parent),
    test(test),
    horizontal_layout(NULL),
    right_vertical_layout(NULL),
    vertical_layout(NULL),
    handwriting(false),
    OK_button(NULL)
{

    // List of languages for which we want a handwriting widget
    QStringList list;
    list << "ja" << "zh";
    handwriting = list.contains(test->get_dst());
    if(handwriting){
        // We want a place for a character handwriting widget.
        // Layout is a horizontal one composed of two vertical layouts. The one on the right is for character handwriting.
        // The widget is set as horizontal_layout's parent so it will use this layout as default.
        horizontal_layout = new QHBoxLayout(this);
        right_vertical_layout = new QVBoxLayout();
        vertical_layout = new QVBoxLayout();
        // The sub-layouts are automatically parented to the main layout.
        horizontal_layout->addLayout(right_vertical_layout);
        horizontal_layout->addLayout(vertical_layout);
    }else{
        // We don't need a character handwriting widget.
        // Layout is just a vertical one.
        vertical_layout = new QVBoxLayout(this);
    }
}

/*
 No need for a destructor to delete the layouts pointed by the widget's attributes because:
    * Either pointers do not point to anything, we don't use them, so they will be destroyed at the same time as the widget.
    * Or they point to a layout whose parent is the widget or another layout, so Qt will destroy them at the same time as their parents.
*/
