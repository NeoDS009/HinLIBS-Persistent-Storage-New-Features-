#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "DatabaseManager.h"


/*
    LoginDialog Class:
    Handles user authentication and role-based system entry for the HinLIBS library system.
    Provides a secure entry point that validates users against the system database and
    determines appropriate access levels based on user roles.

    Key Responsibilities:
    - Present login interface to users
    - Validate credentials against User database
    - Determine user role (Patron, Librarian, Administrator)
    - Emit authentication signal for main application entry
    - Handle authentication errors with user feedback

    UI Components:
    - usernameInput: Text field for user identifier entry
    - loginButton: Primary action button to initiate authentication
    - errorLabel: Dynamic display for authentication feedback

    Data Members:
    - usernameInput: QLineEdit for capturing username input
    - loginButton: QPushButton to trigger authentication attempt
    - errorLabel: QLabel for displaying error messages
    - lastAuthenticatedUser: User* storing successfully authenticated user

    Member Functions:
    - LoginDialog(): Constructs and initializes the login interface
    - getLoggedInUser(): Provides access to authenticated user data
    - attemptLogin(): Validates credentials and processes authentication

    Signals:
    - userAuthenticated(): Emitted upon successful authentication with user data
*/
class LoginDialog : public QDialog {
    Q_OBJECT

public:

    /*
        Function: LoginDialog
        Purpose: Constructs the login dialog interface with all UI components
        Parameters:
            in: QWidget* parent - Parent widget for modal behavior
        Return: LoginDialog instance
    */
    LoginDialog(QWidget *parent = nullptr);

    /*
        Function: getLoggedInUser
        Purpose: Provides access to the most recently authenticated user object
        Parameters: None
        Return: User* - Pointer to authenticated user, or nullptr if no successful login
        Notes: Returns the actual User object from DatabaseManager for role-based access control
    */
    User* getLoggedInUser() { return lastAuthenticatedUser; }


signals:

    /*
        Signal: userAuthenticated
        Purpose: Notifies connected components of successful user authentication
        Parameters:
            out: User* user - Pointer to the successfully authenticated user object
        Emitted When: User credentials are validated against system records
    */
    void userAuthenticated(User* user);


private slots:

    /*
        Function: attemptLogin
        Purpose: Validates entered credentials and processes authentication logic
    */
    void attemptLogin();

private:
    QLineEdit *usernameInput;
    QPushButton *loginButton;
    QLabel *errorLabel;
    User* lastAuthenticatedUser;
};

#endif
