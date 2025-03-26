#ifndef LANGUAGEBUTTONS_H
#define LANGUAGEBUTTONS_H

#include <QWidget>
#include <QSignalMapper>

#include "Test.h"

class MultilinePushButton;

class LanguageButtons : public QWidget{
    Q_OBJECT
public:
    LanguageButtons(const QList<Test>& tests, bool new_button, QWidget *parent = nullptr);
    void layout_buttons();
    void disconnect_all();
private:
    QSignalMapper signal_mapper;
    QList<MultilinePushButton *> buttons;
    
    /**
     * Determine the best number of columns to fit `l` items
     * into a grid layout while maintaining an aspect ratio close to the window's dimensions.
     *
     * @param l The number of items to fit into the grid.
     */
    int compute_best_num_cols(int l);
    
private slots:
    void forward_click(QObject *obj);

signals:
    void clicked(Test *test);
};

#endif // LANGUAGEBUTTONS_H
