#ifndef DATABASEINITIALIZER_H
#define DATABASEINITIALIZER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

/*
    DatabaseInitializer Class:
    Responsible for initializing the SQLite database and populating it with default data.
    Handles database schema creation and initial data population for the HinLIBS system.
    Implements intelligent initialization that only populates data for new databases.

    Data Members: None (static class with no instance data)

    Member Functions:
    - initializeDatabase(): Main method that orchestrates complete database setup
    - createTables(): Defines and creates all database tables with proper schemas
    - populateDefaultData(): Populates database with default users and catalogue items
    - addDefaultUsers(): Inserts predefined user accounts
    - addDefaultCatalogue(): Inserts default library items with realistic metadata
*/
class DatabaseInitializer {
public:
    /*
        Function: initializeDatabase
        Purpose: Main initialization method that orchestrates complete database setup.
                 Creates all tables and populates with default data only for new databases.
        Parameters:
          in: const QString& databasePath - File path for the SQLite database file
        Return: bool - true if initialization successful, false on any error
    */
    static bool initializeDatabase(const QString& databasePath);

private:
    /*
        Function: createTables
        Purpose: Creates all database tables with proper schema definitions and constraints.
        Tables Created:
          - users: id, username, role, created_date
          - catalogue_items: id, title, author, item_type, plus type-specific fields
          - loans: id, user_id, item_id, checkout_date, due_date, return_date
          - holds: id, user_id, item_id, position, created_date
        Parameters:
          in: QSqlDatabase& db - Reference to active database connection
        Return: bool - true if all tables created successfully, false on any error
    */
    static bool createTables(QSqlDatabase& db);

    /*
        Function: populateDefaultData
        Purpose: Orchestrates population of all default data into the database;
                 populates default user accounts first, then catalogue items.
        Parameters:
          in: QSqlDatabase& db - Reference to active database connection
        Return: bool - true if all data populated successfully, false on any error
    */
    static bool populateDefaultData(QSqlDatabase& db);

    /*
        Function: addDefaultUsers
        Purpose: Populates the users table with default accounts for system testing.
                 (5 patrons {alice_p, bob_p, charlie_p, diana_p, eve_p}, 1 librarian {libby}, 1 system admin {admin})
        Parameters:
          in: QSqlDatabase& db - Reference to active database connection
        Return: bool - true if users added successfully, false on any error
    */
    static bool addDefaultUsers(QSqlDatabase& db);

    /*
        Function: addDefaultCatalogue
        Purpose: Populates the catalogue_items table with comprehensive default library items
                 (Fiction Books, Non-Fiction Books, Magazines, Movies, Video Games)
        Parameters:
          in: QSqlDatabase& db - Reference to active database connection
        Return: bool - true if catalogue items added successfully, false on any error
    */
    static bool addDefaultCatalogue(QSqlDatabase& db);
};

#endif
