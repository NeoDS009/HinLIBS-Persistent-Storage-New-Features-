#include <QMessageBox>
#include <QApplication>
#include <QCloseEvent>
#include "MainWindow.h"
#include "PatronSelectionDialog.h"
#include "PatronReturnDialog.h"

MainWindow::MainWindow(User* user, QWidget *parent)
    : QMainWindow(parent), currentUser(user) {

    setWindowTitle("HinLIBS - Hintonville Library System");
    setFixedSize(1200, 800);

    setupUI();
    refreshCatalogue();
    refreshAccountStatus();
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

    setupLibrarianUI(); // Add librarian tools if applicable
}

void MainWindow::setupLibrarianUI() {
    // Only show librarian features for librarians/admins
    if (currentUser->role != "librarian" && currentUser->role != "admin") {
        return;
    }

    // Create librarian panel with professional styling
    librarianPanel = new QWidget();
    QVBoxLayout* librarianLayout = new QVBoxLayout(librarianPanel);

    librarianLayout->setContentsMargins(10, 10, 10, 10);
    librarianLayout->setSpacing(15);

    QLabel* librarianLabel = new QLabel("Librarian Tools");
    librarianLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #2c3e50;");
    librarianLabel->setAlignment(Qt::AlignCenter);
    librarianLayout->addWidget(librarianLabel);

    // Separator line
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("color: #bdc3c7;");
    librarianLayout->addWidget(line);

    // Librarian buttons
    addItemButton = new QPushButton("Add New Item to Catalogue");
    removeItemButton = new QPushButton("Remove Selected Item");
    returnForPatronButton = new QPushButton("Return Item for Patron");

    QString buttonStyle = "QPushButton { padding: 8px; }";
    addItemButton->setStyleSheet(buttonStyle);
    removeItemButton->setStyleSheet(buttonStyle);
    returnForPatronButton->setStyleSheet(buttonStyle);

    librarianLayout->addWidget(addItemButton);
    librarianLayout->addWidget(removeItemButton);
    librarianLayout->addWidget(returnForPatronButton);

    librarianLayout->addStretch(); // Push content to top

    // Add to main layout
    QHBoxLayout* mainLayout = qobject_cast<QHBoxLayout*>(centralWidget()->layout());
    if (mainLayout) {
        mainLayout->addWidget(librarianPanel, 1);
    }

    // Connect librarian buttons
    connect(addItemButton, &QPushButton::clicked, this, &MainWindow::showAddItemDialog);
    connect(removeItemButton, &QPushButton::clicked, this, &MainWindow::removeSelectedItem);
    connect(returnForPatronButton, &QPushButton::clicked, this, &MainWindow::showReturnForPatronDialog);
}



// === LIBRARIAN FEATURES ===

void MainWindow::showAddItemDialog() {
    AddItemDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        bool success = DatabaseManager::getInstance().addItemToCatalogue(
            dialog.getTitle(), dialog.getAuthor(), dialog.getItemType(),
            dialog.getDeweyDecimal(), dialog.getISBN(), dialog.getGenre(),
            dialog.getRating(), dialog.getIssueNumber(), dialog.getPublicationDate(),
            dialog.getPublicationYear(), dialog.getCondition()
        );

        if (success) {
            QMessageBox::information(this, "Success", "Item added to catalogue successfully!");
            refreshCatalogue();
        } else {
            QMessageBox::warning(this, "Error", "Failed to add item to catalogue.");
        }
    }
}

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

    // Safety confirmation before deletion
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Removal",
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
    // 3-step process: Select patron → Select item → Process return
    PatronSelectionDialog patronDialog(this);
    if (patronDialog.exec() == QDialog::Accepted) {
        User* selectedPatron = patronDialog.getSelectedPatron();
        if (selectedPatron) {
            PatronReturnDialog returnDialog(selectedPatron, this);
            if (returnDialog.exec() == QDialog::Accepted) {
                LibraryItem* selectedItem = returnDialog.getSelectedItem();
                if (selectedItem) {
                    int itemId = DatabaseManager::getInstance().getItemId(selectedItem);
                    processPatronReturn(selectedPatron->id, itemId);
                }
            }
        }
    }
}

void MainWindow::processPatronReturn(int patronId, int itemId) {
    bool success = DatabaseManager::getInstance().returnItem(patronId, itemId);
    if (success) {
        // Find patron name for success message
        auto allUsers = DatabaseManager::getInstance().getAllUsers();
        QString patronName;
        for (auto user : allUsers) {
            if (user->id == patronId) {
                patronName = QString::fromStdString(user->name);
            }
            delete user; // Clean up memory
        }

        QMessageBox::information(this, "Success",
            QString("Successfully returned item for patron: %1").arg(patronName));
        refreshCatalogue();
    } else {
        QMessageBox::warning(this, "Error", "Failed to return item.");
    }
}



// === CORE LIBRARY OPERATIONS ===

void MainWindow::refreshCatalogue() {
    // Preserve selection across refresh for better UX
    QString previouslySelectedTitle;
    LibraryItem* selected = getSelectedBook();
    if (selected) {
        previouslySelectedTitle = QString::fromStdString(selected->getTitle());
    }

    bookListWidget->clear();
    auto catalogue = DatabaseManager::getInstance().getAllCatalogueItems();

    for (auto item : catalogue) {
        QString displayText = QString::fromStdString(item->getDisplayText());

        // Get real-time availability and hold counts from database
        int itemId = DatabaseManager::getInstance().getItemId(item);
        int holdCount = DatabaseManager::getInstance().getHoldCountForItem(itemId);

        if (item->getAvailability()) {
            displayText += " [AVAILABLE]";
        } else {
            displayText += " [CHECKED OUT]";
        }

        if (holdCount > 0) {
            displayText += QString(" (%1 holds)").arg(holdCount);
        }

        QListWidgetItem* listItem = new QListWidgetItem(displayText);

        // Visual status indicators
        if (!item->getAvailability()) {
            listItem->setBackground(QBrush(QColor(255, 200, 200)));
        }

        bookListWidget->addItem(listItem);

        // Restore previous selection if possible
        if (!previouslySelectedTitle.isEmpty() &&
            previouslySelectedTitle == QString::fromStdString(item->getTitle())) {
            listItem->setSelected(true);
        }
    }

    onBookSelected();
    refreshAccountStatus();
}

void MainWindow::refreshAccountStatus() {
    // Critical: Sync in-memory state with database to prevent state mismatches
    auto borrowedItems = DatabaseManager::getInstance().getUserBorrowedItems(currentUser->id);
    auto userHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);

    QString status = QString("Borrowed: %1/3 items | Active Holds: %2")
        .arg(borrowedItems.size()).arg(userHolds.size());
    accountStatusLabel->setText(status);

    // Update borrowed items list
    borrowedItemsList->clear();
    currentUser->borrowedItems.clear(); // Clear before sync
    for (auto item : borrowedItems) {
        QString itemText = QString::fromStdString(item->getDisplayText()) + " (Due in 14 days)";
        borrowedItemsList->addItem(itemText);
        currentUser->borrowedItems.push_back(item); // Sync in-memory state
    }

    // Update holds list with real positions from database
    holdsList->clear();
    currentUser->activeHolds.clear(); // Clear before sync
    for (auto item : userHolds) {
        int itemId = DatabaseManager::getInstance().getItemId(item);
        int realPosition = DatabaseManager::getInstance().getHoldPosition(currentUser->id, itemId);

        QString holdText = QString::fromStdString(item->getDisplayText()) +
                          QString(" - Position #%1").arg(realPosition);
        holdsList->addItem(holdText);
        currentUser->activeHolds.push_back(item); // Sync in-memory state
    }

    onBookSelected();
}

void MainWindow::borrowSelectedBook() {
    LibraryItem* selected = getSelectedBook();
    if (!selected) return;

    // Business rule validation
    if (!selected->getAvailability()) {
        QMessageBox::warning(this, "Error", "This book is already checked out!");
        return;
    }
    if (!currentUser->canBorrow()) {
        QMessageBox::warning(this, "Error",
            "You have reached the maximum of 3 borrowed books. Please return one first!");
        return;
    }

    // Database operation
    int itemId = DatabaseManager::getInstance().getItemId(selected);
    if (itemId == -1) return;

    bool success = DatabaseManager::getInstance().borrowItem(currentUser->id, itemId);
    if (!success) {
        QMessageBox::warning(this, "Error", "Failed to borrow book in database!");
        return;
    }

    // Update in-memory state for current session
    selected->setAvailable(false);
    currentUser->borrowItem(selected);

    QMessageBox::information(this, "Success",
        QString("You have successfully borrowed: %1").arg(QString::fromStdString(selected->getTitle())));

    refreshCatalogue();
    refreshAccountStatus();
}

void MainWindow::returnSelectedBook() {
    LibraryItem* selected = getSelectedBorrowedItem();
    if (!selected) return;

    int itemId = DatabaseManager::getInstance().getItemId(selected);
    if (itemId == -1) return;

    bool success = DatabaseManager::getInstance().returnItem(currentUser->id, itemId);
    if (!success) {
        QMessageBox::warning(this, "Error", "Failed to return book in database!");
        return;
    }

    // Update in-memory state
    selected->setAvailable(true);
    currentUser->returnItem(selected);

    QMessageBox::information(this, "Success",
        QString("You have successfully returned: %1").arg(QString::fromStdString(selected->getTitle())));

    refreshCatalogue();
    refreshAccountStatus();
}

void MainWindow::placeHoldOnSelected() {
    LibraryItem* selected = getSelectedBook();
    if (!selected) return;

    if (selected->getAvailability()) {
        QMessageBox::information(this, "Info",
            "This book is available! Just borrow it directly instead of placing a hold.");
        return;
    }

    // Check for duplicate holds
    auto userHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);
    for (auto hold : userHolds) {
        if (DatabaseManager::getInstance().getItemId(hold) == DatabaseManager::getInstance().getItemId(selected)) {
            QMessageBox::information(this, "Info", "You already have a hold on this book!");
            return;
        }
    }

    int itemId = DatabaseManager::getInstance().getItemId(selected);
    if (itemId == -1) return;

    // Calculate position before placing hold
    int currentHoldCount = DatabaseManager::getInstance().getHoldCountForItem(itemId);
    int userPosition = currentHoldCount + 1;

    bool success = DatabaseManager::getInstance().placeHold(currentUser->id, itemId);
    if (!success) return;

    QMessageBox::information(this, "Hold Placed",
        QString("You are now #%1 in line for: %2").arg(userPosition).arg(QString::fromStdString(selected->getTitle())));

    refreshCatalogue();
    refreshAccountStatus();
}

void MainWindow::cancelSelectedHold() {
    int currentRow = holdsList->currentRow();
    if (currentRow < 0) return;

    auto userHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);
    if (currentRow >= userHolds.size()) return;

    LibraryItem* holdItem = userHolds[currentRow];
    int itemId = DatabaseManager::getInstance().getItemId(holdItem);
    if (itemId == -1) return;

    bool success = DatabaseManager::getInstance().cancelHold(currentUser->id, itemId);
    if (!success) return;

    QMessageBox::information(this, "Hold Cancelled",
        QString("Hold removed for: %1").arg(QString::fromStdString(holdItem->getTitle())));

    refreshCatalogue();
    refreshAccountStatus();
}



// === UI STATE MANAGEMENT ===

void MainWindow::onBookSelected() {
    LibraryItem* selectedBook = getSelectedBook();
    LibraryItem* selectedBorrowed = getSelectedBorrowedItem();

    borrowButton->setEnabled(selectedBook && selectedBook->getAvailability() && currentUser->canBorrow());
    returnButton->setEnabled(selectedBorrowed != nullptr);
    updateHoldButtons();
}

void MainWindow::updateHoldButtons() {
    // Update cancel hold button state
    auto userHolds = DatabaseManager::getInstance().getUserHolds(currentUser->id);
    bool holdSelected = (holdsList->currentRow() >= 0 && holdsList->currentRow() < userHolds.size());
    cancelHoldButton->setEnabled(holdSelected);

    // Update place hold button state
    LibraryItem* selectedBook = getSelectedBook();
    if (selectedBook) {
        int itemId = DatabaseManager::getInstance().getItemId(selectedBook);
        bool userHasHold = false;

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



// === UTILITY METHODS ===

void MainWindow::showItemDetails() {
    LibraryItem* item = getSelectedBook();
    if (!item) item = getSelectedBorrowedItem();
    if (!item) return;

    QString details = QString("Title: %1\nAuthor: %2\nFormat: %3\n%4")
        .arg(QString::fromStdString(item->getTitle()))
        .arg(QString::fromStdString(item->getAuthor()))
        .arg(QString::fromStdString(item->getFormat()))
        .arg(QString::fromStdString(item->getDetailedInfo()));

    QMessageBox::information(this, "Item Details", details);
}

void MainWindow::logout() {
    this->close();
}

LibraryItem* MainWindow::getSelectedBook() {
    int currentRow = bookListWidget->currentRow();
    if (currentRow >= 0) {
        auto catalogue = DatabaseManager::getInstance().getAllCatalogueItems();
        if (currentRow < catalogue.size()) {
            return catalogue[currentRow];
        }
    }
    return nullptr;
}

LibraryItem* MainWindow::getSelectedBorrowedItem() {
    int currentRow = borrowedItemsList->currentRow();
    if (currentRow >= 0 && currentRow < currentUser->borrowedItems.size()) {
        return currentUser->borrowedItems[currentRow];
    }
    return nullptr;
}
