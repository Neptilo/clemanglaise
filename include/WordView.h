#ifndef WORDVIEW_H
#define WORDVIEW_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>

#include "Test.h"

class WordView : public QWidget
{
    Q_OBJECT

protected:
    Test *test;
    QBoxLayout* main_layout;
    QVBoxLayout* handwriting_layout;
    QVBoxLayout* vertical_layout;
    QPushButton* OK_button;

    void resizeEvent(QResizeEvent *);

public:
    WordView(Test *test, QWidget *parent);
    
signals:
    void OK_clicked();

public slots:

private slots:
    void update_layouts();
};

#endif // WORDVIEW_H
