#ifndef LANGUAGEBUTTONS_H
#define LANGUAGEBUTTONS_H

#include <QWidget>
#include <QSignalMapper>

#include <test.h>

class LanguageButtons : public QWidget{
    Q_OBJECT
public:
    LanguageButtons(const QList<Test>& tests, QWidget *parent, const QStringList & flags);
    void disconnect_all();

signals:
    void clicked(const QString &text);

private:
    QSignalMapper *signal_mapper;
    
};

#endif // LANGUAGEBUTTONS_H
