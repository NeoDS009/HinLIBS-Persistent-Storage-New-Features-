#ifndef IDATAREPOSITORY_H
#define IDATAREPOSITORY_H

#include "User.h"
#include "LibraryItem.h"
#include <vector>

class IDataRepository {
public:
    virtual ~IDataRepository() = default;

    // Core methods your DataManager already has
    virtual User* findUser(const std::string& username) = 0;
    virtual std::vector<LibraryItem*>& getCatalogue() = 0;

    // Methods we'll add for librarian features
    virtual bool addItemToCatalogue(LibraryItem* item) = 0;
    virtual bool removeItemFromCatalogue(int itemId) = 0;
    virtual std::vector<User*> getAllUsers() = 0;
};

#endif
