PREAMBLE
Program Author: 
Deneo [last name], [student number] //last name and student number have been erased for privacy purposes, thank you for understanding!

Purpose:
HinLIBS (Hintonville Library Information and Borrowing System) is a vertical prototype 
for a library management system. It supports three user roles with appropriate permissions:
- Patrons: Browse catalogue, borrow/return items, place/cancel holds, view account status
- Librarians: All patron features, and also adding item to catalogue, removing a selected item
  from the catalogue, and returning item for a selected patron
- Administrators: All librarian features for now, in this deliverable
This prototype demonstrates core library workflows including borrowing with limits,
hold queues, and real-time inventory management. The system meets all specified functional 
requirements for the vertical prototype. Business rules are enforced: 3-book limit, 
hold queues in FIFO order, and availability checks. All data resets when the database file 
has been deleted; otherwise if the system restarts, all changed data remains, thus using 
persistence storage. 

Source Files:
- main.cpp
- MainWindow.cpp
- AddItemDialog.cpp
- DatabaseInitializer.cpp
- DatabaseManager.cpp
- LoginDialog.cpp
- PatronReturnDialog.cpp
- PatronSelectionDialog.cpp

Header Files:
- MainWindow.h
- AddItemDialog.h
- DatabaseInitializer.h
- DatabaseManager.h
- LibraryItem.h
- LoginDialog.h
- PatronReturnDialog.h
- PatronSelectionDialog.h
- User.h

Project File:
- team_126_D2.pro

Data Files:
- n/a -- (hinlibs.db is only created after the system starts)


COMPILATION AND LAUNCHING INSTRUCTIONS:
Primary Method (Recommended):
1.   Extract team_126_D2.zip
2.   Open team_126_D2 folder
3.   Open team_126_D2.pro in Qt Creator (double-click "team_126_D2.pro" file)
4.   Click "Configure Project" when prompted
5.   Build the project (Ctrl+B)
6.   Run the application (Ctrl+R)

Alternative Method (Command Line):
1.   unzip team_126_D2.zip
2.   cd team_126_D2
3.   qmake team_126_D2.pro
4.   make
5.   ./team_126_D2


USAGE INSTRUCTIONS:
Available Usernames (No passwords required, just enter the username and click Login):
- Patrons: alice_p, bob_p, charlie_p, diana_p, eve_p
- Librarian: libby
- Administrator: admin

Borrowing Items:
- Select an available book from the catalogue (left panel; will have tag [AVAILABLE])
- Double-click on an item to view more details
- Click "Borrow Selected Item" button

Returning Items:
- Go to "Books You've Borrowed" list (upper-right panel)  
- Select the book to return
- Click "Return Selected Item" button

Placing Holds:
- Find a checked-out book in the catalogue (will be colored in red (by others) or green (by you) and have tag [CHECKED OUT])
- Select it and click "Place Hold" button
- View your position in the queue in "Your Active Holds"

Cancelling Holds:
- Select a hold from "Your Active Holds" list (lower-right panel)
- Click "Cancel Selected Hold" button

Librarian-specific features: **only if logged in successfully as 'libby' **
Add Item to Catalogue:
- Click "Add New Item to Catalogue" button
- Input desired item details
- Click the "OK" button

Remove Item from Catalogue:
- Select any catalogue item
- Click "Remove Selected Item" button
- Click the "Yes" button

Return Item on Behalf of Patron:
- Click "Return Item for Patron" button
- Select any patron, then click the "OK" button
- Select a patron loan (if applicable), then click the "OK" button


NOTES:
- The UI is implemented programmatically in C++ for dynamic role-based content (in `MainWindow::setupUI()`)
- The system includes robust error handling for database operations
- In the current prototype, closing the main window (via X button or Logout) returns the user to the login screen

- To end the program, close the login window
