PREAMBLE
Program Author: 
Deneo Shi, 101271921

Purpose:
HinLIBS (Hintonville Library Information and Borrowing System) is a vertical prototype 
for a library management system. It supports three user roles with appropriate permissions:
- Patrons: Browse catalogue, borrow/return items, place/cancel holds, view account status
- Librarians & Administrators: All patron features for now, in this deliverable
This prototype demonstrates core library workflows including borrowing with limits,
hold queues, and real-time inventory management. The system meets all specified functional 
requirements for the vertical prototype. Business rules are enforced: 3-book limit, 
hold queues in FIFO order, and availability checks. All data resets when the program restarts, 
thus only using in-memory storage.

Source Files:
- main.cpp
- MainWindow.cpp
- LoginDialog.cpp

Header Files:
- MainWindow.h
- LoginDialog.h
- DataManager.h
- LibraryItem.h
- User.h

Project File:
- Team_126_D1.pro

Data Files:
- NONE - All data is stored in memory (as required)


COMPILATION AND LAUNCHING INSTRUCTIONS:
Primary Method (Recommended):
1.   Extract Team_126_D1.zip
2.   Open Team_126_D1 folder
3.   Open Team_126_D1.pro in Qt Creator (double-click "Team_126_D1.pro" file)
4.   Click "Configure Project" when prompted
5.   Build the project (Ctrl+B)
6.   Run the application (Ctrl+R)

Alternative Method (Command Line):
1.   unzip Team_126_D1.zip
2.   cd Team_126_D1
3.   qmake Team_126_D1.pro
4.   make
5.   ./Team_126_D1


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


NOTES:
- The UI is implemented programmatically in C++ for dynamic role-based content (in `MainWindow::setupUI()`)
- In the current prototype, closing the main window (via X button or Logout) returns the user to the login screen
- To end the program, close the login window