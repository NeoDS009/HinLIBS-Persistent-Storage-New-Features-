#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>

class AddItemDialog : public QDialog {
    Q_OBJECT

public:
    AddItemDialog(QWidget *parent = nullptr);

    // Methods to get the entered data
    QString getItemType() const;
    QString getTitle() const;
    QString getAuthor() const;
    QString getDeweyDecimal() const;
    QString getISBN() const;
    QString getGenre() const;
    QString getRating() const;
    int getIssueNumber() const;
    QString getPublicationDate() const;
    int getPublicationYear() const;
    QString getCondition() const;

private:
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

    void setupUI();
    void updateFieldsVisibility();
};

#endif
