#include "LanguageButtons.h"

#include <QGridLayout>
#include <QPushButton>

LanguageButtons::LanguageButtons(const QList<Test>& tests, QWidget *parent, const QStringList & flags)
    : QWidget(parent),
      signal_mapper(NULL)
{

    signal_mapper = new QSignalMapper(this);

    QGridLayout *layout = new QGridLayout(this);

    for (int i = 0, l=tests.size(); i < l; ++i) {
        const Test& test = tests[i];
        QString button_text = test.getName();
        QPushButton *button = new QPushButton(button_text);
        button->setIcon(QIcon(flags.at(i)));
        connect(button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
        signal_mapper->setMapping(button, test.getId());
        layout->addWidget(button, 0, i);
    }

    connect(signal_mapper, SIGNAL(mapped(int)), parent, SLOT(start_test(int)));
}

void LanguageButtons::disconnect_all(){
    signal_mapper->disconnect();
}
