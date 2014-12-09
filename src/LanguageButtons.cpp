#include "LanguageButtons.h"

#include <math.h>

#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>

#include "InterfaceParameters.h"
#include "MultilinePushButton.h"
#include "string_utils.h"
#include "iso_mapping.h"

LanguageButtons::LanguageButtons(const QList<Test> &tests, bool new_button, QWidget *parent) :
    QWidget(parent),
    signal_mapper(this)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QGridLayout *layout = new QGridLayout(this);

    int l = tests.size();
    int w; // number of rows of the button grid
    if(new_button)
        w = (int) sqrt(l+1);
    else
        w = (int) sqrt(l);
    for (int i = 0; i < l; ++i) {
        Test *test = new Test(tests.at(i), this); // pointer to non constant copy of test
        MultilinePushButton *button = new MultilinePushButton(
                    QIcon(getImgPath("flags/"+test->get_flag()+".png")),
                    test->get_name(),
                    this);
        if (InterfaceParameters::orientation == Qt::PortraitOrientation)
            button->set_tool_button_style(Qt::ToolButtonTextUnderIcon);
        button->setToolTip(tr("from ") + LANG_MAP.value(test->get_src()) + tr(" to ") + LANG_MAP.value(test->get_dst()));
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->set_icon_size(QSize(2*fontMetrics().height(),
                                    2*fontMetrics().height()));
        signal_mapper.setMapping(button, (QObject *) test);
        connect(button, SIGNAL(clicked()), &signal_mapper, SLOT(map()));
        layout->addWidget(button, i/w, i%w); // so height and width of layout are approximately the same
    }
    if(new_button){
        MultilinePushButton *button = new MultilinePushButton(
                    QIcon::fromTheme("list-add", QIcon(getImgPath("list-add.png"))),
                    tr("New list"),
                    this);
        if (InterfaceParameters::orientation == Qt::PortraitOrientation)
            button->set_tool_button_style(Qt::ToolButtonTextUnderIcon);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->set_icon_size(QSize(2*fontMetrics().height(),
                                    2*fontMetrics().height()));
        signal_mapper.setMapping(button, (QObject *) NULL);
        connect(button, SIGNAL(clicked()), &signal_mapper, SLOT(map()));
        layout->addWidget(button, l/w, l%w);
    }
    connect(&signal_mapper, SIGNAL(mapped(QObject *)), this, SLOT(forward_click(QObject *)));
}

void LanguageButtons::disconnect_all(){
    signal_mapper.disconnect();
}

// necessary slot to cast object type to Test
void LanguageButtons::forward_click(QObject *obj)
{
    emit clicked((Test *) obj);
}
