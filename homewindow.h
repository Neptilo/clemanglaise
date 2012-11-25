#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QList>

#include <testframe.h>

class HomeWindow : public QWidget{
    Q_OBJECT

private:
    TestFrame* test_frame;
    QLabel* title;
    QList<QPushButton*>* buttons;
    QLayout* layout;

public:
    explicit HomeWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    void start_test();
    
};

#endif // HOMEWINDOW_H
