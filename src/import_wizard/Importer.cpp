#include <QDebug>
#include <QObject>
#include <QString>

#include "include/import_wizard/Importer.h"

Importer::Importer(DatabaseManager *database_manager) :
    database_manager(database_manager)
{
}

bool Importer::import(const QHash<QString, QString> &word_data)
{
    if(!word_data["list_id"].toInt()){
        error = QObject::tr("Destination test ID has not been defined.");
        return false;
    }

    // Define data to send
    // word_data["id"] is the id in the original list. It has no use here.
    if(!database_manager->add_word(word_data)){
        error = QObject::tr("<b>SQLite error: </b>%1").arg(database_manager->pop_last_error());
        return false;
    }

    return true;
}

QString Importer::get_error() const
{
    return error;
}
