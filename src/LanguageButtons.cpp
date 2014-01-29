#include "LanguageButtons.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

LanguageButtons::LanguageButtons(const QList<Test>& tests, QWidget *parent)
    : QWidget(parent),
      signal_mapper(NULL)
{

    signal_mapper = new QSignalMapper(this);

    QGridLayout *layout = new QGridLayout(this);

    if(tests.isEmpty())
        layout->addWidget(new QLabel(tr("You have not created any vocabulary list yet.")), 0, 0);
    else{
        for (int i = 0, l=tests.size(); i < l; ++i) {
            const Test& test = tests[i];
            QPushButton *button = new QPushButton(test.get_name(), this);
            button->setIcon(QIcon(":/" + test.get_dst() + "-img.png"));
            button->setToolTip(tr("from ") + test.get_src() + tr(" to ") + test.get_dst());
            connect(button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
            signal_mapper->setMapping(button, test.get_id());
            layout->addWidget(button, 0, i);
        }
    }

    connect(signal_mapper, SIGNAL(mapped(int)), parent, SLOT(start_test(int)));
}

void LanguageButtons::disconnect_all(){
    signal_mapper->disconnect();
}
