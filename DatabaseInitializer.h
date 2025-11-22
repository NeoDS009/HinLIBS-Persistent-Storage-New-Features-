#ifndef DATABASEINITIALIZER_H
#define DATABASEINITIALIZER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

/*
    DatabaseInitializer Class:
    Responsible for initializing the SQLite database and populating it with default data.
    Handles database schema creation and initial data population for the HinLIBS system.
    Implements intelligent initialization that only populates data for new databases.

    Key Responsibilities:
    - Create all necessary database tables with proper schema definitions
    - Populate database with default users and catalogue items
    - Prevent duplicate initialization on existing databases
    - Ensure database integrity with foreign key constraints and unique indexes

    Database Schema:
    - users: User accounts with authentication and role management
    - catalogue_items: Library items with type-specific metadata
    - loans: Borrowing records with checkout/return dates
    - holds: Hold queue management with position tracking

    Initial Data:
    - 7 default users: 5 patrons, 1 librarian, 1 system administrator
    - 20 default catalogue items covering all required formats:
      * 5 fiction books with ISBNs
      * 5 non-fiction books with Dewey Decimal classification
      * 3 magazines with issue numbers and publication dates
      * 3 movies with genre and rating information
      * 4 video games with genre and rating information

    Design Patterns:
    - Static utility class: No instantiation required, all methods are static
    - Idempotent operations: Safe to call multiple times without side effects

    Error Handling:
    - Comprehensive error logging for all database operations
    - Returns false on any initialization failure for caller handling
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
