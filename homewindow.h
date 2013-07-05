#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QRadioButton>

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
	QRadioButton* radioLocal; 
	QRadioButton* radioRemote;

public:
    explicit HomeWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    void start_test(int i);
};

#endif // HOMEWINDOW_H
