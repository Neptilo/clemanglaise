#include "resource_utils.h"

QIcon getIcon(const QString& name){
    return QIcon::fromTheme(name, QIcon(":/img/" + name + ".png"));
}
