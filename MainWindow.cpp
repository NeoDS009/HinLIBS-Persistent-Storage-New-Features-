#include "MainWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QCloseEvent>
#include "DataManager.h"
//#include "AddItemDialog.h"
#include "PatronSelectionDialog.h"
#include "PatronReturnDialog.h"


MainWindow::MainWindow(User* user, QWidget *parent)
    : QMainWindow(parent), currentUser(user) {

    setWindowTitle("HinLIBS - Hintonville Library System");
    setFixedSize(1200, 800);

    setupUI();
    refreshCatalogue();
    refreshAccountStatus();

        debugCheckDatabase();  // ADD THIS
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Left Panel: Catalogue
    QVBoxLayout *leftLayout = new QVBoxLayout();

    // Welcome section with logout
    QHBoxLayout *welcomeLayout = new QHBoxLayout();
    QLabel *welcomeLabel = new QLabel(
        QString("Welcome, %1! (Role: %2)").arg(
            QString::fromStdString(currentUser->name),
            QString::fromStdString(currentUser->role)
        )
    );
    QPushButton *logoutButton = new QPushButton("Logout");
    welcomeLayout->addWidget(welcomeLabel);
    welcomeLayout->addWidget(logoutButton);
    leftLayout->addLayout(welcomeLayout);

    // Catalogue display
    QLabel *catalogueLabel = new QLabel("Library Catalogue:");
    leftLayout->addWidget(catalogueLabel);

    bookListWidget = new QListWidget();
    leftLayout->addWidget(bookListWidget);

    // Action buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    borrowButton = new QPushButton("Borrow Selected Item");
    borrowButton->setEnabled(false);

    returnButton = new QPushButton("Return Selected Item");
    returnButton->setEnabled(false);

    holdButton = new QPushButton("Place Hold");
    holdButton->setEnabled(false);

    buttonLayout->addWidget(borrowButton);
    buttonLayout->addWidget(returnButton);
    buttonLayout->addWidget(holdButton);
    leftLayout->addLayout(buttonLayout);

    // Right Panel: Account Status
    QVBoxLayout *rightLayout = new QVBoxLayout();

    QLabel *accountLabel = new QLabel("Your Account Status:");
    rightLayout->addWidget(accountLabel);

    accountStatusLabel = new QLabel();
    rightLayout->addWidget(accountStatusLabel);

    QLabel *borrowedLabel = new QLabel("Items You've Borrowed:");
    rightLayout->addWidget(borrowedLabel);

    borrowedItemsList = new QListWidget();
    rightLayout->addWidget(borrowedItemsList);

    QLabel *holdsLabel = new QLabel("Your Active Holds:");
    rightLayout->addWidget(holdsLabel);

    holdsList = new QListWidget();
    rightLayout->addWidget(holdsList);

    cancelHoldButton = new QPushButton("Cancel Selected Hold");
    cancelHoldButton->setEnabled(false);
    rightLayout->addWidget(cancelHoldButton);

    // Signal connections
    connect(bookListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::onBookSelected);
    connect(borrowedItemsList, &QListWidget::itemSelectionChanged, this, &MainWindow::onBookSelected);
    connect(holdsList, &QListWidget::itemSelectionChanged, this, &MainWindow::updateHoldButtons);

    connect(borrowButton, &QPushButton::clicked, this, &MainWindow::borrowSelectedBook);
    connect(returnButton, &QPushButton::clicked, this, &MainWindow::returnSelectedBook);
    connect(holdButton, &QPushButton::clicked, this, &MainWindow::placeHoldOnSelected);
    connect(cancelHoldButton, &QPushButton::clicked, this, &MainWindow::cancelSelectedHold);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::logout);

    connect(bookListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::showItemDetails);

    // Layout configuration
    mainLayout->addLayout(leftLayout, 2);
    mainLayout->addLayout(rightLayout, 2);
    setCentralWidget(centralWidget);

    // AFTER your existing UI is set up, add librarian panel
    setupLibrarianUI();
}



//void MainWindow::setupLibrarianUI() {
//    // Only show librarian features for librarians/admins
//    if (currentUser->role != "librarian" && currentUser->role != "admin") {
//        return;
//    }

//    // Create librarian panel
//    librarianPanel = new QWidget();
//    QVBoxLayout* librarianLayout = new QVBoxLayout(librarianPanel);

//    QLabel* librarianLabel = new QLabel("Librarian Tools");
//    librarianLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
//    librarianLayout->addWidget(librarianLabel);

//    // Librarian buttons
//    addItemButton = new QPushButton("Add New Item to Catalogue");
//    removeItemButton = new QPushButton("Remove Selected Item");
//    returnForPatronButton = new QPushButton("Return Item for Patron");

//    librarianLayout->addWidget(addItemButton);
//    librarianLayout->addWidget(removeItemButton);
//    librarianLayout->addWidget(returnForPatronButton);

//    // Add librarian panel to main layout (right side)
//    QHBoxLayout* mainLayout = qobject_cast<QHBoxLayout*>(centralWidget()->layout());
//    if (mainLayout) {
//        mainLayout->addWidget(librarianPanel, 1); // Add to right side
//    }

//    // Connect librarian buttons
//    connect(addItemButton, &QPushButton::clicked, this, &MainWindow::showAddItemDialog);
//    connect(removeItemButton, &QPushButton::clicked, this, &MainWindow::removeSelectedItem);
//    connect(returnForPatronButton, &QPushButton::clicked, this, &MainWindow::showReturnForPatronDialog);
//}

void MainWindow::setupLibrarianUI() {
    // Only show librarian features for librarians/admins
    if (currentUser->role != "librarian" && currentUser->role != "admin") {
        return;
    }

    // Create librarian panel
    librarianPanel = new QWidget();
    QVBoxLayout* librarianLayout = new QVBoxLayout(librarianPanel);

    // Add some margins and spacing for better appearance
    librarianLayout->setContentsMargins(10, 10, 10, 10); // Left, Top, Right, Bottom
    librarianLayout->setSpacing(15); // More space between widgets

    QLabel* librarianLabel = new QLabel("Librarian Tools");
    librarianLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #2c3e50;");
    librarianLabel->setAlignment(Qt::AlignCenter); // Center the text
    librarianLayout->addWidget(librarianLabel);

    // Add a subtle separator line
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("color: #bdc3c7;");
    librarianLayout->addWidget(line);

    // Librarian buttons
    addItemButton = new QPushButton("Add New Item to Catalogue");
    removeItemButton = new QPushButton("Remove Selected Item");
    returnForPatronButton = new QPushButton("Return Item for Patron");

    // Style the buttons for a more professional look
//    QString buttonStyle = "QPushButton { padding: 8px; font-weight: bold; }";
    QString buttonStyle = "QPushButton { padding: 8px; }";
    addItemButton->setStyleSheet(buttonStyle);
    removeItemButton->setStyleSheet(buttonStyle);
    returnForPatronButton->setStyleSheet(buttonStyle);

    librarianLayout->addWidget(addItemButton);
    librarianLayout->addWidget(removeItemButton);
    librarianLayout->addWidget(returnForPatronButton);

    // Add stretch to push everything to the top
    librarianLayout->addStretch();

    // Add librarian panel to main layout (right side)
    QHBoxLayout* mainLayout = qobject_cast<QHBoxLayout*>(centralWidget()->layout());
    if (mainLayout) {
        mainLayout->addWidget(librarianPanel, 1); // Add to right side
    }

    // Connect librarian buttons
    connect(addItemButton, &QPushButton::clicked, this, &MainWindow::showAddItemDialog);
    connect(removeItemButton, &QPushButton::clicked, this, &MainWindow::removeSelectedItem);
    connect(returnForPatronButton, &QPushButton::clicked, this, &MainWindow::showReturnForPatronDialog);
}




void MainWindow::showAddItemDialog() {
    AddItemDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        // Get all the data from the dialog
        bool success = DatabaseManager::getInstance().addItemToCatalogue(
            dialog.getTitle(),
            dialog.getAuthor(),
            dialog.getItemType(),
            dialog.getDeweyDecimal(),
            dialog.getISBN(),
            dialog.getGenre(),
            dialog.getRating(),
            dialog.getIssueNumber(),
            dialog.getPublicationDate(),
            dialog.getPublicationYear(),
            dialog.getCondition()
        );

        if (success) {
            QMessageBox::information(this, "Success", "Item added to catalogue successfully!");
            refreshCatalogue();
        } else {
            QMessageBox::warning(this, "Error", "Failed to add item to catalogue.");
        }
    }
}



// TEMPORARY IMPLEMENTATIONS - We'll fill these in next
void MainWindow::removeSelectedItem() {
    LibraryItem* selected = getSelectedBook();

    if (!selected) {
        QMessageBox::warning(this, "Error", "Please select an item to remove!");
        return;
    }

    int itemId = DatabaseManager::getInstance().getItemId(selected);
    if (itemId == -1) {
        QMessageBox::warning(this, "Error", "Could not find item in database!");
        return;
    }

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Removal",
                                 QString("Are you sure you want to remove:\n\"%1\"\n\nThis action cannot be undone!")
                                 .arg(QString::fromStdString(selected->getTitle())),
                                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool success = DatabaseManager::getInstance().removeItemFromCatalogue(itemId);
        if (success) {
            QMessageBox::information(this, "Success", "Item removed from catalogue!");
            refreshCatalogue();
        } else {
            QMessageBox::warning(this, "Error",
                               "Could not remove item. It may be currently borrowed or have active holds.");
        }
    }
}


void MainWindow::showReturnForPatronDialog() {
    // Step 1: Select a patron
    PatronSelectionDialog patronDialog(this);
    if (patronDialog.exec() == QDialog::Accepted) {
        User* selectedPatron = patronDialog.getSelectedPatron();

        if (selectedPatron) {
            // Step 2: Show patron's borrowed items
            PatronReturnDialog returnDialog(selectedPatron, this);
            if (returnDialog.exec() == QDialog::Accepted) {
                LibraryItem* selectedItem = returnDialog.getSelectedItem();

                if (selectedItem) {
                    // Step 3: Process the return
                    int itemId = DatabaseManager::getInstance().getItemId(selectedItem);
                    processPatronReturn(selectedPatron->id, itemId);
                } else {
                    QMessageBox::information(this, "Info", "No item selected.");
                }
            }
        } else {
            QMessageBox::warning(this, "Error", "No patron selected.");
        }
    }
}

void MainWindow::processPatronReturn(int patronId, int itemId) {
    bool success = DatabaseManager::getInstance().returnItem(patronId, itemId);

    if (success) {
        // Get patron name for success message
        auto allUsers = DatabaseManager::getInstance().getAllUsers();
        QString patronName;
        for (auto user : allUsers) {
            if (user->id == patronId) {
                patronName = QString::fromStdString(user->name);
                delete user; // Clean up
            } else {
                delete user; // Clean up
            }
        }

        QMessageBox::information(this, "Success",
                               QString("Successfully returned item for patron: %1").arg(patronName));
        refreshCatalogue(); // Refresh to show item is available again
    } else {
        QMessageBox::warning(this, "Error", "Failed to return item.");
    }
}


void MainWindow::logout() {
    this->close();
}

//void MainWindow::refreshCatalogue() {
//    // Preserve selection across refresh
//    QString previouslySelectedTitle;
//    LibraryItem* selected = getSelectedBook();
//    if (selected) {
//        previouslySelectedTitle = QString::fromStdString(selected->getTitle());
//    }

//    bookListWidget->clear();
////    auto& catalogue = DataManager::getInstance().getCatalogue(); //we do not want memory
//    auto catalogue = DatabaseManager::getInstance().getAllCatalogueItems();


//    for (auto item : catalogue) {
//        QString displayText = QString::fromStdString(item->getDisplayText());

//        // Determine item status for display
//        bool userHasBook = false;
//        for (auto borrowed : currentUser->borrowedItems) {
//            if (borrowed == item) {
//                userHasBook = true;
//                break;
//            }
//        }

//        // Status indicators
//        if (item->getAvailability() && !userHasBook) {
//            displayText += " [AVAILABLE]";
//        } else if (userHasBook) {
//            displayText += " [BORROWED BY YOU]";
//        } else {
//            displayText += " [CHECKED OUT]";
//        }

//        QListWidgetItem* listItem = new QListWidgetItem(displayText);

//        // Visual status coding
//        if (userHasBook) {
//            listItem->setBackground(QBrush(QColor(200, 255, 200)));
//        } else if (!item->getAvailability()) {
//            listItem->setBackground(QBrush(QColor(255, 200, 200)));
//        }

//        bookListWidget->addItem(listItem);

//        // Restore previous selection
//        if (!previouslySelectedTitle.isEmpty() &&
//            previouslySelectedTitle == QString::fromStdString(item->getTitle())) {
//            listItem->setSelected(true);
//        }
//    }

//    onBookSelected();
//    refreshAccountStatus();
//}

//database: try 1-2
void MainWindow::refreshCatalogue() {
    // Preserve selection across refresh
    QString previouslySelectedTitle;
    LibraryItem* selected = getSelectedBook();
    if (selected) {
        previouslySelectedTitle = QString::fromStdString(selected->getTitle());
    }

    bookListWidget->clear();

    // Load from database - this reflects the TRUE availability
    auto catalogue = DatabaseManager::getInstance().getAllCatalogueItems();

    for (auto item : catalogue) {
        QString displayText = QString::fromStdString(item->getDisplayText());

        // Get hold count for this item
        int itemId = DatabaseManager::getInstance().getItemId(item);
        int holdCount = DatabaseManager::getInstance().getHoldCountForItem(itemId);

        // Use the REAL availability from database
        if (item->getAvailability()) {
            displayText += " [AVAILABLE]";
        } else {
            displayText += " [CHECKED OUT]";
        }

        // Add hold count if there are holds
        if (holdCount > 0) {
            displayText += QString(" (%1 holds)").arg(holdCount);
        }

        QListWidgetItem* listItem = new QListWidgetItem(displayText);

        // Visual status coding based on REAL availability
        if (!item->getAvailability()) {
            listItem->setBackground(QBrush(QColor(255, 200, 200))); // Light red for checked out
        }

        bookListWidget->addItem(listItem);

        // Restore previous selection
        if (!previouslySelectedTitle.isEmpty() &&
            previouslySelectedTitle == QString::fromStdString(item->getTitle())) {
            listItem->setSelected(true);
        }
    }

    onBookSelected();
    refreshAccountStatus();
}

//void MainWindow::showItemDetails() {
//    LibraryItem* item = getSelectedBook();
//    if (!item) {
//        item = getSelectedBorrowedItem();
//    }
//    if (!item) return;

//    QString details = QString("Title: %1\nAuthor: %2\nFormat: %3\nPublication Year: %4\nCondition: %5")
//        .arg(QString::fromStdString(item->getTitle()))
//        .arg(QString::fromStdString(item->getAuthor()))
//        .arg(QString::fromStdString(item->getFormat()))
//        .arg(2023)
//        .arg("Good");

//    QMessageBox::information(this, "Item Details", details);
//}

void MainWindow::showItemDetails() {
    LibraryItem* item = getSelectedBook();
    if (!item) {
        item = getSelectedBorrowedItem();
    }
    if (!item) return;

    QString details = QString("Title: %1\nAuthor: %2\nFormat: %3\n%4")
        .arg(QString::fromStdString(item->getTitle()))
        .arg(QString::fromStdString(item->getAuthor()))
        .arg(QString::fromStdString(item->getFormat()))
        .arg(QString::fromStdString(item->getDetailedInfo()));

    QMessageBox::information(this, "Item Details", details);
}

void MainWindow::onBookSelected() {
    LibraryItem* selectedBook = getSelectedBook();
    LibraryItem* selectedBorrowed = getSelectedBorrowedItem();

    borrowButton->setEnabled(selectedBook && selectedBook->getAvailability() && currentUser->canBorrow());
    returnButton->setEnabled(selectedBorrowed != nullptr);
    updateHoldButtons();
}




//void MainWindow::borrowSelectedBook() {
//    LibraryItem* selected = getSelectedBook();

//    if (!selected) return;

//    // DEBUG: Check if we can find the item in database
//    int itemId = DatabaseManager::getInstance().getItemId(selected);
//    qDebug() << "Attempting to borrow:" << QString::fromStdString(selected->getTitle())
//             << "Item ID:" << itemId << "User ID:" << currentUser->id;

//    if (itemId == -1) {
//        QMessageBox::warning(this, "Error", "Could not find book in database!");
//        return;
//    }

//    // For now, just show a message but don't actually update database
//    QMessageBox::information(this, "Debug",
//        QString("Would borrow: %1\nItem ID: %2\nUser ID: %3")
//        .arg(QString::fromStdString(selected->getTitle()))
//        .arg(itemId)
//        .arg(currentUser->id));

//    // Keep in-memory working for now
//    selected->setAvailable(false);
//    currentUser->borrowItem(selected);

//    refreshCatalogue();
//    refreshAccountStatus();
//}


void MainWindow::borrowSelectedBook() {
    LibraryItem* selected = getSelectedBook();

    if (!selected) {
        QMessageBox::warning(this, "Error", "Please select a book first!");
        return;
    }

    if (!selected->getAvailability()) {
        QMessageBox::warning(this, "Error", "This book is already checked out!");
        return;
    }

    if (!currentUser->canBorrow()) {
        QMessageBox::warning(this, "Error",
            "You have reached the maximum of 3 borrowed books. Please return one first!");
        return;
    }

    // UPDATE DATABASE
    int itemId = DatabaseManager::getInstance().getItemId(selected);
    if (itemId == -1) {
        QMessageBox::warning(this, "Error", "Could not find book in database!");
        return;
    }

    qDebug() << "Attempting database borrow - User ID:" << currentUser->id << "Item ID:" << itemId;

    bool success = DatabaseManager::getInstance().borrowItem(currentUser->id, itemId);
    if (!success) {
        QMessageBox::warning(this, "Error", "Failed to borrow book in database!");
        return;
    }

    // Also update in-memory for current session
    selected->setAvailable(false);
    currentUser->borrowItem(selected);

    QMessageBox::information(this, "Success",
        QString("You have successfully borrowed: %1")
        .arg(QString::fromStdString(selected->getTitle())));

    refreshCatalogue();
    refreshAccountStatus();
}

//void MainWindow::returnSelectedBook() {
//    LibraryItem* selected = getSelectedBorrowedItem();

//    if (!selected) {
//        QMessageBox::warning(this, "Error", "Please select one of YOUR borrowed books to return!");
//        return;
//    }

//    // Process successful return
//    selected->setAvailable(true);
//    currentUser->returnItem(selected);

//    QMessageBox::information(this, "Success",
//        QString("You have successfully returned: %1")
//        .arg(QString::fromStdString(selected->getTitle())));

//    refreshCatalogue();
//    refreshAccountStatus();
//}

//using Database: try 1
void MainWindow::returnSelectedBook() {
    LibraryItem* selected = getSelectedBorrowedItem();

    if (!selected) {
        QMessageBox::warning(this, "Error", "Please select one of YOUR borrowed books to return!");
        return;
    }

    // UPDATE DATABASE
    int itemId = DatabaseManager::getInstance().getItemId(selected);
    if (itemId == -1) {
        QMessageBox::warning(this, "Error", "Could not find book in database!");
        return;
    }

    bool success = DatabaseManager::getInstance().returnItem(currentUser->id, itemId);
    if (!success) {
        QMessageBox::warning(this, "Error", "Failed to return book in database!");
        return;
    }

    // Also update in-memory
    selected->setAvailable(true);
    currentUser->returnItem(selected);

    QMessageBox::information(this, "Success",
        QString("You have successfully returned: %1")
        .arg(QString::fromStdString(selected->getTitle())));

    refreshCatalogue();
    refreshAccountStatus();
}

//void MainWindow::refreshAccountStatus() {
//    // Update status summary
//    QString status = QString("Borrowed: %1/3 items | Account: %2")
//        .arg(currentUser->borrowedItems.size())
//        .arg(currentUser->borrowedItems.size() == 0 ? "No fines" : "In good standing");
//    accountStatusLabel->setText(status);

//    // Update borrowed items list
//    borrowedItemsList->clear();
//    for (auto item : currentUser->borrowedItems) {
//        QString itemText = QString::fromStdString(item->getDisplayText());
//        itemText += " (Due in 14 days)";
//        borrowedItemsList->addItem(itemText);
//    }

//    // Update holds list
//    holdsList->clear();
//    for (auto item : currentUser->activeHolds) {
//        int position = item->getHoldPosition(currentUser->name);
//        QString holdText = QString::fromStdString(item->getDisplayText());
//        holdText += QString(" - Position #%1").arg(position);
//        holdsList->addItem(holdText);
//    }

//    onBookSelected();
//}

// database: try 1
//void MainWindow::refreshAccountStatus() {
//    // Update status summary
//    auto borrowedItems = DatabaseManager::getInstance().getUserBorrowedItems(currentUser->id);
//    auto userHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);

//    QString status = QString("Borrowed: %1/3 items | Active Holds: %2")
//        .arg(borrowedItems.size())
//        .arg(userHolds.size());
//    accountStatusLabel->setText(status);

//    // Update borrowed items list from DATABASE
//    borrowedItemsList->clear();
//    for (auto item : borrowedItems) {
//        QString itemText = QString::fromStdString(item->getDisplayText());
//        itemText += " (Due in 14 days)";
//        borrowedItemsList->addItem(itemText);

//        // Also update in-memory for current session
//        currentUser->borrowItem(item);
//    }

//    // Update holds list from DATABASE
//    holdsList->clear();
//    int position = 1;
//    for (auto item : userHolds) {
//        QString holdText = QString::fromStdString(item->getDisplayText());
//        holdText += QString(" - Position #%1").arg(position++);
//        holdsList->addItem(holdText);

//        // Also update in-memory for current session
//        currentUser->addHold(item);
//    }

//    onBookSelected();
//}


//void MainWindow::refreshAccountStatus() {
//    // Update status summary
//    auto borrowedItems = DatabaseManager::getInstance().getUserBorrowedItems(currentUser->id);
//    auto userHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);

//    QString status = QString("Borrowed: %1/3 items | Active Holds: %2")
//        .arg(borrowedItems.size())
//        .arg(userHolds.size());
//    accountStatusLabel->setText(status);

//    // Update borrowed items list from DATABASE
//    borrowedItemsList->clear();
//    for (auto item : borrowedItems) {
//        QString itemText = QString::fromStdString(item->getDisplayText());
//        itemText += " (Due in 14 days)";
//        borrowedItemsList->addItem(itemText);
//    }

//    // Update holds list from DATABASE - with REAL positions
//    holdsList->clear();
//    for (auto item : userHolds) {
//        int itemId = DatabaseManager::getInstance().getItemId(item);
//        int realPosition = DatabaseManager::getInstance().getHoldPosition(currentUser->id, itemId);

//        QString holdText = QString::fromStdString(item->getDisplayText());
//        holdText += QString(" - Position #%1").arg(realPosition);
//        holdsList->addItem(holdText);
//    }

//    onBookSelected();
//}

//database try 3:
void MainWindow::refreshAccountStatus() {
    // Update status summary
    auto borrowedItems = DatabaseManager::getInstance().getUserBorrowedItems(currentUser->id);
    auto userHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);

    QString status = QString("Borrowed: %1/3 items | Active Holds: %2")
        .arg(borrowedItems.size())
        .arg(userHolds.size());
    accountStatusLabel->setText(status);

    // Update borrowed items list from DATABASE
    borrowedItemsList->clear();

    // CRITICAL: Sync in-memory with database
    currentUser->borrowedItems.clear();
    for (auto item : borrowedItems) {
        QString itemText = QString::fromStdString(item->getDisplayText());
        itemText += " (Due in 14 days)";
        borrowedItemsList->addItem(itemText);

        // Keep in-memory in sync with database
        currentUser->borrowedItems.push_back(item);
    }

    // Update holds list from DATABASE - with REAL positions
    holdsList->clear();

    // CRITICAL: Sync in-memory holds with database
    currentUser->activeHolds.clear();
    for (auto item : userHolds) {
        int itemId = DatabaseManager::getInstance().getItemId(item);
        int realPosition = DatabaseManager::getInstance().getHoldPosition(currentUser->id, itemId);

        QString holdText = QString::fromStdString(item->getDisplayText());
        holdText += QString(" - Position #%1").arg(realPosition);
        holdsList->addItem(holdText);

        // Keep in-memory in sync with database
        currentUser->activeHolds.push_back(item);
    }

    onBookSelected();
}

//void MainWindow::placeHoldOnSelected() {
//    LibraryItem* selected = getSelectedBook();

//    if (!selected) {
//        QMessageBox::warning(this, "Error", "Please select a book first!");
//        return;
//    }

//    if (selected->getAvailability()) {
//        QMessageBox::information(this, "Info",
//            "This book is available! Just borrow it directly instead of placing a hold.");
//        return;
//    }

//    if (currentUser->hasHoldOn(selected)) {
//        QMessageBox::information(this, "Info", "You already have a hold on this book!");
//        return;
//    }

//    // Process hold placement
//    selected->addHold(currentUser->name);
//    currentUser->addHold(selected);

//    int position = selected->getHoldPosition(currentUser->name);

//    QMessageBox::information(this, "Hold Placed",
//        QString("You are now #%1 in line for: %2")
//        .arg(position)
//        .arg(QString::fromStdString(selected->getTitle())));

//    refreshCatalogue();
//    refreshAccountStatus();
//}

// database: try 1
//void MainWindow::placeHoldOnSelected() {
//    LibraryItem* selected = getSelectedBook();

//    if (!selected) {
//        QMessageBox::warning(this, "Error", "Please select a book first!");
//        return;
//    }

//    if (selected->getAvailability()) {
//        QMessageBox::information(this, "Info",
//            "This book is available! Just borrow it directly instead of placing a hold.");
//        return;
//    }

//    // Check if user already has a hold (using database)
//    auto userHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);
//    for (auto hold : userHolds) {
//        if (DatabaseManager::getInstance().getItemId(hold) == DatabaseManager::getInstance().getItemId(selected)) {
//            QMessageBox::information(this, "Info", "You already have a hold on this book!");
//            return;
//        }
//    }

//    // UPDATE DATABASE
//    int itemId = DatabaseManager::getInstance().getItemId(selected);
//    if (itemId == -1) {
//        QMessageBox::warning(this, "Error", "Could not find book in database!");
//        return;
//    }

//    bool success = DatabaseManager::getInstance().placeHold(currentUser->id, itemId);
//    if (!success) {
//        QMessageBox::warning(this, "Error", "Failed to place hold in database!");
//        return;
//    }

//    // Get the position from database
//    int position = 1;
//    auto updatedHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);
//    for (auto hold : updatedHolds) {
//        if (DatabaseManager::getInstance().getItemId(hold) == itemId) {
//            // We'd need to store position in the hold query to get the exact position
//            // For now, we'll use the order in the list
//            break;
//        }
//        position++;
//    }

//    QMessageBox::information(this, "Hold Placed",
//        QString("You are now #%1 in line for: %2")
//        .arg(position)
//        .arg(QString::fromStdString(selected->getTitle())));

//    refreshCatalogue();
//    refreshAccountStatus();
//}

//database: try 2
void MainWindow::placeHoldOnSelected() {
    LibraryItem* selected = getSelectedBook();

    if (!selected) {
        QMessageBox::warning(this, "Error", "Please select a book first!");
        return;
    }

    if (selected->getAvailability()) {
        QMessageBox::information(this, "Info",
            "This book is available! Just borrow it directly instead of placing a hold.");
        return;
    }

    // Check if user already has a hold (using database)
    auto userHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);
    for (auto hold : userHolds) {
        if (DatabaseManager::getInstance().getItemId(hold) == DatabaseManager::getInstance().getItemId(selected)) {
            QMessageBox::information(this, "Info", "You already have a hold on this book!");
            return;
        }
    }

    // UPDATE DATABASE
    int itemId = DatabaseManager::getInstance().getItemId(selected);
    if (itemId == -1) {
        QMessageBox::warning(this, "Error", "Could not find book in database!");
        return;
    }

    // Get the CURRENT hold count BEFORE placing the new hold (this will be their position)
    int currentHoldCount = DatabaseManager::getInstance().getHoldCountForItem(itemId);
    int userPosition = currentHoldCount + 1;

    bool success = DatabaseManager::getInstance().placeHold(currentUser->id, itemId);
    if (!success) {
        QMessageBox::warning(this, "Error", "Failed to place hold in database!");
        return;
    }

    QMessageBox::information(this, "Hold Placed",
        QString("You are now #%1 in line for: %2")
        .arg(userPosition)
        .arg(QString::fromStdString(selected->getTitle())));

    refreshCatalogue();
    refreshAccountStatus();
}

//void MainWindow::cancelSelectedHold() {
//    int currentRow = holdsList->currentRow();
//    if (currentRow < 0 || currentRow >= currentUser->activeHolds.size()) {
//        QMessageBox::warning(this, "Error", "Please select a hold to cancel!");
//        return;
//    }

//    LibraryItem* holdItem = currentUser->activeHolds[currentRow];

//    // Remove hold from system
//    currentUser->removeHold(holdItem);
//    holdItem->removeHold(currentUser->name);

//    QMessageBox::information(this, "Hold Cancelled",
//        QString("Hold removed for: %1")
//        .arg(QString::fromStdString(holdItem->getTitle())));

//    refreshCatalogue();
//    refreshAccountStatus();
//}

//using database: try 1
void MainWindow::cancelSelectedHold() {
    int currentRow = holdsList->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Error", "Please select a hold to cancel!");
        return;
    }

    // Get the hold from database instead of in-memory
    auto userHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);
    if (currentRow >= userHolds.size()) {
        QMessageBox::warning(this, "Error", "Invalid hold selection!");
        return;
    }

    LibraryItem* holdItem = userHolds[currentRow];
    int itemId = DatabaseManager::getInstance().getItemId(holdItem);

    if (itemId == -1) {
        QMessageBox::warning(this, "Error", "Could not find hold in database!");
        return;
    }

    // UPDATE DATABASE
    bool success = DatabaseManager::getInstance().cancelHold(currentUser->id, itemId);
    if (!success) {
        QMessageBox::warning(this, "Error", "Failed to cancel hold in database!");
        return;
    }

    QMessageBox::information(this, "Hold Cancelled",
        QString("Hold removed for: %1")
        .arg(QString::fromStdString(holdItem->getTitle())));

    refreshCatalogue();
    refreshAccountStatus();
}

//void MainWindow::updateHoldButtons() {
//    // Update cancel hold button
//    bool holdSelected = (holdsList->currentRow() >= 0 &&
//                        holdsList->currentRow() < currentUser->activeHolds.size());
//    cancelHoldButton->setEnabled(holdSelected);

//    // Update place hold button
//    LibraryItem* selectedBook = getSelectedBook();
//    bool canPlaceHold = selectedBook &&
//                       !selectedBook->getAvailability() &&
//                       !currentUser->hasHoldOn(selectedBook);
//    holdButton->setEnabled(canPlaceHold);
//}

//using database : try 1
void MainWindow::updateHoldButtons() {
    // Update cancel hold button using DATABASE
    auto userHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);
    bool holdSelected = (holdsList->currentRow() >= 0 && holdsList->currentRow() < userHolds.size());
    cancelHoldButton->setEnabled(holdSelected);

    // Update place hold button using DATABASE
    LibraryItem* selectedBook = getSelectedBook();
    if (selectedBook) {
        int itemId = DatabaseManager::getInstance().getItemId(selectedBook);
        bool userHasHold = false;

        // Check if user already has a hold on this book
        for (auto hold : userHolds) {
            if (DatabaseManager::getInstance().getItemId(hold) == itemId) {
                userHasHold = true;
                break;
            }
        }

        bool canPlaceHold = !selectedBook->getAvailability() && !userHasHold;
        holdButton->setEnabled(canPlaceHold);
    } else {
        holdButton->setEnabled(false);
    }
}

LibraryItem* MainWindow::getSelectedBorrowedItem() {
    int currentRow = borrowedItemsList->currentRow();
    if (currentRow >= 0 && currentRow < currentUser->borrowedItems.size()) {
        return currentUser->borrowedItems[currentRow];
    }
    return nullptr;
}

LibraryItem* MainWindow::getSelectedBook() {
    int currentRow = bookListWidget->currentRow();
    if (currentRow >= 0) {
//        auto& catalogue = DataManager::getInstance().getCatalogue(); // we don't want memory
        auto catalogue = DatabaseManager::getInstance().getAllCatalogueItems();

        if (currentRow < catalogue.size()) {
            return catalogue[currentRow];
        }
    }
    return nullptr;
}


void MainWindow::debugCheckDatabase() {
    qDebug() << "=== DEBUG: Current User ===";
    qDebug() << "User:" << QString::fromStdString(currentUser->name) << "ID:" << currentUser->id;

    qDebug() << "=== DEBUG: Database Catalogue ===";
    auto dbCatalogue = DatabaseManager::getInstance().getAllCatalogueItems();
    for (auto item : dbCatalogue) {
        qDebug() << "DB Item:" << QString::fromStdString(item->getTitle())
                 << "Available:" << item->getAvailability();
    }

    qDebug() << "=== DEBUG: In-Memory User State ===";
    qDebug() << "Borrowed items count:" << currentUser->borrowedItems.size();
    for (auto item : currentUser->borrowedItems) {
        qDebug() << "Borrowed:" << QString::fromStdString(item->getTitle());
    }
    qDebug() << "=== END DEBUG ===";
}
