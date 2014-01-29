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

signals:
    void clicked(const QString &text);

private:
    QSignalMapper *signal_mapper;
    
};

#endif // LANGUAGEBUTTONS_H
