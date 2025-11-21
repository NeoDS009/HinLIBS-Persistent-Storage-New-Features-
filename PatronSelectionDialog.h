#ifndef PATRONSELECTIONDIALOG_H
#define PATRONSELECTIONDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include "User.h"

class PatronSelectionDialog : public QDialog {
    Q_OBJECT

public:
    PatronSelectionDialog(QWidget *parent = nullptr);
    User* getSelectedPatron() const;

private:
    QListWidget *patronList;
    QList<User*> allPatrons;

    void loadPatrons();
};

#endif
