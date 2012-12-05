#include <QGridLayout>
#include <QPushButton>

#include "languagebuttons.h"

LanguageButtons::LanguageButtons(const QStringList& mapping_texts, const QStringList& button_texts, QWidget *parent)
    : QWidget(parent){
    signal_mapper = new QSignalMapper(this);

    QGridLayout *layout = new QGridLayout(this);
    for (int i = 0; i < button_texts.size(); ++i) {
        QPushButton *button = new QPushButton(button_texts[i]);
        connect(button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
        signal_mapper->setMapping(button, mapping_texts[i]);
        layout->addWidget(button, i / 3, i % 3);
    }

    connect(signal_mapper, SIGNAL(mapped(const QString &)), parent, SLOT(start_test(const QString &)));
}

void LanguageButtons::disconnect_all(){
    signal_mapper->disconnect();
}
