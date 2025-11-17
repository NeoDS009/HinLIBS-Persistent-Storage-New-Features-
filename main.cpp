#include <QApplication>
#include "LoginDialog.h"
#include "MainWindow.h"
#include "DatabaseManager.h"
#include "DatabaseInitializer.h"  // ADD THIS LINE
#include "QDir"
#include "QFile"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qDebug() << "Current directory:" << QDir::currentPath();
    qDebug() << "Database file exists:" << QFile::exists("hinlibs.db");

    // Initialize database
    DatabaseInitializer::initializeDatabase("hinlibs.db");

    // DEBUG: Check what users are in the database
    DatabaseManager::getInstance().debugPrintAllUsers();

    while (true) {
        LoginDialog loginDialog;

        // Attempt user authentication
        if (loginDialog.exec() == QDialog::Accepted) {
            User* loggedInUser = loginDialog.getLoggedInUser();

            // Launch main application window with authenticated user
            MainWindow mainWindow(loggedInUser);
            mainWindow.show();
            app.exec();


            // main loop will run until Login Dialog gets closed: to support mutliple user per session
        } else {
            break;
        }
    }

    return 0;
}
