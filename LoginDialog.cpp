#include "LoginDialog.h"
#include "DataManager.h"
#include "DatabaseManager.h" //new: testing
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("HinLIBS Login");
    setFixedSize(400, 250);

    // Create login interface
    QLabel *titleLabel = new QLabel("HinLIBS Login");
    titleLabel->setAlignment(Qt::AlignCenter);

    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Enter your username");

    loginButton = new QPushButton("Login");
    errorLabel = new QLabel();
    errorLabel->setStyleSheet("color: red;");
    errorLabel->setVisible(false);

    // Layout setup
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(titleLabel);
    layout->addWidget(new QLabel("Username:"));
    layout->addWidget(usernameInput);
    layout->addWidget(loginButton);
    layout->addWidget(errorLabel);

    setLayout(layout);

    // Connect authentication triggers
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::attemptLogin);
    connect(usernameInput, &QLineEdit::returnPressed, this, &LoginDialog::attemptLogin);
}

void LoginDialog::attemptLogin() {
    QString username = usernameInput->text().trimmed();

    if (username.isEmpty()) {
        errorLabel->setText("Please enter a username");
        errorLabel->setVisible(true);
        return;
    }

    // Authenticate against DatabaseManager
    User* user = DatabaseManager::getInstance().findUser(username);


    if (user) {
        errorLabel->setVisible(false);
        lastAuthenticatedUser = user;
        emit userAuthenticated(user);
        accept();
    } else {
        errorLabel->setText("User not found. Try: alice_p, libby, or admin");
        errorLabel->setVisible(true);
    }
}
