#include <QDebug>
#include <QDate>
#include <QFileInfo>
#include <QDir>
#include "DatabaseInitializer.h"

bool DatabaseInitializer::initializeDatabase(const QString& databasePath) {
    // Use absolute path to be sure
    QString absolutePath = QDir::current().absoluteFilePath(databasePath);
    QFileInfo dbFile(absolutePath);
    bool databaseExists = dbFile.exists();

    // Set up database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
        return false;
    }

    // Always create tables (they won't be recreated if they exist)
    if (!createTables(db)) {
        db.close();
        return false;
    }

    // Only populate default data if database is NEW
    if (!databaseExists) {
        qDebug() << "New database detected - populating with default data";
        if (!populateDefaultData(db)) {
            db.close();
            return false;
        }
    } else {
        qDebug() << "Existing database found - skipping default data population";
    }

    db.close();
    return true;
}

bool DatabaseInitializer::createTables(QSqlDatabase& db) {
    QSqlQuery query;

    // Users table
    QString usersTableSQL =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT DEFAULT '', "
        "role TEXT NOT NULL, "
        "created_date TEXT DEFAULT CURRENT_TIMESTAMP"
        ");";

    if (!query.exec(usersTableSQL)) {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return false;
    }

    // Catalogue items table
    QString catalogueTableSQL =
        "CREATE TABLE IF NOT EXISTS catalogue_items ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT NOT NULL, "
        "author TEXT NOT NULL, "
        "item_type TEXT NOT NULL, "
        "dewey_decimal TEXT, "
        "isbn TEXT, "
        "genre TEXT, "
        "rating TEXT, "
        "issue_number INTEGER, "
        "publication_date TEXT, "
        "publication_year INTEGER, "
        "condition TEXT DEFAULT 'Good', "
        "is_available BOOLEAN DEFAULT 1,"
        "UNIQUE(title, author, publication_year)"
        ");";

    if (!query.exec(catalogueTableSQL)) {
        qDebug() << "Error creating catalogue_items table:" << query.lastError().text();
        return false;
    }

    // Loans table
    QString loansTableSQL =
        "CREATE TABLE IF NOT EXISTS loans ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER NOT NULL, "
        "item_id INTEGER NOT NULL, "
        "checkout_date TEXT NOT NULL, "
        "due_date TEXT NOT NULL, "
        "return_date TEXT, "
        "FOREIGN KEY(user_id) REFERENCES users(id), "
        "FOREIGN KEY(item_id) REFERENCES catalogue_items(id)"
        ");";

    if (!query.exec(loansTableSQL)) {
        qDebug() << "Error creating loans table:" << query.lastError().text();
        return false;
    }

    // Holds table
    QString holdsTableSQL =
        "CREATE TABLE IF NOT EXISTS holds ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER NOT NULL, "
        "item_id INTEGER NOT NULL, "
        "position INTEGER NOT NULL, "
        "created_date TEXT DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY(user_id) REFERENCES users(id), "
        "FOREIGN KEY(item_id) REFERENCES catalogue_items(id)"
        ");";

    if (!query.exec(holdsTableSQL)) {
        qDebug() << "Error creating holds table:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseInitializer::populateDefaultData(QSqlDatabase& db) {
    return addDefaultUsers(db) && addDefaultCatalogue(db);
}

bool DatabaseInitializer::addDefaultUsers(QSqlDatabase& db) {
    QSqlQuery query;
    QString insertUsersSQL =
        "INSERT OR IGNORE INTO users (username, role) VALUES "
        "('alice_p', 'patron'), "
        "('bob_p', 'patron'), "
        "('charlie_p', 'patron'), "
        "('diana_p', 'patron'), "
        "('eve_p', 'patron'), "
        "('libby', 'librarian'), "
        "('admin', 'admin');";

    if (!query.exec(insertUsersSQL)) {
        qDebug() << "Error inserting default users:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseInitializer::addDefaultCatalogue(QSqlDatabase& db) {
    QSqlQuery query(db);

    // First, check if catalogue is already populated
    query.exec("SELECT COUNT(*) as count FROM catalogue_items");
    if (query.next() && query.value("count").toInt() > 0) {
        qDebug() << "Catalogue already has" << query.value("count").toInt() << "items - skipping population";
        return true;
    }

    // We'll insert your 20 catalogue items here
    QString insertCatalogueSQL =
        "INSERT OR IGNORE INTO catalogue_items "
        "(title, author, item_type, dewey_decimal, isbn, genre, rating, issue_number, publication_date, publication_year, condition) VALUES "

        // Fiction Books
        "('The Great Gatsby', 'F. Scott Fitzgerald', 'fiction', NULL, '978-0-7432-7356-5', NULL, NULL, NULL, NULL, 1925, 'Good'), "
        "('To Kill a Mockingbird', 'Harper Lee', 'fiction', NULL, '978-0-06-112008-4', NULL, NULL, NULL, NULL, 1960, 'Excellent'), "
        "('1984', 'George Orwell', 'fiction', NULL, '978-0-452-28423-4', NULL, NULL, NULL, NULL, 1949, 'Good'), "
        "('Pride and Prejudice', 'Jane Austen', 'fiction', NULL, '978-0-14-143951-8', NULL, NULL, NULL, NULL, 1813, 'Fair'), "
        "('The Hobbit', 'J.R.R. Tolkien', 'fiction', NULL, '978-0-547-92822-7', NULL, NULL, NULL, NULL, 1937, 'Excellent'), "

        // Non-Fiction Books
        "('Sapiens', 'Yuval Noah Harari', 'nonfiction', '909.04', '978-0-06-231609-7', NULL, NULL, NULL, NULL, 2011, 'Excellent'), "
        "('Cosmos', 'Carl Sagan', 'nonfiction', '520.92', '978-0-375-50832-3', NULL, NULL, NULL, NULL, 1980, 'Good'), "
        "('A Brief History of Time', 'Stephen Hawking', 'nonfiction', '523.01', '978-0-553-05340-1', NULL, NULL, NULL, NULL, 1988, 'Excellent'), "
        "('The Selfish Gene', 'Richard Dawkins', 'nonfiction', '576.82', '978-0-19-286092-7', NULL, NULL, NULL, NULL, 1976, 'Good'), "
        "('Silent Spring', 'Rachel Carson', 'nonfiction', '632.95', '978-0-618-24906-0', NULL, NULL, NULL, NULL, 1962, 'Fair'), "

        // Magazines
        "('National Geographic', 'Various', 'magazine', NULL, NULL, NULL, NULL, 255, 'January 2024', 2024, 'Excellent'), "
        "('Scientific American', 'Various', 'magazine', NULL, NULL, NULL, NULL, 330, 'March 2024', 2024, 'Good'), "
        "('The Economist', 'Various', 'magazine', NULL, NULL, NULL, NULL, 452, 'February 2024', 2024, 'Excellent'), "

        // Movies
        "('Inception', 'Christopher Nolan', 'movie', NULL, NULL, 'Sci-Fi/Thriller', 'PG-13', NULL, NULL, 2010, 'Excellent'), "
        "('The Shawshank Redemption', 'Frank Darabont', 'movie', NULL, NULL, 'Drama', 'R', NULL, NULL, 1994, 'Good'), "
        "('Spirited Away', 'Hayao Miyazaki', 'movie', NULL, NULL, 'Animation/Fantasy', 'PG', NULL, NULL, 2001, 'Excellent'), "

        // Video Games
        "('The Legend of Zelda: Breath of the Wild', 'Nintendo', 'videogame', NULL, NULL, 'Action-Adventure', 'E10+', NULL, NULL, 2017, 'Excellent'), "
        "('Portal 2', 'Valve', 'videogame', NULL, NULL, 'Puzzle-Platform', 'E10+', NULL, NULL, 2011, 'Good'), "
        "('Minecraft', 'Mojang', 'videogame', NULL, NULL, 'Sandbox/Survival', 'E10+', NULL, NULL, 2011, 'Excellent'), "
        "('Celeste', 'Maddy Makes Games', 'videogame', NULL, NULL, 'Platformer', 'E10+', NULL, NULL, 2018, 'Good');";

    if (!query.exec(insertCatalogueSQL)) {
        qDebug() << "Error inserting default catalogue:" << query.lastError().text();
        return false;
    }

    return true;
}
