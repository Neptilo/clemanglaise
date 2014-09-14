#ifndef IMPORTER_H
#define IMPORTER_H

#include "DatabaseManager.h"

class Importer
{
public:
    explicit Importer(DatabaseManager *database_manager);
    bool import(int dst_test_id, const QHash<QString, QString> &word_data);
    QString get_error() const;

protected:
    DatabaseManager *database_manager;

private:
    QString error;
};

#endif // IMPORTER_H
