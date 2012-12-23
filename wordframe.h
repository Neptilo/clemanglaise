#ifndef WORDFRAME_H
#define WORDFRAME_H

#include <QWidget>
#include <QBoxLayout>

#include "test.h"

class WordFrame : public QWidget
{
    Q_OBJECT

private:
    Test test;
    QHBoxLayout* horizontal_layout;
    QVBoxLayout* right_vertical_layout;
    QVBoxLayout* vertical_layout;

public:
    WordFrame(Test &test, QWidget *parent);
    
signals:
    
public slots:
    
};

#endif // WORDFRAME_H
