#include <QGridLayout>
#include <QPushButton>
#include <QtGui>

#include "languagebuttons.h"

LanguageButtons::LanguageButtons(const QList<Test>& tests, QWidget *parent)
    : QWidget(parent){

    signal_mapper = new QSignalMapper(this);

    QGridLayout *layout = new QGridLayout(this);

    for (int i = 0; i < tests.size(); ++i) {
        Test test = tests[i];
        QString button_text = test.getName();
        QString mapping_text = test.getSrc() + test.getDst();
        QPushButton *button = new QPushButton(button_text);
        connect(button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
        signal_mapper->setMapping(button, mapping_text);
        layout->addWidget(button, 0, i);
    }

    connect(signal_mapper, SIGNAL(mapped(const QString &)), parent, SLOT(start_test(const QString &)));
}

void LanguageButtons::disconnect_all(){
    signal_mapper->disconnect();
}
