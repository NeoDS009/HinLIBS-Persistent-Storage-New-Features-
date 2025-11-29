#include "PatronReturnDialog.h"
#include "DatabaseManager.h"


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
    // Clean up loan items
    for (auto& loan : patronLoans) {
        delete loan.item;
    }
}

void PatronReturnDialog::loadBorrowedItems() {
    patronLoans = DatabaseManager::getInstance().getUserLoansWithDates(currentPatron->id);

    for (const auto& loan : patronLoans) {
        QString displayText = QString::fromStdString(loan.item->getDisplayText());

        // ADD DATES TO DISPLAY
        displayText += QString("\n  Checked out: %1 | Due: %2")
                      .arg(loan.checkoutDate)
                      .arg(loan.dueDate);

        itemsList->addItem(displayText);
    }

    if (patronLoans.empty()) {
        itemsList->addItem("No borrowed items found");
        itemsList->setEnabled(false);
    }
}

LibraryItem* PatronReturnDialog::getSelectedItem() const {
    int currentRow = itemsList->currentRow();
    if (currentRow >= 0 && currentRow < patronLoans.size()) {
        return patronLoans[currentRow].item;
    }
    return nullptr;
}
