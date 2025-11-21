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

class DatabaseManager {
private:
    QSqlDatabase db;
    static DatabaseManager* instance;

    DatabaseManager(); // Private constructor for singleton

public:
    static DatabaseManager& getInstance();
    ~DatabaseManager();

    // User operations
    User* findUser(const QString& username);
    std::vector<User*> getAllUsers();

    // Catalogue operations (we'll add these next)
    std::vector<LibraryItem*> getAllCatalogueItems();
    LibraryItem* getItemById(int id);

    // Loan operations (we'll add these later)
    bool borrowItem(int userId, int itemId);
    bool returnItem(int userId, int itemId);
    std::vector<LibraryItem*> getUserBorrowedItems(int userId);

    // Hold operations (we'll add these last)
    bool placeHold(int userId, int itemId);
    bool cancelHold(int userId, int itemId);
    std::vector<LibraryItem*> getUserHolds(int userId);

    // Utility
    bool isDatabaseOpen() const;

    void debugPrintAllUsers();  // for debugging
    int getItemId(LibraryItem* item);

    int getHoldCountForItem(int itemID);
    int getHoldPosition(int userId, int itemId); //helper

    bool addItemToCatalogue(const QString& title, const QString& author, const QString& itemType,
                           const QString& deweyDecimal, const QString& isbn, const QString& genre,
                           const QString& rating, int issueNumber, const QString& publicationDate,
                           int publicationYear, const QString& condition);

    bool removeItemFromCatalogue(int itemId);


private:
    LibraryItem* createItemFromQuery(const QSqlQuery& query);
};

#endif
