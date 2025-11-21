#include "PatronReturnDialog.h"
#include "DatabaseManager.h"
#include <QDebug>

PatronReturnDialog::PatronReturnDialog(User* patron, QWidget *parent)
    : QDialog(parent), currentPatron(patron) {

    setWindowTitle(QString("Return Item for %1").arg(QString::fromStdString(patron->name)));
    setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel(QString("Select item to return for %1:").arg(QString::fromStdString(patron->name)));
    layout->addWidget(label);

    itemsList = new QListWidget();
    layout->addWidget(itemsList);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    loadBorrowedItems();
}

PatronReturnDialog::~PatronReturnDialog() {
    // Clean up borrowed items
    for (auto item : borrowedItems) {
        delete item;
    }
}

void PatronReturnDialog::loadBorrowedItems() {
    borrowedItems = DatabaseManager::getInstance().getUserBorrowedItems(currentPatron->id);

    for (auto item : borrowedItems) {
        QString displayText = QString::fromStdString(item->getDisplayText());
        itemsList->addItem(displayText);
    }

    if (borrowedItems.empty()) {
        itemsList->addItem("No borrowed items found");
        itemsList->setEnabled(false);
    }
}

//void PatronReturnDialog::loadBorrowedItems() {
//    auto itemsVector = DatabaseManager::getInstance().getUserBorrowedItems(currentPatron->id);

//    // Convert std::vector to QList
//    borrowedItems = QList<LibraryItem*>(itemsVector.begin(), itemsVector.end());

//    for (auto item : borrowedItems) {
//        QString displayText = QString::fromStdString(item->getDisplayText());
//        itemsList->addItem(displayText);
//    }

//    if (borrowedItems.empty()) {
//        itemsList->addItem("No borrowed items found");
//        itemsList->setEnabled(false);
//    }
//}

LibraryItem* PatronReturnDialog::getSelectedItem() const {
    int currentRow = itemsList->currentRow();
    if (currentRow >= 0 && currentRow < borrowedItems.size()) {
        return borrowedItems[currentRow];
    }
    return nullptr;
}
