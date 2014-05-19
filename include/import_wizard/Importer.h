#ifndef IMPORTER_H
#define IMPORTER_H

#include "DatabaseManager.h"

class Importer
{
public:
    explicit Importer(DatabaseManager *database_manager);
    bool import(const QHash<QString, QString> &word_data);

protected:
    DatabaseManager *database_manager;
    int dst_test_id;
};

#endif // IMPORTER_H
