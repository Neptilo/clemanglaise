#include <QDebug>
#include <QObject>
#include <QString>

#include "include/import_wizard/Importer.h"

Importer::Importer(DatabaseManager *database_manager) :
    database_manager(database_manager),
    dst_test_id(0)
{
}

bool Importer::import(const QHash<QString, QString> &word_data)
{
    if(!dst_test_id){ // TODO: show error message
        qDebug() << QObject::tr("Destination test ID has not been defined.");
        return false;
    }

    // Define data to send
    // word_data["id"] is the ID in the original list. It has no use here.
    if(!database_manager->add_word(dst_test_id, word_data)){ // TODO: show error message if it fails
        qDebug() << QObject::tr("<b>SQLite error: </b>") << database_manager->pop_last_error();
        return false;
    }

    return true;
}
