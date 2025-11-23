#ifndef PATRONSELECTIONDIALOG_H
#define PATRONSELECTIONDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include "User.h"

/*
    PatronSelectionDialog Class:
    A modal dialog that allows librarians to select a patron from the complete list
    of library users. This dialog is the first step in the "Return Item for Patron"
    workflow, enabling librarians to process returns on behalf of specific patrons.

    Key Responsibilities:
    - Display all registered patrons in a searchable/list format
    - Provide clean selection interface for librarian workflows
    - Return selected patron object for further processing
    - Filter and display only patron accounts (excludes librarians/admins)

    UI Design:
    - Simple list-based selection interface
    - Clear labeling and intuitive selection mechanism
    - Standard dialog buttons (OK/Cancel) for user confirmation

    Data Members:
      - QListWidget* patronList: Visual list displaying all patron accounts
      - QList<User*> allPatrons: Internal collection of patron user objects

    Member Functions:
      Public:
      - PatronSelectionDialog(): Constructs and initializes the dialog
      - getSelectedPatron(): Returns the user-selected patron object

      Private:
        - loadPatrons(): Populates the list with patron accounts from database

    Integration Points:
      - Used by MainWindow::showReturnForPatronDialog() as first step in patron return flow
      - Interfaces with DatabaseManager to retrieve patron accounts
      - Returns selected patron to calling code for further processing
*/
class PatronSelectionDialog : public QDialog {
    Q_OBJECT

public:
    /*
        Function: PatronSelectionDialog
        Purpose: Constructs and initializes the patron selection dialog. Automatically
                 loads all patron accounts from the database and populates the selection
                 list upon construction.
        Parameters:
          in: QWidget* parent - Parent widget for modal behavior (optional)
        Return: Fully initialized PatronSelectionDialog instance
    */
    PatronSelectionDialog(QWidget *parent = nullptr);

    /*
        Function: getSelectedPatron
        Purpose: Retrieves the patron selected by the user in the dialog. Returns the
                 complete User object for the selected patron, which can be used for
                 subsequent operations like viewing borrowed items or processing returns.
        Return: User* - Pointer to the selected patron user object, or nullptr if no
                 selection was made or dialog was cancelled
    */
    User* getSelectedPatron() const;

private:
    QListWidget *patronList;    // Visual list widget displaying patron accounts
    QList<User*> allPatrons;    // Internal collection of patron user objects

    /*
        Function: loadPatrons
        Purpose: Internal method that queries the database for all user accounts and
                 filters them to display only patrons (excluding librarians and admins).
                 Populates both the visual list and internal collection with appropriate
                 display formatting that includes patron names and IDs.
    */
    void loadPatrons();
};

#endif
