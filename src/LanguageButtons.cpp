#include "LanguageButtons.h"

#include <math.h>

#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include <string_utils.h>

LanguageButtons::LanguageButtons(const QList<Test> &tests, bool new_button, QWidget *parent) :
    QWidget(parent),
    signal_mapper(this)
{
    QGridLayout *layout = new QGridLayout(this);

    int l = tests.size();
    int w = (int) sqrt(l);
    for (int i = 0; i < l; ++i) {
        Test *test = new Test(tests.at(i)); // pointer to non constant copy of test, having same parent as original
        QPushButton *button = new QPushButton(test->get_name(), this);
        button->setIcon(QIcon(":/" + test->get_dst() + "-img.png"));
        button->setToolTip(tr("from ") + test->get_src() + tr(" to ") + test->get_dst());
        signal_mapper.setMapping(button, test);
        connect(button, SIGNAL(clicked()), &signal_mapper, SLOT(map()));
        layout->addWidget(button, i/w, i%w); // so height and width of layout are approximately the same
    }
    if(new_button){
        QPushButton *button = new QPushButton(tr("New list"), this);
        button->setIcon(QIcon::fromTheme("list-add", QIcon(getImgPath("list-add.png"))));
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
