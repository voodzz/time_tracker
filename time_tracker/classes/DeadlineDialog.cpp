#include "DeadlineDialog.h"

#include <QPushButton>
#include <QVBoxLayout>

DeadlineDialog::DeadlineDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Select Date");
    calendar = new QCalendarWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(calendar);

    QPushButton *okButton = new QPushButton("OK", this);
    layout->addWidget(okButton);

    connect(okButton, &QPushButton::clicked, this, &DeadlineDialog::accept);
}

QDate DeadlineDialog::selectedDate() const {
    return calendar->selectedDate();
}
