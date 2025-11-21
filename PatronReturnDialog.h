#ifndef PATRONRETURNDIALOG_H
#define PATRONRETURNDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include "User.h"
#include "LibraryItem.h"

class PatronReturnDialog : public QDialog {
    Q_OBJECT

public:
    PatronReturnDialog(User* patron, QWidget *parent = nullptr);
    LibraryItem* getSelectedItem() const;
    ~PatronReturnDialog();  // new

private:
    User* currentPatron;
    QListWidget *itemsList;
//    QList<LibraryItem*> borrowedItems;  //first: error
    std::vector<LibraryItem*> borrowedItems;

    void loadBorrowedItems();
};

#endif
