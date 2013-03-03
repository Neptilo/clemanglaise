#ifndef WORDFRAME_H
#define WORDFRAME_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>

#include "test.h"

class WordFrame : public QWidget
{
    Q_OBJECT

protected:
    Test test;
    QHBoxLayout* horizontal_layout;
    QVBoxLayout* right_vertical_layout;
    QVBoxLayout* vertical_layout;
    bool handwriting;
    QPushButton* OK_button;


public:
    WordFrame(Test &test, QWidget *parent);
    
signals:
    
public slots:
    
};

#endif // WORDFRAME_H
