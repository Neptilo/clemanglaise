#include "LanguageButtons.h"

#include <math.h>

#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

LanguageButtons::LanguageButtons(const QList<Test> &tests, QWidget *parent)
    : QWidget(parent),
      signal_mapper(this)
{
    QGridLayout *layout = new QGridLayout(this);

    if(tests.isEmpty())
        layout->addWidget(new QLabel(tr("You have not created any vocabulary list yet.")), 0, 0);
    else{
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
        connect(&signal_mapper, SIGNAL(mapped(QObject *)), this, SLOT(forward_click(QObject *)));
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
