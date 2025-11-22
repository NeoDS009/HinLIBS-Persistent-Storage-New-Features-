//#ifndef LIBRARYITEM_H
//#define LIBRARYITEM_H

//#include <string>
//#include <vector>

//using namespace std;


///*
//    LibraryItem Class:
//    Base class representing an item in the library catalogue. Provides common functionality
//    for all library materials and implements the hold queue management system.
//    Serves as the parent class for specific item types through inheritance.

//    Data Members:
//      - string title: The title of the library item
//      - string author: The author or creator of the item
//      - string format: The type/format of item (e.g., "Fiction Book", "Movie")
//      - bool isAvailable: Current circulation status (true if available for borrowing)
//      - vector<string> holdQueue: FIFO queue of usernames waiting for this item

//    Member Functions:
//      - getDisplayText(): Generates formatted display string with hold information
//      - addHold(): Adds a user to the hold queue
//      - removeHold(): Removes a user from the hold queue
//      - getHoldPosition(): Returns user's position in hold queue (1-based)
//      - getHoldQueue(): Provides access to the hold queue collection
//      - Various getters/setters for item properties
//*/

//class LibraryItem {
//protected:
//    string title;
//    string author;
//    string format;
//    bool isAvailable;
//    vector<string> holdQueue;
//    int publicationYear;
//    string condition;

//public:
//    LibraryItem(string t, string a, string f, int year, string cond)
//        : title(t), author(a), format(f), isAvailable(true),
//          publicationYear(year), condition(cond) {}

//    virtual ~LibraryItem() {}

//    /*
//        Function: getTitle
//        Purpose: Retrieves the title of the library item
//        Return: string - The item's title
//    */
//    string getTitle() const { return title; }

//    /*
//        Function: getAuthor
//        Purpose: Retrieves the author/creator of the library item
//        Return: string - The item's author
//    */
//    string getAuthor() const { return author; }

//    /*
//        Function: getFormat
//        Purpose: Retrieves the format/type of the library item
//        Return: string - The item's format description
//    */
//    string getFormat() const { return format; }

//    /*
//        Function: getAvailability
//        Purpose: Checks if the item is currently available for borrowing
//        Return: bool - True if available, false if checked out
//    */
//    bool getAvailability() const { return isAvailable; }

//    /*
//        Function: setAvailable
//        Purpose: Updates the availability status of the item
//        Parameters:
//          in: bool available - New availability status
//    */
//    void setAvailable(bool available) { isAvailable = available; }

//    /*
//        Function: addHold
//        Purpose: Adds a user to the hold queue for this item (FIFO order), by modifiying the holdQueue vector
//        Parameters:
//          in: string username - The username to add to the queue
//    */
//    void addHold(string username) {
//       holdQueue.push_back(username);
//    }

//    /*
//       Function: removeHold
//       Purpose: Removes a specific user from the hold queue
//       Parameters:
//         in: string username - The username to remove from the queue
//    */
//    void removeHold(string username) {
//       for (auto it = holdQueue.begin(); it != holdQueue.end(); ++it) {
//           if (*it == username) {
//               holdQueue.erase(it);
//               break;
//           }
//       }
//    }

//    /*
//       Function: getHoldPosition
//       Purpose: Finds a user's current position in the hold queue
//       Parameters:
//         in: string username - The username to search for
//       Return: int -- 1-based position in queue, or -1 if not found
//    */
//    int getHoldPosition(string username) {
//       for (size_t i = 0; i < holdQueue.size(); ++i) {
//           if (holdQueue[i] == username) {
//               return i + 1;  // Position (1-based)
//           }
//       }
//       return -1;  // Not in queue
//    }

//    /*
//       Function: getHoldQueue
//       Purpose: Provides direct access to the hold queue vector
//       Return: vector<string>& - Reference to the hold queue
//    */
//    vector<string>& getHoldQueue() { return holdQueue; }

//    /*
//       Function: getDisplayText
//       Purpose: Generates a formatted string for UI display including hold count
//       Return: string - Formatted display text with title, author, format, and hold info
//    */
//    virtual string getDisplayText() const {
//       string text = title + " - " + author + " [" + format + "]";
//       if (!isAvailable && !holdQueue.empty()) {
//           text += " (" + to_string(holdQueue.size()) + " holds)";
//       }
//       return text;
//    }


//    // ADD NEW GETTERS
//    int getPublicationYear() const { return publicationYear; }
//    string getCondition() const { return condition; }


//    // NEW: Method to get detailed information
//    virtual string getDetailedInfo() const {
//        return "Publication Year: " + to_string(publicationYear) + "\n" +
//               "Condition: " + condition;
//    }
//};



///*
//    FictionBook Class:
//    Represents a fiction book in the library catalogue. Inherits from LibraryItem
//    and automatically sets the format to "Fiction Book".

//    Data Members: None (inherits all from LibraryItem)
//    Member Functions: Inherits all from LibraryItem, uses default constructor
//*/
//class FictionBook : public LibraryItem {
//private:
//    string isbn;
//public:
//    FictionBook(string t, string a, int year, string cond, string isbn)
//        : LibraryItem(t, a, "Fiction Book", year, cond), isbn(isbn) {}

//    string getDetailedInfo() const override {
//           return LibraryItem::getDetailedInfo() + "\n" +
//                  "ISBN: " + (isbn.empty() ? "N/A" : isbn);
//       }
//};


///*
//    NonFictionBook Class:
//    Represents a non-fiction book with Dewey Decimal classification.
//    Inherits from LibraryItem and includes additional classification data.

//    Data Members:
//      - string deweyDecimal: Dewey Decimal classification number (XXX.XX format)

//    Member Functions: Inherits all from LibraryItem
//*/
//class NonFictionBook : public LibraryItem {
//private:
//    string deweyDecimal;
//    string isbn;
//public:
//    NonFictionBook(string t, string a, string dewey, int year, string cond, string isbn)
//        : LibraryItem(t, a, "Non-Fiction Book", year, cond),
//          deweyDecimal(dewey), isbn(isbn) {}

//    string getDetailedInfo() const override {
//        return LibraryItem::getDetailedInfo() + "\n" +
//               "Dewey Decimal: " + deweyDecimal + "\n" +
//               "ISBN: " + (isbn.empty() ? "N/A" : isbn);
//    }
//};


///*
//    Magazine Class:
//    Represents a magazine periodical. Inherits from LibraryItem and automatically
//    sets the format to "Magazine".

//    Data Members: None (inherits all from LibraryItem)
//    Member Functions: Inherits all from LibraryItem, uses default constructor
//*/
//class Magazine : public LibraryItem {
//private:
//    int issueNumber;
//    string publicationDate;
//public:
//    Magazine(string t, string a, int issue, string pubDate, int year, string cond)
//        : LibraryItem(t, a, "Magazine", year, cond),
//          issueNumber(issue), publicationDate(pubDate) {}

//    string getDetailedInfo() const override {
//        return LibraryItem::getDetailedInfo() + "\n" +
//               "Issue Number: " + to_string(issueNumber) + "\n" +
//               "Publication Date: " + publicationDate;
//    }
//};


///*
//    Movie Class:
//    Represents a movie in the library collection. Inherits from LibraryItem and
//    automatically sets the format to "Movie".

//    Data Members: None (inherits all from LibraryItem)
//    Member Functions: Inherits all from LibraryItem, uses default constructor
//*/
//class Movie : public LibraryItem {
//private:
//    string genre;
//    string rating;
//public:
//    Movie(string t, string a, string genre, string rating, int year, string cond)
//        : LibraryItem(t, a, "Movie", year, cond),
//          genre(genre), rating(rating) {}

//    string getDetailedInfo() const override {
//        return LibraryItem::getDetailedInfo() + "\n" +
//               "Genre: " + genre + "\n" +
//               "Rating: " + rating;
//    }
//};


///*
//    VideoGame Class:
//    Represents a video game in the library collection. Inherits from LibraryItem and
//    automatically sets the format to "Video Game".

//    Data Members: None (inherits all from LibraryItem)
//    Member Functions: Inherits all from LibraryItem, uses default constructor
//*/
//class VideoGame : public LibraryItem {
//private:
//    string genre;
//    string rating;
//public:
//    VideoGame(string t, string a, string genre, string rating, int year, string cond)
//        : LibraryItem(t, a, "Video Game", year, cond),
//          genre(genre), rating(rating) {}

//    string getDetailedInfo() const override {
//        return LibraryItem::getDetailedInfo() + "\n" +
//               "Genre: " + genre + "\n" +
//               "Rating: " + rating;
//    }
//};

//#endif



#ifndef LIBRARYITEM_H
#define LIBRARYITEM_H

#include <string>
#include <vector>

using namespace std;

/*
    LibraryItem Class Hierarchy:
    Base class and derived classes representing all items in the library catalogue.
    Provides common functionality for all library materials and implements comprehensive
    metadata storage and hold queue management system. Supports polymorphic behavior
    for type-specific detailed information display.

    Inheritance Structure:
    LibraryItem (base)
    ├── FictionBook
    ├── NonFictionBook
    ├── Magazine
    ├── Movie
    └── VideoGame

    Key Features:
    - Comprehensive metadata storage for all item types
    - Hold queue management with FIFO ordering
    - Polymorphic detailed information display
    - Type-specific data fields and validation
    - Database-compatible data structure

    Data Members (LibraryItem base):
      - string title: The title of the library item
      - string author: The author or creator of the item
      - string format: The type/format of item (e.g., "Fiction Book", "Movie")
      - bool isAvailable: Current circulation status (true if available for borrowing)
      - vector<string> holdQueue: FIFO queue of usernames waiting for this item
      - int publicationYear: The year the item was published
      - string condition: Physical condition of the item (Excellent/Good/Fair/Poor)

    Common Member Functions (LibraryItem):
      - getDisplayText(): Generates formatted display string with hold information
      - getDetailedInfo(): Provides comprehensive item metadata for display
      - addHold(): Adds a user to the hold queue
      - removeHold(): Removes a user from the hold queue
      - getHoldPosition(): Returns user's position in hold queue (1-based)
      - Various getters/setters for item properties

    Database Integration:
      - All data members map directly to database table columns
      - Supports object-relational mapping for persistence
      - Compatible with SQLite catalogue_items table structure
*/

class LibraryItem {
protected:
    string title;
    string author;
    string format;
    bool isAvailable;
    vector<string> holdQueue;
    int publicationYear;
    string condition;

public:
    /*
        Function: LibraryItem
        Purpose: Constructs a base library item with comprehensive metadata
        Parameters:
          in: string t - Item title
          in: string a - Author/creator name
          in: string f - Item format/type
          in: int year - Publication year
          in: string cond - Physical condition
    */
    LibraryItem(string t, string a, string f, int year, string cond)
        : title(t), author(a), format(f), isAvailable(true),
          publicationYear(year), condition(cond) {}

    virtual ~LibraryItem() {}

    /*
        Function: getTitle
        Purpose: Retrieves the title of the library item
        Return: string - The item's title
    */
    string getTitle() const { return title; }

    /*
        Function: getAuthor
        Purpose: Retrieves the author/creator of the library item
        Return: string - The item's author
    */
    string getAuthor() const { return author; }

    /*
        Function: getFormat
        Purpose: Retrieves the format/type of the library item
        Return: string - The item's format description
    */
    string getFormat() const { return format; }

    /*
        Function: getAvailability
        Purpose: Checks if the item is currently available for borrowing
        Return: bool - True if available, false if checked out
    */
    bool getAvailability() const { return isAvailable; }

    /*
        Function: setAvailable
        Purpose: Updates the availability status of the item
        Parameters:
          in: bool available - New availability status
    */
    void setAvailable(bool available) { isAvailable = available; }

    /*
        Function: getPublicationYear
        Purpose: Retrieves the publication year of the item
        Return: int - Year of publication
    */
    int getPublicationYear() const { return publicationYear; }

    /*
        Function: getCondition
        Purpose: Retrieves the physical condition of the item
        Return: string - Condition description (Excellent, Good, Fair, Poor)
    */
    string getCondition() const { return condition; }

    /*
        Function: addHold
        Purpose: Adds a user to the hold queue for this item (FIFO order)
        Parameters:
          in: string username - The username to add to the queue
    */
    void addHold(string username) {
       holdQueue.push_back(username);
    }

    /*
       Function: removeHold
       Purpose: Removes a specific user from the hold queue
       Parameters:
         in: string username - The username to remove from the queue
    */
    void removeHold(string username) {
       for (auto it = holdQueue.begin(); it != holdQueue.end(); ++it) {
           if (*it == username) {
               holdQueue.erase(it);
               break;
           }
       }
    }

    /*
       Function: getHoldPosition
       Purpose: Finds a user's current position in the hold queue
       Parameters:
         in: string username - The username to search for
       Return: int - 1-based position in queue, or -1 if not found
    */
    int getHoldPosition(string username) {
       for (size_t i = 0; i < holdQueue.size(); ++i) {
           if (holdQueue[i] == username) {
               return i + 1;  // Position (1-based)
           }
       }
       return -1;  // Not in queue
    }

    /*
       Function: getHoldQueue
       Purpose: Provides direct access to the hold queue vector
       Return: vector<string>& - Reference to the hold queue
    */
    vector<string>& getHoldQueue() { return holdQueue; }

    /*
       Function: getDisplayText
       Purpose: Generates a formatted string for UI display including hold count
       Return: string - Formatted display text with title, author, format, and hold info
    */
    virtual string getDisplayText() const {
       string text = title + " - " + author + " [" + format + "]";
       if (!isAvailable && !holdQueue.empty()) {
           text += " (" + to_string(holdQueue.size()) + " holds)";
       }
       return text;
    }

    /*
       Function: getDetailedInfo
       Purpose: Generates comprehensive item information for detailed views.
                 Base implementation provides publication year and condition.
                 Overridden by derived classes to include type-specific details.
       Return: string - Multi-line detailed information string
    */
    virtual string getDetailedInfo() const {
        return "Publication Year: " + to_string(publicationYear) + "\n" +
               "Condition: " + condition;
    }
};

/*
    FictionBook Class:
    Represents a fiction book in the library catalogue. Extends LibraryItem with
    ISBN support and specialized detailed information.

    Additional Data Members:
      - string isbn: International Standard Book Number

    Polymorphic Behavior:
      - Overrides getDetailedInfo() to include ISBN information
*/
class FictionBook : public LibraryItem {
private:
    string isbn;
public:
    /*
        Function: FictionBook
        Purpose: Constructs a fiction book with complete metadata
        Parameters:
          in: string t - Book title
          in: string a - Author name
          in: int year - Publication year
          in: string cond - Physical condition
          in: string isbn - ISBN number
    */
    FictionBook(string t, string a, int year, string cond, string isbn)
        : LibraryItem(t, a, "Fiction Book", year, cond), isbn(isbn) {}

    /*
        Function: getDetailedInfo
        Purpose: Provides comprehensive fiction book information including ISBN
        Return: string - Multi-line detailed information with book-specific data
    */
    string getDetailedInfo() const override {
        return LibraryItem::getDetailedInfo() + "\n" +
               "ISBN: " + (isbn.empty() ? "N/A" : isbn);
    }
};

/*
    NonFictionBook Class:
    Represents a non-fiction book with Dewey Decimal classification.
    Extends LibraryItem with classification data and ISBN support.

    Additional Data Members:
      - string deweyDecimal: Dewey Decimal classification (XXX.XX format)
      - string isbn: International Standard Book Number

    Polymorphic Behavior:
      - Overrides getDetailedInfo() to include Dewey and ISBN information
*/
class NonFictionBook : public LibraryItem {
private:
    string deweyDecimal;
    string isbn;
public:
    /*
        Function: NonFictionBook
        Purpose: Constructs a non-fiction book with classification data
        Parameters:
          in: string t - Book title
          in: string a - Author name
          in: string dewey - Dewey Decimal classification
          in: int year - Publication year
          in: string cond - Physical condition
          in: string isbn - ISBN number
    */
    NonFictionBook(string t, string a, string dewey, int year, string cond, string isbn)
        : LibraryItem(t, a, "Non-Fiction Book", year, cond),
          deweyDecimal(dewey), isbn(isbn) {}

    /*
        Function: getDetailedInfo
        Purpose: Provides comprehensive non-fiction book information including
                 Dewey Decimal classification and ISBN
        Return: string - Multi-line detailed information with classification data
    */
    string getDetailedInfo() const override {
        return LibraryItem::getDetailedInfo() + "\n" +
               "Dewey Decimal: " + deweyDecimal + "\n" +
               "ISBN: " + (isbn.empty() ? "N/A" : isbn);
    }
};

/*
    Magazine Class:
    Represents a magazine periodical. Extends LibraryItem with issue tracking
    and publication date information.

    Additional Data Members:
      - int issueNumber: Specific issue number
      - string publicationDate: Human-readable publication date

    Polymorphic Behavior:
      - Overrides getDetailedInfo() to include issue-specific information
*/
class Magazine : public LibraryItem {
private:
    int issueNumber;
    string publicationDate;
public:
    /*
        Function: Magazine
        Purpose: Constructs a magazine with issue and publication data
        Parameters:
          in: string t - Magazine title
          in: string a - Publisher/Various
          in: int issue - Issue number
          in: string pubDate - Publication date (e.g., "January 2024")
          in: int year - Publication year
          in: string cond - Physical condition
    */
    Magazine(string t, string a, int issue, string pubDate, int year, string cond)
        : LibraryItem(t, a, "Magazine", year, cond),
          issueNumber(issue), publicationDate(pubDate) {}

    /*
        Function: getDetailedInfo
        Purpose: Provides comprehensive magazine information including
                 issue number and publication date
        Return: string - Multi-line detailed information with issue data
    */
    string getDetailedInfo() const override {
        return LibraryItem::getDetailedInfo() + "\n" +
               "Issue Number: " + to_string(issueNumber) + "\n" +
               "Publication Date: " + publicationDate;
    }
};

/*
    Movie Class:
    Represents a movie in the library collection. Extends LibraryItem with
    genre and content rating information.

    Additional Data Members:
      - string genre: Movie genre/category
      - string rating: Content rating (G, PG, PG-13, R, etc.)

    Polymorphic Behavior:
      - Overrides getDetailedInfo() to include genre and rating information
*/
class Movie : public LibraryItem {
private:
    string genre;
    string rating;
public:
    /*
        Function: Movie
        Purpose: Constructs a movie with genre and rating information
        Parameters:
          in: string t - Movie title
          in: string a - Director/Studio
          in: string genre - Movie genre
          in: string rating - Content rating
          in: int year - Release year
          in: string cond - Physical condition
    */
    Movie(string t, string a, string genre, string rating, int year, string cond)
        : LibraryItem(t, a, "Movie", year, cond),
          genre(genre), rating(rating) {}

    /*
        Function: getDetailedInfo
        Purpose: Provides comprehensive movie information including genre and rating
        Return: string - Multi-line detailed information with media-specific data
    */
    string getDetailedInfo() const override {
        return LibraryItem::getDetailedInfo() + "\n" +
               "Genre: " + genre + "\n" +
               "Rating: " + rating;
    }
};

/*
    VideoGame Class:
    Represents a video game in the library collection. Extends LibraryItem with
    genre and ESRB rating information.

    Additional Data Members:
      - string genre: Game genre/category
      - string rating: ESRB rating (E, E10+, T, M, AO)

    Polymorphic Behavior:
      - Overrides getDetailedInfo() to include genre and rating information
*/
class VideoGame : public LibraryItem {
private:
    string genre;
    string rating;
public:
    /*
        Function: VideoGame
        Purpose: Constructs a video game with genre and rating information
        Parameters:
          in: string t - Game title
          in: string a - Developer/Publisher
          in: string genre - Game genre
          in: string rating - ESRB rating
          in: int year - Release year
          in: string cond - Physical condition
    */
    VideoGame(string t, string a, string genre, string rating, int year, string cond)
        : LibraryItem(t, a, "Video Game", year, cond),
          genre(genre), rating(rating) {}

    /*
        Function: getDetailedInfo
        Purpose: Provides comprehensive video game information including genre and rating
        Return: string - Multi-line detailed information with game-specific data
    */
    string getDetailedInfo() const override {
        return LibraryItem::getDetailedInfo() + "\n" +
               "Genre: " + genre + "\n" +
               "Rating: " + rating;
    }
};

#endif
