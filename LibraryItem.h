#ifndef LIBRARYITEM_H
#define LIBRARYITEM_H

#include <string>
#include <vector>

using namespace std;


/*
    LibraryItem Class:
    Base class representing an item in the library catalogue. Provides common functionality
    for all library materials and implements the hold queue management system.
    Serves as the parent class for specific item types through inheritance.

    Data Members:
      - string title: The title of the library item
      - string author: The author or creator of the item
      - string format: The type/format of item (e.g., "Fiction Book", "Movie")
      - bool isAvailable: Current circulation status (true if available for borrowing)
      - vector<string> holdQueue: FIFO queue of usernames waiting for this item

    Member Functions:
      - getDisplayText(): Generates formatted display string with hold information
      - addHold(): Adds a user to the hold queue
      - removeHold(): Removes a user from the hold queue
      - getHoldPosition(): Returns user's position in hold queue (1-based)
      - getHoldQueue(): Provides access to the hold queue collection
      - Various getters/setters for item properties
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
        Function: addHold
        Purpose: Adds a user to the hold queue for this item (FIFO order), by modifiying the holdQueue vector
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
       Return: int -- 1-based position in queue, or -1 if not found
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


    // ADD NEW GETTERS
    int getPublicationYear() const { return publicationYear; }
    string getCondition() const { return condition; }


    // NEW: Method to get detailed information
    virtual string getDetailedInfo() const {
        return "Publication Year: " + to_string(publicationYear) + "\n" +
               "Condition: " + condition;
    }
};



/*
    FictionBook Class:
    Represents a fiction book in the library catalogue. Inherits from LibraryItem
    and automatically sets the format to "Fiction Book".

    Data Members: None (inherits all from LibraryItem)
    Member Functions: Inherits all from LibraryItem, uses default constructor
*/
class FictionBook : public LibraryItem {
private:
    string isbn;
public:
    FictionBook(string t, string a, int year, string cond, string isbn)
        : LibraryItem(t, a, "Fiction Book", year, cond), isbn(isbn) {}

    string getDetailedInfo() const override {
           return LibraryItem::getDetailedInfo() + "\n" +
                  "ISBN: " + (isbn.empty() ? "N/A" : isbn);
       }
};


/*
    NonFictionBook Class:
    Represents a non-fiction book with Dewey Decimal classification.
    Inherits from LibraryItem and includes additional classification data.

    Data Members:
      - string deweyDecimal: Dewey Decimal classification number (XXX.XX format)

    Member Functions: Inherits all from LibraryItem
*/
class NonFictionBook : public LibraryItem {
private:
    string deweyDecimal;
    string isbn;
public:
    NonFictionBook(string t, string a, string dewey, int year, string cond, string isbn)
        : LibraryItem(t, a, "Non-Fiction Book", year, cond),
          deweyDecimal(dewey), isbn(isbn) {}

    string getDetailedInfo() const override {
        return LibraryItem::getDetailedInfo() + "\n" +
               "Dewey Decimal: " + deweyDecimal + "\n" +
               "ISBN: " + (isbn.empty() ? "N/A" : isbn);
    }
};


/*
    Magazine Class:
    Represents a magazine periodical. Inherits from LibraryItem and automatically
    sets the format to "Magazine".

    Data Members: None (inherits all from LibraryItem)
    Member Functions: Inherits all from LibraryItem, uses default constructor
*/
class Magazine : public LibraryItem {
private:
    int issueNumber;
    string publicationDate;
public:
    Magazine(string t, string a, int issue, string pubDate, int year, string cond)
        : LibraryItem(t, a, "Magazine", year, cond),
          issueNumber(issue), publicationDate(pubDate) {}

    string getDetailedInfo() const override {
        return LibraryItem::getDetailedInfo() + "\n" +
               "Issue Number: " + to_string(issueNumber) + "\n" +
               "Publication Date: " + publicationDate;
    }
};


/*
    Movie Class:
    Represents a movie in the library collection. Inherits from LibraryItem and
    automatically sets the format to "Movie".

    Data Members: None (inherits all from LibraryItem)
    Member Functions: Inherits all from LibraryItem, uses default constructor
*/
class Movie : public LibraryItem {
private:
    string genre;
    string rating;
public:
    Movie(string t, string a, string genre, string rating, int year, string cond)
        : LibraryItem(t, a, "Movie", year, cond),
          genre(genre), rating(rating) {}

    string getDetailedInfo() const override {
        return LibraryItem::getDetailedInfo() + "\n" +
               "Genre: " + genre + "\n" +
               "Rating: " + rating;
    }
};


/*
    VideoGame Class:
    Represents a video game in the library collection. Inherits from LibraryItem and
    automatically sets the format to "Video Game".

    Data Members: None (inherits all from LibraryItem)
    Member Functions: Inherits all from LibraryItem, uses default constructor
*/
class VideoGame : public LibraryItem {
private:
    string genre;
    string rating;
public:
    VideoGame(string t, string a, string genre, string rating, int year, string cond)
        : LibraryItem(t, a, "Video Game", year, cond),
          genre(genre), rating(rating) {}

    string getDetailedInfo() const override {
        return LibraryItem::getDetailedInfo() + "\n" +
               "Genre: " + genre + "\n" +
               "Rating: " + rating;
    }
};

#endif
