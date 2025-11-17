//#ifndef DATAMANAGER_H
//#define DATAMANAGER_H

//#include <vector>
//#include "User.h"
//#include "LibraryItem.h"

///*
//    DataManager Class:
//    Singleton class that serves as the central data repository for the HinLIBS library system.
//    Maintains all catalogue items and user accounts in memory during application runtime.
//    Implements the Singleton pattern to ensure consistent data state across all system components.

//    Design Pattern: Singleton - ensures only one instance exists application-wide

//    Data Members:
//      - static DataManager* instance: Pointer to the singleton instance
//      - vector<User> users: Collection of all system users with their accounts
//      - vector<LibraryItem*> catalogue: Collection of all library items in the system

//    Member Functions:
//      - getInstance(): Provides global access to the singleton instance
//      - findUser(): Locates and authenticates users by username
//      - getCatalogue(): Provides access to the complete item collection
//      - resetAllData(): Restores system to initial state (for testing/demo)
//      - ~DataManager(): Cleans up dynamically allocated library items

//    Private Member Functions:
//      - initializeUsers(): Populates the system with default user accounts
//      - initializeCatalogue(): Creates the initial library inventory

//    Private Constructor: Enforced to maintain singleton pattern integrity
//*/

//class DataManager{
//private:
//    static DataManager* instance;
//    vector<User> users;
//    vector<LibraryItem*> catalogue;

//    DataManager() {
//        initializeUsers();
//        initializeCatalogue();
//    }

//public:
//    /*
//        Function: getInstance
//        Purpose: Provides global access point to the singleton DataManager instance
//        Return: DataManager& - Reference to the singleton instance
//    */
//    static DataManager& getInstance() {
//        static DataManager instance;
//        return instance;
//    }

//    // Delete copy constructor and assignment operator to enforce singleton
//    DataManager(const DataManager&) = delete;
//    DataManager& operator=(const DataManager&) = delete;

//    /*
//        Function: ~DataManager
//        Purpose: Destructor that cleans up dynamically allocated LibraryItem objects. Frees all memory allocated for catalogue items
//    */
//    ~DataManager() {
//        for (auto item : catalogue) {
//            delete item;
//        }
//    }

//    /*
//        Function: findUser
//        Purpose: Locates a user by username for authentication and role-based access. Performs linear search through users collection
//        Parameters:
//            in: string name - The username to search for
//        Return: User* - Pointer to User object if found, nullptr otherwise
//    */
//    User* findUser(string name) {
//        for (auto& user : users) {
//            if (user.name == name) {
//                return &user;
//            }
//        }
//        return nullptr;
//    }

//    /*
//        Function: getCatalogue
//        Purpose: Provides access to the complete library item collection. Return by reference allows direct modification of catalogue items
//        Return: vector<LibraryItem*>& - Reference to the catalogue vector
//    */
//    vector<LibraryItem*>& getCatalogue() {
//        return catalogue;
//    }

//    /*
//        Function: resetAllData
//        Purpose: Restores the system to its initial state by clearing all dynamic data, including catalogue items, borrowed items, active holds, and user accounts
//    */
//    void resetAllData() {
//        // Clear existing catalogue
//        for (auto item : catalogue) {
//            delete item;
//        }
//        catalogue.clear();

//        initializeCatalogue();

//        // Reset users' borrowed items and holds
//        for (auto& user : users) {
//            user.borrowedItems.clear();
//            user.activeHolds.clear();
//        }
//    }

//private:
//    /*
//        Function: initializeUsers
//        Purpose: Populates the system with default user accounts for testing/demo
//    */
//    void initializeUsers() {
//        users.clear();
//        users.push_back(User("alice_p", "patron"));
//        users.push_back(User("bob_p", "patron"));
//        users.push_back(User("charlie_p", "patron"));
//        users.push_back(User("diana_p", "patron"));
//        users.push_back(User("eve_p", "patron"));
//        users.push_back(User("libby", "librarian"));
//        users.push_back(User("admin", "admin"));
//    }

//    /*
//        Function: initializeCatalogue
//        Purpose: Creates the initial library inventory with 20 diverse items. Populates the catalogue with
//            5 fiction, 5 non-fiction, 3 magazines, 3 movies, and 4 video games as per requirements
//    */
//    void initializeCatalogue() {
//        // 5 Fiction Books with realistic publication years and ISBNs
//        catalogue.push_back(new FictionBook("The Great Gatsby", "F. Scott Fitzgerald", 1925, "Good", "978-0-7432-7356-5"));
//        catalogue.push_back(new FictionBook("To Kill a Mockingbird", "Harper Lee", 1960, "Excellent", "978-0-06-112008-4"));
//        catalogue.push_back(new FictionBook("1984", "George Orwell", 1949, "Good", "978-0-452-28423-4"));
//        catalogue.push_back(new FictionBook("Pride and Prejudice", "Jane Austen", 1813, "Fair", "978-0-14-143951-8"));
//        catalogue.push_back(new FictionBook("The Hobbit", "J.R.R. Tolkien", 1937, "Excellent", "978-0-547-92822-7"));

//        // 5 Non-Fiction Books with Dewey Decimal, years, and ISBNs
//        catalogue.push_back(new NonFictionBook("Sapiens", "Yuval Noah Harari", "909.04", 2011, "Excellent", "978-0-06-231609-7"));
//        catalogue.push_back(new NonFictionBook("Cosmos", "Carl Sagan", "520.92", 1980, "Good", "978-0-375-50832-3"));
//        catalogue.push_back(new NonFictionBook("A Brief History of Time", "Stephen Hawking", "523.01", 1988, "Excellent", "978-0-553-05340-1"));
//        catalogue.push_back(new NonFictionBook("The Selfish Gene", "Richard Dawkins", "576.82", 1976, "Good", "978-0-19-286092-7"));
//        catalogue.push_back(new NonFictionBook("Silent Spring", "Rachel Carson", "632.95", 1962, "Fair", "978-0-618-24906-0"));

//        // 3 Magazines with issue numbers and publication dates
//        catalogue.push_back(new Magazine("National Geographic", "Various", 255, "January 2024", 2024, "Excellent"));
//        catalogue.push_back(new Magazine("Scientific American", "Various", 330, "March 2024", 2024, "Good"));
//        catalogue.push_back(new Magazine("The Economist", "Various", 452, "February 2024", 2024, "Excellent"));

//        // 3 Movies with genre and rating information
//        catalogue.push_back(new Movie("Inception", "Christopher Nolan", "Sci-Fi/Thriller", "PG-13", 2010, "Excellent"));
//        catalogue.push_back(new Movie("The Shawshank Redemption", "Frank Darabont", "Drama", "R", 1994, "Good"));
//        catalogue.push_back(new Movie("Spirited Away", "Hayao Miyazaki", "Animation/Fantasy", "PG", 2001, "Excellent"));

//        // 4 Video Games with genre and rating information
//        catalogue.push_back(new VideoGame("The Legend of Zelda: Breath of the Wild", "Nintendo", "Action-Adventure", "E10+", 2017, "Excellent"));
//        catalogue.push_back(new VideoGame("Portal 2", "Valve", "Puzzle-Platform", "E10+", 2011, "Good"));
//        catalogue.push_back(new VideoGame("Minecraft", "Mojang", "Sandbox/Survival", "E10+", 2011, "Excellent"));
//        catalogue.push_back(new VideoGame("Celeste", "Maddy Makes Games", "Platformer", "E10+", 2018, "Good"));
//    }
//};

//#endif
