#ifndef RESOURCE_UTILS_H
#define RESOURCE_UTILS_H

#include <QIcon>

// Returns the QIcon corresponding to name in the current icon theme,
// or the corresponding fallback icon if no such icon is found.
QIcon getIcon(const QString& name);

#endif // RESOURCE_UTILS_H
