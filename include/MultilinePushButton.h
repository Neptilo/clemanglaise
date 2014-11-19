#ifndef MULTILINEPUSHBUTTON_H
#define MULTILINEPUSHBUTTON_H

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>

class MultilinePushButton : public QPushButton
{
   Q_OBJECT
public:
    explicit MultilinePushButton(QWidget *parent=0);
    explicit MultilinePushButton(const QString &text, QWidget *parent=0);
    MultilinePushButton(const QIcon& icon, const QString &text, QWidget *parent=0);

    void set_icon(const QIcon &icon);
    void set_text(const QString &text);
    void setToolButtonStyle(Qt::ToolButtonStyle style);
    QSize sizeHint();

private:
   void refresh();

   Qt::ToolButtonStyle tool_button_style;

   QBoxLayout *layout;
   QLabel *icon_label;
   QLabel *text_label;
};

#endif // MULTILINEPUSHBUTTON_H
