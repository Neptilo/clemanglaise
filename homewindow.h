#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QList>

#include <testframe.h>
#include <languagebuttons.h>
#include <test.h>

class HomeWindow : public QWidget{
    Q_OBJECT

private:
    TestFrame* test_frame;
    QLabel* title;
    QList<Test>* tests;
    QStringList* mapping_texts;
    QStringList* button_texts;
    LanguageButtons* buttons;

public:
    explicit HomeWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    void start_test(const QString & mapping_text);
    
};

#endif // HOMEWINDOW_H
