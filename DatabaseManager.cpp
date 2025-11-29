#include <QDebug>
#include <QDate>
#include "DatabaseManager.h"

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::addDatabase("QSQLITE", "library_connection");
    db.setDatabaseName("hinlibs.db");

    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
    }
}

DatabaseManager& DatabaseManager::getInstance() {
    if (!instance) {
        instance = new DatabaseManager();
    }
    return *instance;
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::isDatabaseOpen() const {
    return db.isOpen();
}


User* DatabaseManager::findUser(const QString& username) {
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return nullptr;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, username, role FROM users WHERE username = ?");
    query.addBindValue(username);

    if (query.exec()) {
        if (query.next()) {
            int id = query.value("id").toInt();
            QString foundUsername = query.value("username").toString();
            QString role = query.value("role").toString();
            return new User(id, foundUsername.toStdString(), role.toStdString());
        } else {
            qDebug() << "No user found with username:" << username;
        }
    } else {
        qDebug() << "Query error:" << query.lastError().text();
    }

    return nullptr;
}


std::vector<User*> DatabaseManager::getAllUsers() {
    std::vector<User*> users;

    if (!db.isOpen()) return users;

    QSqlQuery query("SELECT id, username, role FROM users", db);

    while (query.next()) {
        int id = query.value("id").toInt();
        QString username = query.value("username").toString();
        QString role = query.value("role").toString();
        users.push_back(new User(id, username.toStdString(), role.toStdString()));
    }

    return users;
}

std::vector<LibraryItem*> DatabaseManager::getAllCatalogueItems() {
    std::vector<LibraryItem*> items;

    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return items;
    }

    QSqlQuery query("SELECT * FROM catalogue_items", db);

    int count = 0;
    while (query.next()) {
        LibraryItem* item = createItemFromQuery(query);
        if (item) {
            items.push_back(item);
            count++;
        }
    }

    return items;
}

LibraryItem* DatabaseManager::createItemFromQuery(const QSqlQuery& query) {
    QString itemType = query.value("item_type").toString();
    QString title = query.value("title").toString();
    QString author = query.value("author").toString();
    bool isAvailable = query.value("is_available").toBool();
    int publicationYear = query.value("publication_year").toInt();
    QString condition = query.value("condition").toString();
    QString isbn = query.value("isbn").toString();
    QString deweyDecimal = query.value("dewey_decimal").toString();
    QString genre = query.value("genre").toString();
    QString rating = query.value("rating").toString();
    int issueNumber = query.value("issue_number").toInt();
    QString publicationDate = query.value("publication_date").toString();

    LibraryItem* item = nullptr;

    if (itemType == "fiction") {
        item = new FictionBook(title.toStdString(), author.toStdString(),
                              publicationYear, condition.toStdString(),
                              isbn.toStdString());
    }
    else if (itemType == "nonfiction") {
        item = new NonFictionBook(title.toStdString(), author.toStdString(),
                                 deweyDecimal.toStdString(), publicationYear,
                                 condition.toStdString(), isbn.toStdString());
    }
    else if (itemType == "magazine") {
        item = new Magazine(title.toStdString(), author.toStdString(),
                           issueNumber, publicationDate.toStdString(),
                           publicationYear, condition.toStdString());
    }
    else if (itemType == "movie") {
        item = new Movie(title.toStdString(), author.toStdString(),
                        genre.toStdString(), rating.toStdString(),
                        publicationYear, condition.toStdString());
    }
    else if (itemType == "videogame") {
        item = new VideoGame(title.toStdString(), author.toStdString(),
                            genre.toStdString(), rating.toStdString(),
                            publicationYear, condition.toStdString());
    }

    if (item) {
        item->setAvailable(isAvailable);
    }

    return item;
}


int DatabaseManager::getItemId(LibraryItem* item) {
    if (!db.isOpen() || !item) return -1;

    QSqlQuery query(db);
    query.prepare("SELECT id FROM catalogue_items WHERE title = ? AND author = ?");
    query.addBindValue(QString::fromStdString(item->getTitle()));
    query.addBindValue(QString::fromStdString(item->getAuthor()));

    if (query.exec() && query.next()) {
        return query.value("id").toInt();
    }

    return -1;
}

bool DatabaseManager::borrowItem(int userId, int itemId) {
    if (!db.isOpen()) {
        qDebug() << "Database not open for borrowing!";
        return false;
    }

    QSqlQuery query(db);

    // 1. Update item availability in catalogue_items
    query.prepare("UPDATE catalogue_items SET is_available = 0 WHERE id = ?");
    query.addBindValue(itemId);

    if (!query.exec()) {
        qDebug() << "Error updating item availability:" << query.lastError().text();
        return false;
    }

    // 2. Create loan record in loans table
    QDate checkoutDate = QDate::currentDate();
    QDate dueDate = checkoutDate.addDays(14);

    query.prepare("INSERT INTO loans (user_id, item_id, checkout_date, due_date) VALUES (?, ?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(itemId);
    query.addBindValue(checkoutDate.toString("yyyy-MM-dd"));
    query.addBindValue(dueDate.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        qDebug() << "Error creating loan record:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::returnItem(int userId, int itemId) {
    if (!db.isOpen()) return false;

    QSqlQuery query(db);

    // 1. Update item availability back to available
    query.prepare("UPDATE catalogue_items SET is_available = 1 WHERE id = ?");
    query.addBindValue(itemId);

    if (!query.exec()) {
        qDebug() << "Error updating item availability on return:" << query.lastError().text();
        return false;
    }

    // 2. Update loan record with return date
    query.prepare("UPDATE loans SET return_date = ? WHERE user_id = ? AND item_id = ? AND return_date IS NULL");
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    query.addBindValue(userId);
    query.addBindValue(itemId);

    if (!query.exec()) {
        qDebug() << "Error updating loan return date:" << query.lastError().text();
        return false;
    }

    return true;
}

std::vector<LibraryItem*> DatabaseManager::getUserBorrowedItems(int userId) {
    std::vector<LibraryItem*> items;

    if (!db.isOpen()) return items;

    QSqlQuery query(db);
    query.prepare(
        "SELECT ci.* FROM catalogue_items ci "
        "JOIN loans l ON ci.id = l.item_id "
        "WHERE l.user_id = ? AND l.return_date IS NULL"
    );
    query.addBindValue(userId);

    if (query.exec()) {
        while (query.next()) {
            LibraryItem* item = createItemFromQuery(query);
            if (item) {
                items.push_back(item);
            }
        }
    } else {
        qDebug() << "Error getting user borrowed items:" << query.lastError().text();
    }

    return items;
}


bool DatabaseManager::placeHold(int userId, int itemId) {
    if (!db.isOpen()) return false;

    QSqlQuery query(db);

    // Get current highest position in hold queue for this item
    query.prepare("SELECT COALESCE(MAX(position), 0) + 1 as new_position FROM holds WHERE item_id = ?");
    query.addBindValue(itemId);

    int position = 1;
    if (query.exec() && query.next()) {
        position = query.value("new_position").toInt();
    }

    // Insert the hold
    query.prepare("INSERT INTO holds (user_id, item_id, position) VALUES (?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(itemId);
    query.addBindValue(position);

    if (!query.exec()) {
        qDebug() << "Error placing hold:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::cancelHold(int userId, int itemId) {
    if (!db.isOpen()) return false;

    QSqlQuery query(db);

    // Get the position of the hold being cancelled
    query.prepare("SELECT position FROM holds WHERE user_id = ? AND item_id = ?");
    query.addBindValue(userId);
    query.addBindValue(itemId);

    int cancelledPosition = -1;
    if (query.exec() && query.next()) {
        cancelledPosition = query.value("position").toInt();
    }

    // Delete the hold
    query.prepare("DELETE FROM holds WHERE user_id = ? AND item_id = ?");
    query.addBindValue(userId);
    query.addBindValue(itemId);

    if (!query.exec()) {
        qDebug() << "Error cancelling hold:" << query.lastError().text();
        return false;
    }

    // Update positions for remaining holds (decrement positions above the cancelled one)
    if (cancelledPosition != -1) {
        query.prepare("UPDATE holds SET position = position - 1 WHERE item_id = ? AND position > ?");
        query.addBindValue(itemId);
        query.addBindValue(cancelledPosition);
        query.exec(); // We don't care too much if this fails
    }

    return true;
}


std::vector<LibraryItem*> DatabaseManager::getUserHolds(int userId) {
    std::vector<LibraryItem*> items;

    if (!db.isOpen()) return items;

    QSqlQuery query(db);
    query.prepare(
        "SELECT ci.*, h.position FROM catalogue_items ci "
        "JOIN holds h ON ci.id = h.item_id "
        "WHERE h.user_id = ? ORDER BY h.position"
    );
    query.addBindValue(userId);

    if (query.exec()) {
        while (query.next()) {
            LibraryItem* item = createItemFromQuery(query);
            if (item) {
                // Store the position in the item somehow
                // For now, we'll just return the items in order
                items.push_back(item);
            }
        }
    } else {
        qDebug() << "Error getting user holds:" << query.lastError().text();
    }

    return items;
}

LibraryItem* DatabaseManager::getItemById(int id) {
    if (!db.isOpen()) return nullptr;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM catalogue_items WHERE id = ?");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        return createItemFromQuery(query);
    }

    return nullptr;
}

int DatabaseManager::getHoldCountForItem(int itemId) {
    if (!db.isOpen()) return 0;

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) as count FROM holds WHERE item_id = ?");
    query.addBindValue(itemId);

    if (query.exec() && query.next()) {
        return query.value("count").toInt();
    }

    return 0;
}

int DatabaseManager::getHoldPosition(int userId, int itemId) {
    if (!db.isOpen()) return -1;

    QSqlQuery query(db);
    query.prepare("SELECT position FROM holds WHERE user_id = ? AND item_id = ?");
    query.addBindValue(userId);
    query.addBindValue(itemId);

    if (query.exec() && query.next()) {
        return query.value("position").toInt();
    }

    return -1;
}

bool DatabaseManager::addItemToCatalogue(const QString& title, const QString& author,
                                        const QString& itemType, const QString& deweyDecimal,
                                        const QString& isbn, const QString& genre,
                                        const QString& rating, int issueNumber,
                                        const QString& publicationDate, int publicationYear,
                                        const QString& condition) {
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO catalogue_items "
        "(title, author, item_type, dewey_decimal, isbn, genre, rating, "
        "issue_number, publication_date, publication_year, condition, is_available) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, 1)"
    );

    // Convert item type to database format
    QString dbItemType;
    if (itemType == "Fiction Book") dbItemType = "fiction";
    else if (itemType == "Non-Fiction Book") dbItemType = "nonfiction";
    else if (itemType == "Magazine") dbItemType = "magazine";
    else if (itemType == "Movie") dbItemType = "movie";
    else if (itemType == "Video Game") dbItemType = "videogame";
    else dbItemType = "fiction"; // default

    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(dbItemType);
    query.addBindValue(deweyDecimal.isEmpty() ? QVariant() : deweyDecimal);
    query.addBindValue(isbn.isEmpty() ? QVariant() : isbn);
    query.addBindValue(genre.isEmpty() ? QVariant() : genre);
    query.addBindValue(rating.isEmpty() ? QVariant() : rating);
    query.addBindValue(issueNumber == 0 ? QVariant() : issueNumber);
    query.addBindValue(publicationDate.isEmpty() ? QVariant() : publicationDate);
    query.addBindValue(publicationYear);
    query.addBindValue(condition);

    if (!query.exec()) {
        qDebug() << "Error adding item to catalogue:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::removeItemFromCatalogue(int itemId) {
    if (!db.isOpen()) return false;

    QSqlQuery query(db);

    // First check if item is currently borrowed
    query.prepare("SELECT COUNT(*) as count FROM loans WHERE item_id = ? AND return_date IS NULL");
    query.addBindValue(itemId);

    if (query.exec() && query.next()) {
        int activeLoans = query.value("count").toInt();
        if (activeLoans > 0) {
            qDebug() << "Cannot remove item - it is currently borrowed";
            return false;
        }
    }

    // Also check if there are active holds
    query.prepare("SELECT COUNT(*) as count FROM holds WHERE item_id = ?");
    query.addBindValue(itemId);

    if (query.exec() && query.next()) {
        int activeHolds = query.value("count").toInt();
        if (activeHolds > 0) {
            qDebug() << "Cannot remove item - there are active holds";
            return false;
        }
    }

    // Safe to remove - delete from catalogue
    query.prepare("DELETE FROM catalogue_items WHERE id = ?");
    query.addBindValue(itemId);

    if (!query.exec()) {
        qDebug() << "Error removing item from catalogue:" << query.lastError().text();
        return false;
    }

    return true;
}

std::vector<DatabaseManager::LoanInfo> DatabaseManager::getUserLoansWithDates(int userId) {
    std::vector<LoanInfo> loans;

    if (!db.isOpen()) return loans;

    QSqlQuery query(db);
    query.prepare(
        "SELECT ci.*, l.checkout_date, l.due_date FROM catalogue_items ci "
        "JOIN loans l ON ci.id = l.item_id "
        "WHERE l.user_id = ? AND l.return_date IS NULL"
    );
    query.addBindValue(userId);

    if (query.exec()) {
        while (query.next()) {
            LibraryItem* item = createItemFromQuery(query);
            if (item) {
                LoanInfo loan;
                loan.item = item;
                loan.checkoutDate = query.value("checkout_date").toString();
                loan.dueDate = query.value("due_date").toString();
                loans.push_back(loan);
            }
        }
    } else {
        qDebug() << "Error getting user loans with dates:" << query.lastError().text();
    }

    return loans;
}
