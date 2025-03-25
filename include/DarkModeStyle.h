#ifndef DARKMODESTYLE_H
#define DARKMODESTYLE_H

#include <QProxyStyle>
#include <QStyleFactory>

class DarkModeStyle : public QProxyStyle {
public:
    DarkModeStyle();

    void polish(QPalette &palette) override;
};

#endif // DARKMODESTYLE_H
