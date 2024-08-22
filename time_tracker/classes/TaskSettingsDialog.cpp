#include "TaskSettingsDialog.h"


// =========================== Constructors & Destructors =============================
TaskSettingsDialog::TaskSettingsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Task Settings");

    initVariables();
    initLayouts();
    initConnections();
}

// ============================ Init Functions ===========================================

void TaskSettingsDialog::initVariables()
{
    nameEdit = new QLineEdit(this);
    durationSpinBox = new QSpinBox(this);
    durationSpinBox->setRange(1, 7);

    applyButton = new QPushButton("Apply", this);
    deleteButton = new QPushButton("Delete Task", this);
}

void TaskSettingsDialog::initLayouts()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(durationSpinBox);

    buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(applyButton);
    buttonsLayout->addWidget(deleteButton);

    mainLayout->addLayout(buttonsLayout);
}

void TaskSettingsDialog::initConnections()
{
    connect(applyButton, &QPushButton::clicked, this, &TaskSettingsDialog::applyChanges);
    connect(deleteButton, &QPushButton::clicked, this, &TaskSettingsDialog::deleteTask);
}

// ============================ Setters ==================================================
void TaskSettingsDialog::setTaskName(const QString &name)
{
    nameEdit->setText(name);
}

void TaskSettingsDialog::setDuration(int duration)
{
    durationSpinBox->setValue(duration);
}

// ============================= Slots ===================================================
void TaskSettingsDialog::applyChanges()
{
    emit taskUpdated(nameEdit->text(), durationSpinBox->value());
    accept();
}

void TaskSettingsDialog::deleteTask()
{
    emit taskDeleted();
    accept();
}
