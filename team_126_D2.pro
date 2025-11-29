QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AddItemDialog.cpp \
    DatabaseInitializer.cpp \
    DatabaseManager.cpp \
    LoginDialog.cpp \
    MainWindow.cpp \
    PatronReturnDialog.cpp \
    PatronSelectionDialog.cpp \
    main.cpp

HEADERS += \
    AddItemDialog.h \
    DatabaseInitializer.h \
    DatabaseManager.h \
    LibraryItem.h \
    LoginDialog.h \
    MainWindow.h \
    PatronReturnDialog.h \
    PatronSelectionDialog.h \
    User.h

#FORMS += MainWindow.ui   #Note: The UI was built programmatically (in MainWindow.cpp) rather than via Designer for better control over dynamic content and role-based interface changes

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
