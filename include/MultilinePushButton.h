#ifndef MULTILINEPUSHBUTTON_H
#define MULTILINEPUSHBUTTON_H

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>

class MultilinePushButton : public QPushButton
{
   Q_OBJECT
public:
    explicit MultilinePushButton(QWidget *parent = nullptr);
    explicit MultilinePushButton(const QString &text, QWidget *parent = nullptr);
    MultilinePushButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);

    void set_icon(const QIcon &icon);
    void set_text(const QString &text);
    void set_tool_button_style(Qt::ToolButtonStyle style);
    void set_icon_size(const QSize &size);

private:
   void refresh();

   Qt::ToolButtonStyle tool_button_style;
   QIcon *icon;

   QBoxLayout *layout;
   QLabel *icon_label;
   QLabel *text_label;
};

#endif // MULTILINEPUSHBUTTON_H
