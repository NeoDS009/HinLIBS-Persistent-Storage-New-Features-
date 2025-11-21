#include "PatronSelectionDialog.h"
#include "DatabaseManager.h"
#include <QDebug>

PatronSelectionDialog::PatronSelectionDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Select Patron");
    setFixedSize(300, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Select a patron:");
    layout->addWidget(label);

    patronList = new QListWidget();
    layout->addWidget(patronList);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    loadPatrons();
}

void PatronSelectionDialog::loadPatrons() {
    // Get all users and filter for patrons
    auto allUsers = DatabaseManager::getInstance().getAllUsers();

    for (auto user : allUsers) {
        if (user->role == "patron") {
            allPatrons.push_back(user);
            QString displayText = QString("%1 (ID: %2)").arg(QString::fromStdString(user->name)).arg(user->id);
            patronList->addItem(displayText);
        } else {
            delete user; // Clean up non-patrons
        }
    }
}

User* PatronSelectionDialog::getSelectedPatron() const {
    int currentRow = patronList->currentRow();
    if (currentRow >= 0 && currentRow < allPatrons.size()) {
        return allPatrons[currentRow];
    }
    return nullptr;
}
