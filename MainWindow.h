//#ifndef MAINWINDOW_H
//#define MAINWINDOW_H

//#include <QMainWindow>
//#include <QListWidget>
//#include <QVBoxLayout>
//#include <QHBoxLayout>
//#include <QLabel>
//#include <QPushButton>
//#include <QGroupBox>
//#include "User.h"
////#include "DataManager.h" //removed, we do not want in-memory
//#include "DatabaseManager.h"  // ADD THIS



///*
//    MainWindow Class:
//    Represents the primary application interface for the HinLIBS library system.
//    Provides role-based access to library operations including browsing, borrowing,
//    returning, and hold management. Implements the main user workflow and enforces
//    business rules through the UI layer.

//    Key Responsibilities:
//    - Display and manage the library catalogue with real-time availability
//    - Handle user authentication state and role-based UI customization
//    - Coordinate borrowing/returning operations with business rule enforcement
//    - Manage hold queue system with position tracking
//    - Maintain synchronization between UI state and data model

//    UI Design:
//    - Two-panel layout: Catalogue (left) and Account Status (right)
//    - Dynamic button states based on selection context
//    - Visual indicators for book availability and user's items

//    Data Members:
//      - User* currentUser: Pointer to the currently authenticated user
//      - QListWidget* bookListWidget: Displays the library catalogue
//      - QPushButton* borrowButton: Initiates book borrowing process
//      - QPushButton* returnButton: Handles book returns
//      - QPushButton* holdButton: Places holds on unavailable items
//      - QPushButton* cancelHoldButton: Removes existing holds
//      - QListWidget* borrowedItemsList: Shows user's currently borrowed books
//      - QListWidget* holdsList: Displays user's active hold requests
//      - QLabel* accountStatusLabel: Shows borrowing status and limits

//    Member Functions:
//      - MainWindow(): Constructs the main interface for a specific user
//      - refreshCatalogue(): Updates the book display with current status
//      - onBookSelected(): Manages UI state based on user selections
//      - borrowSelectedBook(): Processes book borrowing with validation
//      - returnSelectedBook(): Handles book returns and status updates
//      - placeHoldOnSelected(): Manages hold placement in FIFO queues
//      - cancelSelectedHold(): Removes holds from queue system
//      - refreshAccountStatus(): Updates user's account information display
//      - logout(): Terminates session and returns to login screen
//      - getSelectedBook(): Retrieves currently selected catalogue item
//      - getSelectedBorrowedItem(): Gets selected borrowed book for return
//      - updateHoldButtons(): Manages hold-related button states
//      - setupUI(): Initializes and arranges all interface components
//      - getActiveList(): Determines which list has user focus

//    Business Rules Enforced:
//      - Maximum of 3 concurrent borrows per patron
//      - Books can only be borrowed if available
//      - Holds follow first-come-first-served queueing
//      - Users cannot place duplicate holds
//      - Returned books fulfill holds in queue order
//*/
//class MainWindow : public QMainWindow {
//    Q_OBJECT

//public:

//    /*
//        Function: MainWindow
//        Purpose: Constructs the main application window for a specific user
//        Parameters:
//          in: User* user - Pointer to the authenticated user
//          in: QWidget* parent - Parent widget (optional)
//        Return: MainWindow instance
//    */
//    MainWindow(User* user, QWidget *parent = nullptr);

//private slots:

//    /*
//        Function: refreshCatalogue
//        Purpose: Updates the catalogue display with current availability and hold counts. Repopulates book list and updates visual status indicators
//    */
//    void refreshCatalogue();

//    /*
//        Function: onBookSelected
//        Purpose: Manages UI state when user selects items in any list. Updates button states and clears conflicting selections
//    */
//    void onBookSelected();

//    /*
//        Function: borrowSelectedBook
//        Purpose: Processes book borrowing with full business rule validation. Updates book status, user's borrowed items, and refreshes UI
//    */
//    void borrowSelectedBook();

//    /*
//        Function: returnSelectedBook
//        Purpose: Handles book returns and triggers hold fulfillment if applicable. Updates book availability, removes from user's items, refreshes UI
//    */
//    void returnSelectedBook();

//    /*
//        Function: refreshAccountStatus
//        Purpose: Updates the account panel with current borrowing and hold status.
//    */
//    void refreshAccountStatus();

//    /*
//        Function: placeHoldOnSelected
//        Purpose: Places a hold on an unavailable book and adds user to wait queue
//    */
//    void placeHoldOnSelected();

//    /*
//        Function: cancelSelectedHold
//        Purpose: Removes an existing hold from the queue system.
//    */
//    void cancelSelectedHold();

//    /*
//        Function: logout
//        Purpose: Ends the current user session and returns to login screen. Closes main window and triggers application logout cycle
//    */
//    void logout();

//    /*
//        Function: showItemDetails
//        Purpose: Shows the extended details of an item, when user double-clicks on it. Hence, saving space for the UI.
//    */
//    void showItemDetails();

//    // NEW Librarian slots
//    void showAddItemDialog();
//    void removeSelectedItem();
//    void showReturnForPatronDialog();
//    void processPatronReturn(int patronId, int itemId);

//private:
//    User* currentUser;

//    QListWidget *bookListWidget;
//    QPushButton *borrowButton;
//    QLabel *accountStatusLabel;
//    QListWidget *borrowedItemsList;
//    QPushButton *returnButton;
//    QPushButton *holdButton;
//    QPushButton *cancelHoldButton;
//    QListWidget *holdsList;


//    // NEW Librarian UI members
//    QWidget* librarianPanel;
//    QPushButton* addItemButton;
//    QPushButton* removeItemButton;
//    QPushButton* returnForPatronButton;

//    void setupLibrarianUI();


//    /*
//        Function: setupUI
//        Purpose: Sets up the UI programatically and precisely using C++, instead of using MainWindow.ui
//    */
//    void setupUI();

//    /*
//        Function: getSelectedBook
//        Purpose: Retrieves the LibraryItem pointer for selected catalogue item
//        Return: LibraryItem* - Selected book or nullptr if no selection
//    */
//    LibraryItem* getSelectedBook();


//    /*
//        Function: getSelectedBorrowedItem
//        Purpose: Gets the LibraryItem pointer for selected borrowed book
//        Return: LibraryItem* - Selected borrowed book or nullptr
//    */
//    LibraryItem* getSelectedBorrowedItem();


//    /*
//        Function: updateHoldButtons
//        Purpose: Manages enable/disable states for hold-related buttons; Updates hold and cancel hold button availability
//    */
//    void updateHoldButtons();

//    void debugCheckDatabase();

//    /*
//        Function: getActiveList
//        Purpose: Determines which list currently has user selection focus
//    */
//    enum ActiveList { NONE, CATALOGUE, BORROWED, HOLDS };
//    ActiveList getActiveList() {
//        if (bookListWidget->currentRow() >= 0) return CATALOGUE;
//        if (borrowedItemsList->currentRow() >= 0) return BORROWED;
//        if (holdsList && holdsList->currentRow() >= 0) return HOLDS;
//        return NONE;
//    }
//};

//#endif










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
#include "DatabaseManager.h"

/*
    MainWindow Class:
    Represents the primary application interface for the HinLIBS library system.
    Provides role-based access to library operations including browsing, borrowing,
    returning, hold management, and librarian administrative functions. Implements
    the main user workflow and enforces business rules through the UI layer.

    Key Responsibilities:
    - Display and manage the library catalogue with real-time availability
    - Handle user authentication state and role-based UI customization
    - Coordinate borrowing/returning operations with business rule enforcement
    - Manage hold queue system with position tracking
    - Provide librarian administrative tools for catalogue management
    - Maintain synchronization between UI state and database persistence

    UI Design:
    - Two-panel layout: Catalogue (left) and Account Status (right)
    - Dynamic button states based on selection context
    - Visual indicators for book availability and user's items
    - Librarian tools panel for administrative functions (right side)

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

      Librarian-specific members:
      - QWidget* librarianPanel: Container for librarian tools
      - QPushButton* addItemButton: Adds new items to catalogue
      - QPushButton* removeItemButton: Removes items from catalogue
      - QPushButton* returnForPatronButton: Processes returns on behalf of patrons

    Member Functions:
      Public:
        - MainWindow(): Constructs the main interface for a specific user

      Private Slots:
        - refreshCatalogue(): Updates the book display with current status
        - onBookSelected(): Manages UI state based on user selections
        - borrowSelectedBook(): Processes book borrowing with validation
        - returnSelectedBook(): Handles book returns and status updates
        - placeHoldOnSelected(): Manages hold placement in FIFO queues
        - cancelSelectedHold(): Removes holds from queue system
        - refreshAccountStatus(): Updates user's account information display
        - logout(): Terminates session and returns to login screen
        - showItemDetails(): Displays comprehensive item information

        Librarian-specific slots:
        - showAddItemDialog(): Opens dialog to add new catalogue items
        - removeSelectedItem(): Removes selected item from catalogue
        - showReturnForPatronDialog(): Opens patron selection for returns
        - processPatronReturn(): Processes returns on behalf of patrons

      Private:
        - setupUI(): Initializes and arranges all interface components
        - setupLibrarianUI(): Creates and configures librarian tools panel
        - getSelectedBook(): Retrieves currently selected catalogue item
        - getSelectedBorrowedItem(): Gets selected borrowed book for return
        - updateHoldButtons(): Manages hold-related button states
        - debugCheckDatabase(): Debug utility for database state verification
        - getActiveList(): Determines which list has user focus

    Database Integration:
      - All operations persist to SQLite database via DatabaseManager
      - Real-time synchronization between UI and database state
      - Automatic data persistence between application sessions

    Business Rules Enforced:
      - Maximum of 3 concurrent borrows per patron
      - Books can only be borrowed if available
      - Holds follow first-come-first-served queueing
      - Users cannot place duplicate holds
      - Returned books fulfill holds in queue order
      - Librarian restrictions: cannot remove borrowed items or items with active holds
*/
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /*
        Function: MainWindow
        Purpose: Constructs the main application window for a specific user with role-based UI
        Parameters:
          in: User* user - Pointer to the authenticated user (determines UI features)
          in: QWidget* parent - Parent widget (optional)
        Return: MainWindow instance with configured interface
    */
    MainWindow(User* user, QWidget *parent = nullptr);

private slots:
    /*
        Function: refreshCatalogue
        Purpose: Updates the catalogue display with current availability and hold counts from database.
                 Repopulates book list and updates visual status indicators. Synchronizes in-memory
                 state with database persistence layer.
    */
    void refreshCatalogue();

    /*
        Function: onBookSelected
        Purpose: Manages UI state when user selects items in any list. Updates button states based
                 on selection context and clears conflicting selections to prevent invalid operations.
    */
    void onBookSelected();

    /*
        Function: borrowSelectedBook
        Purpose: Processes book borrowing with full business rule validation. Updates database
                 availability, user's borrowed items, and refreshes UI. Enforces 3-item limit.
    */
    void borrowSelectedBook();

    /*
        Function: returnSelectedBook
        Purpose: Handles book returns and triggers hold fulfillment if applicable. Updates database
                 availability, removes from user's items, and refreshes UI to reflect changes.
    */
    void returnSelectedBook();

    /*
        Function: refreshAccountStatus
        Purpose: Updates the account panel with current borrowing and hold status from database.
                 Synchronizes in-memory user state with persistent database records.
    */
    void refreshAccountStatus();

    /*
        Function: placeHoldOnSelected
        Purpose: Places a hold on an unavailable book and adds user to wait queue. Enforces
                 no-duplicate-holds rule and calculates queue position from database.
    */
    void placeHoldOnSelected();

    /*
        Function: cancelSelectedHold
        Purpose: Removes an existing hold from the queue system and updates positions of
                 remaining holds in the database.
    */
    void cancelSelectedHold();

    /*
        Function: logout
        Purpose: Ends the current user session and returns to login screen. Closes main window
                 and triggers application logout cycle while preserving database state.
    */
    void logout();

    /*
        Function: showItemDetails
        Purpose: Shows comprehensive item details when user double-clicks on catalogue items.
                 Displays all stored metadata including publication year, condition, and
                 format-specific information.
    */
    void showItemDetails();

    // Librarian Administrative Functions
    /*
        Function: showAddItemDialog
        Purpose: Opens comprehensive dialog for librarians to add new items to catalogue.
                 Supports all item types with type-specific form fields and validation.
    */
    void showAddItemDialog();

    /*
        Function: removeSelectedItem
        Purpose: Removes selected item from catalogue with safety checks. Prevents removal
                 of borrowed items or items with active holds. Includes confirmation dialog.
    */
    void removeSelectedItem();

    /*
        Function: showReturnForPatronDialog
        Purpose: Initiates multi-step process for librarians to return items on behalf of patrons.
                 Includes patron selection and item selection dialogs.
    */
    void showReturnForPatronDialog();

    /*
        Function: processPatronReturn
        Purpose: Processes the actual return operation for patron items. Updates database
                 records and refreshes UI to reflect the return.
        Parameters:
          in: int patronId - Database ID of the patron
          in: int itemId - Database ID of the item to return
    */
    void processPatronReturn(int patronId, int itemId);

private:
    User* currentUser;

    // Core UI Components
    QListWidget *bookListWidget;
    QPushButton *borrowButton;
    QLabel *accountStatusLabel;
    QListWidget *borrowedItemsList;
    QPushButton *returnButton;
    QPushButton *holdButton;
    QPushButton *cancelHoldButton;
    QListWidget *holdsList;

    // Librarian UI Components
    QWidget* librarianPanel;
    QPushButton* addItemButton;
    QPushButton* removeItemButton;
    QPushButton* returnForPatronButton;

    /*
        Function: setupLibrarianUI
        Purpose: Creates and configures the librarian tools panel. Only visible to users
                 with librarian or admin roles. Includes styled buttons and proper layout.
    */
    void setupLibrarianUI();

    /*
        Function: setupUI
        Purpose: Sets up the complete UI programmatically using Qt layouts. Creates main
                 application interface with catalogue and account panels, and conditionally
                 adds librarian tools based on user role.
    */
    void setupUI();

    /*
        Function: getSelectedBook
        Purpose: Retrieves the LibraryItem pointer for selected catalogue item by querying
                 database based on list position.
        Return: LibraryItem* - Selected book or nullptr if no valid selection
    */
    LibraryItem* getSelectedBook();

    /*
        Function: getSelectedBorrowedItem
        Purpose: Gets the LibraryItem pointer for selected borrowed book from user's
                 database-synchronized borrowed items list.
        Return: LibraryItem* - Selected borrowed book or nullptr if invalid selection
    */
    LibraryItem* getSelectedBorrowedItem();

    /*
        Function: updateHoldButtons
        Purpose: Manages enable/disable states for hold-related buttons based on current
                 selections and database state. Updates hold placement and cancellation
                 button availability.
    */
    void updateHoldButtons();

    /*
        Function: debugCheckDatabase
        Purpose: Debug utility method that prints current database state to console.
                 Used for verification and troubleshooting during development.
    */
    void debugCheckDatabase();

    /*
        Function: getActiveList
        Purpose: Determines which list currently has user selection focus to manage
                 button states and prevent conflicting operations.
        Return: ActiveList - Enum indicating which list has active selection
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
