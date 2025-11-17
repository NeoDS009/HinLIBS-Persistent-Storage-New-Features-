#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "LibraryItem.h"

/*
    User Class:
    Represents a library system user with borrowing privileges, hold management, and account state.
    Serves as the central entity for tracking user interactions with the library collection.
    Used by all system roles (Patron, Librarian, Administrator) for borrowing operations.

    Data Members:
      - string name: Unique identifier for the user (username or library card number)
      - string role: User type classification ("patron", "librarian", or "admin")
      - vector<LibraryItem*> borrowedItems: Collection of items currently checked out by the user
      - vector<LibraryItem*> activeHolds: Collection of items the user has placed holds on

    Member Functions:
      - User(): Constructor that initializes user with name and role
      - canBorrow(): Checks if user is under the maximum borrowing limit
      - borrowItem(): Adds an item to user's borrowed collection
      - returnItem(): Removes an item from user's borrowed collection
      - addHold(): Registers a new hold on an unavailable item
      - removeHold(): Cancels an existing hold registration
      - hasHoldOn(): Checks if user already has a hold on a specific item

    Business Rules Enforced:
      - Maximum of 3 borrowed items per user (enforced by canBorrow())
      - Prevents duplicate holds on the same item
      - Maintains separation between borrowed items and active holds
*/
class User {
public:
    int id; //new for database
    string name;
    string role;
    vector<LibraryItem*> borrowedItems;
    vector<LibraryItem*> activeHolds;

    /*
        Function: User (Constructor)
        Purpose: Initializes a new user with specified name and role
        Parameters:
          in: int i - id for user
          in: string n - Username or unique identifier for the user
          in: string r - Role classification ("patron", "librarian", "admin")
        Return: User object instance
    */
    User(int i, string n, string r) : id(i), name(n), role(r) {}

    /*
        Function: canBorrow
        Purpose: Determines if user can borrow additional items based on system limits (Enforces the business rule of maximum 3 active loans per patron)
        Return: bool - True if user has fewer than 3 borrowed items, False otherwise
    */
    bool canBorrow() const {
        return borrowedItems.size() < 3;  // Max 3 books
    }

    /*
        Function: borrowItem
        Purpose: Adds a library item to user's collection of borrowed items. Modifies the borrowedItems vector by adding the new item
        Parameters:
          in: LibraryItem* item - Pointer to the item being borrowed
    */
    void borrowItem(LibraryItem* item) {
        borrowedItems.push_back(item);
    }

    /*
        Function: returnItem
        Purpose: Removes a library item from user's borrowed collection. Modifies the borrowedItems vector by removing the specified item
        Parameters:
          in: LibraryItem* item - Pointer to the item being returned
    */
    void returnItem(LibraryItem* item) {
        for (auto it = borrowedItems.begin(); it != borrowedItems.end(); ++it) {
            if (*it == item) {
                borrowedItems.erase(it);
                break;
            }
        }
    }

    /*
        Function: addHold
        Purpose: Registers a hold request for an unavailable library item. Adds the item to user's activeHolds collection
        Parameters:
          in: LibraryItem* item - Pointer to the item being requested
    */
    void addHold(LibraryItem* item) {
        activeHolds.push_back(item);
    }

    /*
        Function: removeHold
        Purpose: Cancels an existing hold request for a library item. Removes the item from user's activeHolds collection
        Parameters:
          in: LibraryItem* item - Pointer to the item hold being cancelled
    */
    void removeHold(LibraryItem* item) {
        for (auto it = activeHolds.begin(); it != activeHolds.end(); ++it) {
            if (*it == item) {
                activeHolds.erase(it);
                break;
            }
        }
    }

    /*
        Function: hasHoldOn
        Purpose: Checks if user already has an active hold on a specific library item
        Parameters:
          in: LibraryItem* item - Pointer to the item to check for existing holds
        Return: bool - True if user has an active hold on the item, False otherwise
    */
    bool hasHoldOn(LibraryItem* item) {
        for (auto hold : activeHolds) {
            if (hold == item) {
                return true;
            }
        }
        return false;
    }
};

#endif
