#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <vector>
#include "User.h"
#include "LibraryItem.h"
#include "AddItemDialog.h"

/*
    DatabaseManager Class:
    Singleton class that serves as the central data access layer for the HinLIBS system.
    Manages all SQLite database operations including user management, catalogue operations,
    loan tracking, and hold queue management. Implements the Object-Relational Mapping
    (ORM) pattern to bridge between C++ objects and relational database tables.

    Key Responsibilities:
    - Manage SQLite database connection and lifecycle
    - Provide thread-safe singleton access to database operations
    - Map C++ objects to database records and vice versa
    - Enforce data integrity through transaction management
    - Handle all CRUD operations for library business entities

    Database Schema Management:
    - Users table: Patron, librarian, and administrator accounts
    - Catalogue_items table: Library collection with type-specific metadata
    - Loans table: Active borrowing records with due dates
    - Holds table: Hold queue management with position tracking

    Data Members:
      - QSqlDatabase db: SQLite database connection instance
      - static DatabaseManager* instance: Singleton instance pointer

    Member Functions:
      Public:
        - getInstance(): Provides global access to singleton instance
        - ~DatabaseManager(): Cleans up database connection

        User Operations:
        - findUser(): Authenticates users by username
        - getAllUsers(): Retrieves all system users

        Catalogue Operations:
        - getAllCatalogueItems(): Retrieves complete library collection
        - getItemById(): Fetches specific item by database ID
        - addItemToCatalogue(): Adds new items to library collection
        - removeItemFromCatalogue(): Removes items with safety checks

        Loan Operations:
        - borrowItem(): Processes book borrowing with status updates
        - returnItem(): Handles book returns and availability updates
        - getUserBorrowedItems(): Retrieves user's active loans

        Hold Operations:
        - placeHold(): Adds users to item wait queues
        - cancelHold(): Removes holds and updates queue positions
        - getUserHolds(): Retrieves user's active hold requests
        - getHoldCountForItem(): Counts active holds for an item
        - getHoldPosition(): Gets user's position in hold queue

        Utility Methods:
        - isDatabaseOpen(): Verifies database connection status
        - getItemId(): Resolves LibraryItem to database ID
        - getUserLoansWithDates(): Gets detaiils of a user's loans

      Private:
        - DatabaseManager(): Private constructor for singleton pattern
        - createItemFromQuery(): Factory method for LibraryItem objects

*/
class DatabaseManager {
private:
    QSqlDatabase db;
    static DatabaseManager* instance;

    DatabaseManager(); // Private constructor for singleton

public:
    /*
        Function: getInstance
        Purpose: Provides global access to the singleton DatabaseManager instance.
                 Implements lazy initialization - creates instance on first call.
        Return: DatabaseManager& - Reference to the singleton instance
    */
    static DatabaseManager& getInstance();

    ~DatabaseManager();

    // User operations
    /*
        Function: findUser
        Purpose: Authenticates and retrieves user by username. Used for login validation
                 and role-based access control throughout the system.
        Parameters:
          in: const QString& username - Username to search for
        Return: User* - Pointer to User object if found, nullptr otherwise
    */
    User* findUser(const QString& username);

    /*
        Function: getAllUsers
        Purpose: Retrieves all registered users from the database. Primarily used
                 for librarian administrative functions and patron selection.
        Return: std::vector<User*> - Vector of all user objects
    */
    std::vector<User*> getAllUsers();

    // Catalogue operations
    /*
        Function: getAllCatalogueItems
        Purpose: Retrieves the complete library catalogue with current availability
                 status. Used to populate the main catalogue display.
        Return: std::vector<LibraryItem*> - Vector of all catalogue items
    */
    std::vector<LibraryItem*> getAllCatalogueItems();

    /*
        Function: getItemById
        Purpose: Retrieves a specific catalogue item by its database ID. Used for
                 precise item lookup and reference resolution.
        Parameters:
          in: int id - Database ID of the item to retrieve
        Return: LibraryItem* - Pointer to item if found, nullptr otherwise
    */
    LibraryItem* getItemById(int id);

    // Loan operations
    /*
        Function: borrowItem
        Purpose: Processes book borrowing operation. Updates item availability and
                 creates loan record with due date calculation (14 days).
        Parameters:
          in: int userId - Database ID of the borrowing user
          in: int itemId - Database ID of the item being borrowed
        Return: bool - True if operation succeeded, false on error
    */
    bool borrowItem(int userId, int itemId);

    /*
        Function: returnItem
        Purpose: Processes book return operation. Updates item availability and
                 marks loan record as returned. Enables hold fulfillment.
        Parameters:
          in: int userId - Database ID of the returning user
          in: int itemId - Database ID of the item being returned
        Return: bool - True if operation succeeded, false on error
    */
    bool returnItem(int userId, int itemId);

    /*
        Function: getUserBorrowedItems
        Purpose: Retrieves all currently borrowed items for a specific user.
                 Used to display user's account status and enable returns.
        Parameters:
          in: int userId - Database ID of the user
        Return: std::vector<LibraryItem*> - Vector of user's borrowed items
    */
    std::vector<LibraryItem*> getUserBorrowedItems(int userId);

    // Hold operations
    /*
        Function: placeHold
        Purpose: Adds a user to an item's hold queue. Calculates and assigns
                 position based on first-come-first-served ordering.
        Parameters:
          in: int userId - Database ID of the user placing hold
          in: int itemId - Database ID of the item being held
        Return: bool - True if hold placed successfully, false on error
    */
    bool placeHold(int userId, int itemId);

    /*
        Function: cancelHold
        Purpose: Removes a user from an item's hold queue and updates positions
                 of remaining users in the queue.
        Parameters:
          in: int userId - Database ID of the user canceling hold
          in: int itemId - Database ID of the held item
        Return: bool - True if hold canceled successfully, false on error
    */
    bool cancelHold(int userId, int itemId);

    /*
        Function: getUserHolds
        Purpose: Retrieves all active hold requests for a specific user.
                 Returns holds in queue position order.
        Parameters:
          in: int userId - Database ID of the user
        Return: std::vector<LibraryItem*> - Vector of user's hold items in position order
    */
    std::vector<LibraryItem*> getUserHolds(int userId);

    // Utility methods
    /*
        Function: isDatabaseOpen
        Purpose: Verifies the database connection is active and operational.
                 Used for error checking before database operations.
        Return: bool - True if database connection is open and valid
    */
    bool isDatabaseOpen() const;

    /*
        Function: getItemId
        Purpose: Resolves a LibraryItem object to its database ID by matching
                 title and author. Used for object-to-database reference resolution.
        Parameters:
          in: LibraryItem* item - Pointer to the LibraryItem object
        Return: int - Database ID of the item, or -1 if not found
    */
    int getItemId(LibraryItem* item);

    /*
        Function: getHoldCountForItem
        Purpose: Counts the number of active holds for a specific item.
                 Used for display purposes and position calculation.
        Parameters:
          in: int itemId - Database ID of the item
        Return: int - Number of active holds for the item
    */
    int getHoldCountForItem(int itemId);

    /*
        Function: getHoldPosition
        Purpose: Retrieves a user's specific position in an item's hold queue.
                 Used for displaying accurate position information to users.
        Parameters:
          in: int userId - Database ID of the user
          in: int itemId - Database ID of the item
        Return: int - User's position in queue (1-based), or -1 if not found
    */
    int getHoldPosition(int userId, int itemId);

    // Catalogue management operations
    /*
        Function: addItemToCatalogue
        Purpose: Adds a new item to the library catalogue with comprehensive
                 metadata. Handles all item types with appropriate field mapping.
        Parameters:
          in: const QString& title - Item title
          in: const QString& author - Author/creator name
          in: const QString& itemType - Type of item (fiction, nonfiction, etc.)
          in: const QString& deweyDecimal - Dewey Decimal classification (non-fiction)
          in: const QString& isbn - ISBN number (books)
          in: const QString& genre - Genre classification (media)
          in: const QString& rating - Content rating (media)
          in: int issueNumber - Issue number (magazines)
          in: const QString& publicationDate - Publication date (magazines)
          in: int publicationYear - Year of publication
          in: const QString& condition - Physical condition of item
        Return: bool - True if item added successfully, false on error
    */
    bool addItemToCatalogue(const QString& title, const QString& author, const QString& itemType,
                           const QString& deweyDecimal, const QString& isbn, const QString& genre,
                           const QString& rating, int issueNumber, const QString& publicationDate,
                           int publicationYear, const QString& condition);

    /*
        Function: removeItemFromCatalogue
        Purpose: Removes an item from the catalogue with comprehensive safety checks.
                 Prevents removal of borrowed items or items with active holds.
        Parameters:
          in: int itemId - Database ID of the item to remove
        Return: bool - True if item removed successfully, false if prevented by safety checks
    */
    bool removeItemFromCatalogue(int itemId);


    /*
        Function: getUserLoansWithDates
        Purpose: Method that returns user loans details.
        Parameters:
          in: int userId
        Return: LoanInfo - an array with the loan details
    */
    struct LoanInfo {
        LibraryItem* item;
        QString checkoutDate;
        QString dueDate;
    };
    std::vector<LoanInfo> getUserLoansWithDates(int userId);


private:
    /*
        Function: createItemFromQuery
        Purpose: Factory method that creates appropriate LibraryItem subclass instances
                 from SQL query results. Implements ORM pattern for database-to-object mapping.
        Parameters:
          in: const QSqlQuery& query - SQL query result containing item data
        Return: LibraryItem* - Appropriately typed LibraryItem instance, or nullptr on error
    */
    LibraryItem* createItemFromQuery(const QSqlQuery& query);
};

#endif
