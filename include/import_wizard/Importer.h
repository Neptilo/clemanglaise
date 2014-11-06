#ifndef IMPORTER_H
#define IMPORTER_H

#include "DatabaseManager.h"

class Importer
{
public:
    explicit Importer(DatabaseManager *database_manager);
    bool import(const QHash<QString, QString> &word_data);
    QString get_error() const;

protected:
    DatabaseManager *database_manager;
    // Each of the three following attributes represent tags that are implied in the import, in the same order in each list.
    QList<int> online_tag_ids;
    QList<int> offline_tag_ids;
    QStringList tag_names;
private:
    QString error;
};

#endif // IMPORTER_H
