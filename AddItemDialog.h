#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>

/*
    AddItemDialog Class:
    Provides a comprehensive form for librarians to add new items to the library catalogue.
    Features dynamic form fields that adapt based on the selected item type, ensuring
    only relevant information is collected for each format.

    Key Features:
    - Dynamic form adaptation based on item type selection
    - Type-specific validation and field requirements
    - Support for all catalogue item types: books, magazines, movies, and video games
    - Professional form layout with appropriate input controls

    Supported Item Types:
    - Fiction Books: Title, author, publication year, condition, ISBN
    - Non-Fiction Books: Title, author, Dewey Decimal, publication year, condition, ISBN
    - Magazines: Title, publisher, issue number, publication date, year, condition
    - Movies: Title, director, genre, rating, publication year, condition
    - Video Games: Title, developer, genre, rating, publication year, condition

    UI Design:
    - Form-based layout with logical grouping of fields
    - Dynamic field visibility based on item type
    - Appropriate input controls (combo boxes, spin boxes, text fields)
    - Placeholder text and validation hints

    Data Members:
      - QComboBox* itemTypeCombo: Selector for item category
      - QLineEdit* titleEdit: Input for item title
      - QLineEdit* authorEdit: Input for author/creator/publisher
      - QLineEdit* deweyDecimalEdit: Dewey Decimal classification (non-fiction only)
      - QLineEdit* isbnEdit: ISBN number (books only)
      - QLineEdit* genreEdit: Genre classification (media only)
      - QComboBox* ratingCombo: Content rating (media only)
      - QSpinBox* issueNumberSpin: Issue number (magazines only)
      - QLineEdit* publicationDateEdit: Publication date (magazines only)
      - QSpinBox* publicationYearSpin: Year of publication
      - QComboBox* conditionCombo: Physical condition assessment

    Member Functions:
      Public:
        - AddItemDialog(): Constructs the dialog with all form components
        - Getters: Comprehensive methods to retrieve all form data

      Private:
        - setupUI(): Initializes and arranges all form components
        - updateFieldsVisibility(): Dynamically shows/hides fields based on item type
*/
class AddItemDialog : public QDialog {
    Q_OBJECT

public:
    /*
        Function: AddItemDialog
        Purpose: Constructs a comprehensive item addition dialog with dynamic form fields
        Parameters:
          in: QWidget* parent - Parent widget (optional)
        Return: AddItemDialog instance with fully configured form interface
    */
    AddItemDialog(QWidget *parent = nullptr);

    // Data Access Methods

    /*
        Function: getItemType
        Purpose: Retrieves the selected item type category
        Return: QString - The chosen item type (e.g., "Fiction Book", "Movie")
    */
    QString getItemType() const;

    /*
        Function: getTitle
        Purpose: Retrieves the entered title of the item
        Return: QString - The title text from the form
    */
    QString getTitle() const;

    /*
        Function: getAuthor
        Purpose: Retrieves the author/creator/publisher information
        Return: QString - The author/creator text from the form
    */
    QString getAuthor() const;

    /*
        Function: getDeweyDecimal
        Purpose: Retrieves the Dewey Decimal classification for non-fiction books
        Return: QString - Dewey Decimal number in XXX.XX format, or empty string
    */
    QString getDeweyDecimal() const;

    /*
        Function: getISBN
        Purpose: Retrieves the ISBN number for books
        Return: QString - ISBN number, or empty string for non-book items
    */
    QString getISBN() const;

    /*
        Function: getGenre
        Purpose: Retrieves the genre classification for movies and video games
        Return: QString - Genre description, or empty string for non-media items
    */
    QString getGenre() const;

    /*
        Function: getRating
        Purpose: Retrieves the content rating for movies and video games
        Return: QString - Content rating (e.g., "PG-13", "E10+"), or empty string
    */
    QString getRating() const;

    /*
        Function: getIssueNumber
        Purpose: Retrieves the issue number for magazines
        Return: int - Issue number, or 0 for non-magazine items
    */
    int getIssueNumber() const;

    /*
        Function: getPublicationDate
        Purpose: Retrieves the publication date for magazines
        Return: QString - Publication date string, or empty string for non-magazines
    */
    QString getPublicationDate() const;

    /*
        Function: getPublicationYear
        Purpose: Retrieves the publication year for all item types
        Return: int - Four-digit publication year
    */
    int getPublicationYear() const;

    /*
        Function: getCondition
        Purpose: Retrieves the physical condition assessment
        Return: QString - Condition description (e.g., "Excellent", "Good", "Fair")
    */
    QString getCondition() const;

private:
    // Form Input Components
    QComboBox *itemTypeCombo;
    QLineEdit *titleEdit;
    QLineEdit *authorEdit;
    QLineEdit *deweyDecimalEdit;
    QLineEdit *isbnEdit;
    QLineEdit *genreEdit;
    QComboBox *ratingCombo;
    QSpinBox *issueNumberSpin;
    QLineEdit *publicationDateEdit;
    QSpinBox *publicationYearSpin;
    QComboBox *conditionCombo;

    /*
        Function: setupUI
        Purpose: Initializes and arranges all dialog components. Creates the form layout,
                 configures input controls with appropriate validators and placeholders,
                 and sets up the dynamic field visibility system.
    */
    void setupUI();

    /*
        Function: updateFieldsVisibility
        Purpose: Dynamically shows or hides form fields based on the selected item type.
                 Ensures users only see relevant input fields for the chosen category.
                 Connected to the itemTypeCombo selection change signal.
    */
    void updateFieldsVisibility();
};

#endif
