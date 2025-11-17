#ifndef DATABASEINITIALIZER_H
#define DATABASEINITIALIZER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

//#include <sqlite3.h> // we cannot use this with the .pro widgets sql

class DatabaseInitializer {
public:
    static bool initializeDatabase(const QString& databasePath);

private:
    static bool createTables(QSqlDatabase& db);
    static bool populateDefaultData(QSqlDatabase& db);
    static bool addDefaultUsers(QSqlDatabase& db);
    static bool addDefaultCatalogue(QSqlDatabase& db);
};

#endif
