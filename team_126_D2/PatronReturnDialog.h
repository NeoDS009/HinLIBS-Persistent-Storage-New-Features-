#ifndef PATRONRETURNDIALOG_H
#define PATRONRETURNDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include "User.h"
#include "LibraryItem.h"
#include "DatabaseManager.h"

/*
    PatronReturnDialog Class:
    Provides a specialized dialog interface for librarians to select and return
    borrowed items on behalf of library patrons. This dialog displays all currently
    borrowed items for a selected patron and allows the librarian to choose which
    item to return.

    Data Members:
      - User* currentPatron: Pointer to the patron whose items are being displayed
      - QListWidget* itemsList: Visual list displaying borrowed items
      - std::vector<LibraryItem*> borrowedItems: Container for patron's borrowed items

    Member Functions:
      Public:
      - PatronReturnDialog(): Constructor that initializes the dialog for a specific patron
      - getSelectedItem(): Returns the currently selected LibraryItem for return
      - ~PatronReturnDialog(): Destructor that cleans up allocated LibraryItem objects

      Private:
        - loadBorrowedItems(): Loads patron's borrowed items from database
*/
class PatronReturnDialog : public QDialog {
    Q_OBJECT

public:
    /*
        Function: PatronReturnDialog
        Purpose: Constructs a dialog displaying all borrowed items for a specific patron.
                 Automatically loads and displays the patron's borrowed items from the database.
        Parameters:
          in: User* patron - Pointer to the patron whose items will be displayed
          in: QWidget* parent - Parent widget for modal behavior (optional)
        Return: PatronReturnDialog instance with loaded patron items
    */
    PatronReturnDialog(User* patron, QWidget *parent = nullptr);

    /*
        Function: getSelectedItem
        Purpose: Retrieves the LibraryItem selected by the librarian for return.
                 Returns the actual item object from the loaded borrowed items list.
        Return: LibraryItem* - Pointer to the selected borrowed item, or nullptr if no selection
    */
    LibraryItem* getSelectedItem() const;

    /*
        Function: ~PatronReturnDialog
        Purpose: Destructor that ensures proper cleanup of dynamically allocated
                 LibraryItem objects loaded from the database. Prevents memory leaks.
    */
    ~PatronReturnDialog();

private:
    User* currentPatron;
    QListWidget *itemsList;
    std::vector<DatabaseManager::LoanInfo> patronLoans;

    /*
        Function: loadBorrowedItems
        Purpose: Queries the DatabaseManager to load all currently borrowed items
                 for the current patron. Populates the visual list and internal
                 storage with the retrieved items.
    */
    void loadBorrowedItems();
};

#endif
