#ifndef LANGUAGEBUTTONS_H
#define LANGUAGEBUTTONS_H

#include <QWidget>
#include <QSignalMapper>

#include "Test.h"

class LanguageButtons : public QWidget{
    Q_OBJECT
public:
    LanguageButtons(const QList<Test>& tests, bool new_button, QWidget *parent = 0);
    void disconnect_all();

private:
    QSignalMapper signal_mapper;

private slots:
    void forward_click(QObject *obj);

signals:
    void clicked(Test *test);
};

#endif // LANGUAGEBUTTONS_H
