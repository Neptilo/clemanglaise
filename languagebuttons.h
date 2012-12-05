#ifndef LANGUAGEBUTTONS_H
#define LANGUAGEBUTTONS_H

#include <QWidget>
#include <QSignalMapper>

class LanguageButtons : public QWidget{
    Q_OBJECT
public:
    LanguageButtons(const QStringList &mapping_texts, const QStringList &button_texts, QWidget *parent = 0);
    void disconnect_all();

signals:
    void clicked(const QString &text);

private:
    QSignalMapper *signal_mapper;
    
};

#endif // LANGUAGEBUTTONS_H
