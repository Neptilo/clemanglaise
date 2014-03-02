#ifndef LANGUAGEBUTTONS_H
#define LANGUAGEBUTTONS_H

#include <QWidget>
#include <QSignalMapper>

#include "Test.h"

class LanguageButtons : public QWidget{
    Q_OBJECT
public:
    LanguageButtons(const QList<Test>& tests, QWidget *parent);
    void disconnect_all();
    QSignalMapper signal_mapper;

signals:
    void clicked(const QString &text);    
};

#endif // LANGUAGEBUTTONS_H
