#ifndef WORDFRAME_H
#define WORDFRAME_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QtNetwork>

#include "Test.h"

class WordView : public QWidget
{
    Q_OBJECT

protected:
    Test *test;
    QHBoxLayout* horizontal_layout;
    QVBoxLayout* right_vertical_layout;
    QVBoxLayout* vertical_layout;
    bool handwriting;
    QPushButton* OK_button;


public:
    WordView(Test *test, QWidget *parent);
    
signals:
    
public slots:

};

#endif // WORDFRAME_H
