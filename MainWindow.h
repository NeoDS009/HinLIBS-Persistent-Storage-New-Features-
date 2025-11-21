#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include "User.h"
//#include "DataManager.h" //removed, we do not want in-memory
#include "DatabaseManager.h"  // ADD THIS



/*
    MainWindow Class:
    Represents the primary application interface for the HinLIBS library system.
    Provides role-based access to library operations including browsing, borrowing,
    returning, and hold management. Implements the main user workflow and enforces
    business rules through the UI layer.

    Key Responsibilities:
    - Display and manage the library catalogue with real-time availability
    - Handle user authentication state and role-based UI customization
    - Coordinate borrowing/returning operations with business rule enforcement
    - Manage hold queue system with position tracking
    - Maintain synchronization between UI state and data model

    UI Design:
    - Two-panel layout: Catalogue (left) and Account Status (right)
    - Dynamic button states based on selection context
    - Visual indicators for book availability and user's items

    Data Members:
      - User* currentUser: Pointer to the currently authenticated user
      - QListWidget* bookListWidget: Displays the library catalogue
      - QPushButton* borrowButton: Initiates book borrowing process
      - QPushButton* returnButton: Handles book returns
      - QPushButton* holdButton: Places holds on unavailable items
      - QPushButton* cancelHoldButton: Removes existing holds
      - QListWidget* borrowedItemsList: Shows user's currently borrowed books
      - QListWidget* holdsList: Displays user's active hold requests
      - QLabel* accountStatusLabel: Shows borrowing status and limits

    Member Functions:
      - MainWindow(): Constructs the main interface for a specific user
      - refreshCatalogue(): Updates the book display with current status
      - onBookSelected(): Manages UI state based on user selections
      - borrowSelectedBook(): Processes book borrowing with validation
      - returnSelectedBook(): Handles book returns and status updates
      - placeHoldOnSelected(): Manages hold placement in FIFO queues
      - cancelSelectedHold(): Removes holds from queue system
      - refreshAccountStatus(): Updates user's account information display
      - logout(): Terminates session and returns to login screen
      - getSelectedBook(): Retrieves currently selected catalogue item
      - getSelectedBorrowedItem(): Gets selected borrowed book for return
      - updateHoldButtons(): Manages hold-related button states
      - setupUI(): Initializes and arranges all interface components
      - getActiveList(): Determines which list has user focus

    Business Rules Enforced:
      - Maximum of 3 concurrent borrows per patron
      - Books can only be borrowed if available
      - Holds follow first-come-first-served queueing
      - Users cannot place duplicate holds
      - Returned books fulfill holds in queue order
*/
class MainWindow : public QMainWindow {
    Q_OBJECT

public:

    /*
        Function: MainWindow
        Purpose: Constructs the main application window for a specific user
        Parameters:
          in: User* user - Pointer to the authenticated user
          in: QWidget* parent - Parent widget (optional)
        Return: MainWindow instance
    */
    MainWindow(User* user, QWidget *parent = nullptr);

private slots:

    /*
        Function: refreshCatalogue
        Purpose: Updates the catalogue display with current availability and hold counts. Repopulates book list and updates visual status indicators
    */
    void refreshCatalogue();

    /*
        Function: onBookSelected
        Purpose: Manages UI state when user selects items in any list. Updates button states and clears conflicting selections
    */
    void onBookSelected();

    /*
        Function: borrowSelectedBook
        Purpose: Processes book borrowing with full business rule validation. Updates book status, user's borrowed items, and refreshes UI
    */
    void borrowSelectedBook();

    /*
        Function: returnSelectedBook
        Purpose: Handles book returns and triggers hold fulfillment if applicable. Updates book availability, removes from user's items, refreshes UI
    */
    void returnSelectedBook();

    /*
        Function: refreshAccountStatus
        Purpose: Updates the account panel with current borrowing and hold status.
    */
    void refreshAccountStatus();

    /*
        Function: placeHoldOnSelected
        Purpose: Places a hold on an unavailable book and adds user to wait queue
    */
    void placeHoldOnSelected();

    /*
        Function: cancelSelectedHold
        Purpose: Removes an existing hold from the queue system.
    */
    void cancelSelectedHold();

    /*
        Function: logout
        Purpose: Ends the current user session and returns to login screen. Closes main window and triggers application logout cycle
    */
    void logout();

    /*
        Function: showItemDetails
        Purpose: Shows the extended details of an item, when user double-clicks on it. Hence, saving space for the UI.
    */
    void showItemDetails();

    // NEW Librarian slots
    void showAddItemDialog();
    void removeSelectedItem();
    void showReturnForPatronDialog();
    void processPatronReturn(int patronId, int itemId);

private:
    User* currentUser;

    QListWidget *bookListWidget;
    QPushButton *borrowButton;
    QLabel *accountStatusLabel;
    QListWidget *borrowedItemsList;
    QPushButton *returnButton;
    QPushButton *holdButton;
    QPushButton *cancelHoldButton;
    QListWidget *holdsList;


    // NEW Librarian UI members
    QWidget* librarianPanel;
    QPushButton* addItemButton;
    QPushButton* removeItemButton;
    QPushButton* returnForPatronButton;

    void setupLibrarianUI();


    /*
        Function: setupUI
        Purpose: Sets up the UI programatically and precisely using C++, instead of using MainWindow.ui
    */
    void setupUI();

    /*
        Function: getSelectedBook
        Purpose: Retrieves the LibraryItem pointer for selected catalogue item
        Return: LibraryItem* - Selected book or nullptr if no selection
    */
    LibraryItem* getSelectedBook();


    /*
        Function: getSelectedBorrowedItem
        Purpose: Gets the LibraryItem pointer for selected borrowed book
        Return: LibraryItem* - Selected borrowed book or nullptr
    */
    LibraryItem* getSelectedBorrowedItem();


    /*
        Function: updateHoldButtons
        Purpose: Manages enable/disable states for hold-related buttons; Updates hold and cancel hold button availability
    */
    void updateHoldButtons();

    void debugCheckDatabase();

    /*
        Function: getActiveList
        Purpose: Determines which list currently has user selection focus
    */
    enum ActiveList { NONE, CATALOGUE, BORROWED, HOLDS };
    ActiveList getActiveList() {
        if (bookListWidget->currentRow() >= 0) return CATALOGUE;
        if (borrowedItemsList->currentRow() >= 0) return BORROWED;
        if (holdsList && holdsList->currentRow() >= 0) return HOLDS;
        return NONE;
    }
};

#endif
