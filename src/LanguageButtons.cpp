#include "LanguageButtons.h"

#include <math.h>

#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>

#include "InterfaceParameters.h"
#include "MultilinePushButton.h"
#include "resource_utils.h"
#include "string_utils.h"
#include "iso_mapping.h"

int LanguageButtons::compute_best_num_cols(int l)
{
    // Compute number of rows and columns, and cell size
    int w = window()->width();
    int h = window()->height();
    float ratio = w / (float)h;
    float ncols_float = sqrt(l * ratio);
    float nrows_float = l / ncols_float;

    // Find best option filling the whole height
    int nrows1 = ceil(nrows_float);
    int ncols1 = ceil(l / nrows1);
    while (nrows1 * ratio < ncols1) {
        nrows1++;
        ncols1 = ceil(l / nrows1);
    }

    // Find best option filling the whole width
    int ncols2 = ceil(ncols_float);
    int nrows2 = ceil(l / ncols2);
    while (ncols2 < nrows2 * ratio) {
        ncols2++;
        nrows2 = ceil(l / ncols2);
    }

    // Find the best values
    return h / (float)nrows1 < w / (float)ncols2 ? ncols2 : ncols1;
}

LanguageButtons::LanguageButtons(const QList<Test> &tests, bool new_button, QWidget *parent) :
    QWidget(parent),
    signal_mapper(this)
{
    int l = tests.size();
    if (new_button) l++;
    if (!l) return;

    int ncols = compute_best_num_cols(l);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QGridLayout *layout = new QGridLayout(this);
    int i;
    for (i = 0; i < tests.size(); ++i) {
        Test *test = new Test(tests.at(i), this); // pointer to non constant copy of test
        MultilinePushButton *button = new MultilinePushButton(
                    QIcon(":/img/flags/"+test->get_flag()+".png"),
                    test->get_name(),
                    this);
        button->setToolTip(tr("from ") + toTitleCase(LANG_MAP.value(test->get_src())) +
                           tr(" to ") + toTitleCase(LANG_MAP.value(test->get_dst())));
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->set_icon_size(QSize(2*fontMetrics().height(),
                                    2*fontMetrics().height()));
        signal_mapper.setMapping(button, (QObject *) test);
        connect(button, SIGNAL(clicked()), &signal_mapper, SLOT(map()));
        layout->addWidget(button, i/ncols, i%ncols);
        buttons.push_back(button);
    }
    if(new_button){
        MultilinePushButton *button = new MultilinePushButton(
                    getIcon("list-add"),
                    tr("New list"),
                    this);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->set_icon_size(QSize(2*fontMetrics().height(),
                                    2*fontMetrics().height()));
        signal_mapper.setMapping(button, (QObject *) nullptr);
        connect(button, SIGNAL(clicked()), &signal_mapper, SLOT(map()));
        layout->addWidget(button, i/ncols, i%ncols);
        buttons.push_back(button);
    }
    connect(&signal_mapper, &QSignalMapper::mappedObject, this, &LanguageButtons::forward_click);
    layout_buttons();
}

void LanguageButtons::layout_buttons()
{
    for (MultilinePushButton* button : std::as_const(buttons))
    {
        switch (InterfaceParameters::orientation)
        {
        case Qt::PortraitOrientation:
            button->set_tool_button_style(Qt::ToolButtonTextUnderIcon);
            break;
        case Qt::LandscapeOrientation:
            button->set_tool_button_style(Qt::ToolButtonTextBesideIcon);
            break;
        default: ;
        }
    }
}

void LanguageButtons::disconnect_all(){
    signal_mapper.disconnect();
}

// necessary slot to cast object type to Test
void LanguageButtons::forward_click(QObject *obj)
{
    emit clicked((Test *) obj);
}
