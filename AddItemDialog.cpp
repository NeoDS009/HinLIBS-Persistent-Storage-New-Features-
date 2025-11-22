#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QDate>
#include "AddItemDialog.h"

AddItemDialog::AddItemDialog(QWidget *parent) : QDialog(parent) {
    setupUI();
}

void AddItemDialog::setupUI() {
    setWindowTitle("Add New Item to Catalogue");
    setFixedSize(400, 500);

    QFormLayout *formLayout = new QFormLayout(this);

    // Item Type
    itemTypeCombo = new QComboBox();
    itemTypeCombo->addItems({"Fiction Book", "Non-Fiction Book", "Magazine", "Movie", "Video Game"});
    formLayout->addRow("Item Type:", itemTypeCombo);
    connect(itemTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddItemDialog::updateFieldsVisibility);

    // Basic fields (always visible)
    titleEdit = new QLineEdit();
    authorEdit = new QLineEdit();
    formLayout->addRow("Title:", titleEdit);
    formLayout->addRow("Author/Creator:", authorEdit);

    // Publication info
    publicationYearSpin = new QSpinBox();
    publicationYearSpin->setRange(1000, QDate::currentDate().year());
    publicationYearSpin->setValue(2024);
    formLayout->addRow("Publication Year:", publicationYearSpin);

    // Condition
    conditionCombo = new QComboBox();
    conditionCombo->addItems({"Excellent", "Good", "Fair", "Poor"});
    formLayout->addRow("Condition:", conditionCombo);

    // Book-specific fields
    deweyDecimalEdit = new QLineEdit();
    deweyDecimalEdit->setPlaceholderText("XXX.XX");
    formLayout->addRow("Dewey Decimal:", deweyDecimalEdit);

    isbnEdit = new QLineEdit();
    isbnEdit->setPlaceholderText("978-0-0000-0000-0");
    formLayout->addRow("ISBN:", isbnEdit);

    // Magazine-specific fields
    issueNumberSpin = new QSpinBox();
    issueNumberSpin->setRange(1, 1000);
    formLayout->addRow("Issue Number:", issueNumberSpin);

    publicationDateEdit = new QLineEdit();
    publicationDateEdit->setPlaceholderText("January 2024");
    formLayout->addRow("Publication Date:", publicationDateEdit);

    // Media-specific fields
    genreEdit = new QLineEdit();
    formLayout->addRow("Genre:", genreEdit);

    ratingCombo = new QComboBox();
    ratingCombo->addItems({"G", "PG", "PG-13", "R", "E", "E10+", "T", "M", "AO"});
    formLayout->addRow("Rating:", ratingCombo);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    formLayout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    updateFieldsVisibility();
}

void AddItemDialog::updateFieldsVisibility() {
    QString itemType = itemTypeCombo->currentText();

    // Determine what to show based on item type
    bool isBook = itemType.contains("Book");
    bool isNonFiction = (itemType == "Non-Fiction Book");
    bool isMagazine = (itemType == "Magazine");
    bool isMedia = (itemType == "Movie" || itemType == "Video Game");

    // Show/hide fields directly
    deweyDecimalEdit->setVisible(isNonFiction);
    isbnEdit->setVisible(isBook);
    issueNumberSpin->setVisible(isMagazine);
    publicationDateEdit->setVisible(isMagazine);
    genreEdit->setVisible(isMedia);
    ratingCombo->setVisible(isMedia);

    // Also hide the labels - we need to find them in the layout
    QFormLayout *layout = qobject_cast<QFormLayout*>(this->layout());
    if (layout) {
        for (int i = 0; i < layout->rowCount(); ++i) {
            QLayoutItem *labelItem = layout->itemAt(i, QFormLayout::LabelRole);
            QLayoutItem *fieldItem = layout->itemAt(i, QFormLayout::FieldRole);

            if (labelItem && fieldItem) {
                QWidget *fieldWidget = fieldItem->widget();

                if (fieldWidget == deweyDecimalEdit) {
                    labelItem->widget()->setVisible(isNonFiction);
                } else if (fieldWidget == isbnEdit) {
                    labelItem->widget()->setVisible(isBook);
                } else if (fieldWidget == issueNumberSpin) {
                    labelItem->widget()->setVisible(isMagazine);
                } else if (fieldWidget == publicationDateEdit) {
                    labelItem->widget()->setVisible(isMagazine);
                } else if (fieldWidget == genreEdit) {
                    labelItem->widget()->setVisible(isMedia);
                } else if (fieldWidget == ratingCombo) {
                    labelItem->widget()->setVisible(isMedia);
                }
            }
        }
    }
}

// Getters for the entered data
QString AddItemDialog::getItemType() const { return itemTypeCombo->currentText(); }
QString AddItemDialog::getTitle() const { return titleEdit->text(); }
QString AddItemDialog::getAuthor() const { return authorEdit->text(); }
QString AddItemDialog::getDeweyDecimal() const { return deweyDecimalEdit->text(); }
QString AddItemDialog::getISBN() const { return isbnEdit->text(); }
QString AddItemDialog::getGenre() const { return genreEdit->text(); }
QString AddItemDialog::getRating() const { return ratingCombo->currentText(); }
int AddItemDialog::getIssueNumber() const { return issueNumberSpin->value(); }
QString AddItemDialog::getPublicationDate() const { return publicationDateEdit->text(); }
int AddItemDialog::getPublicationYear() const { return publicationYearSpin->value(); }
QString AddItemDialog::getCondition() const { return conditionCombo->currentText(); }
