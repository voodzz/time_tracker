#include "TaskSettingsDialog.h"


// =========================== Constructors & Destructors =============================
TaskSettingsDialog::TaskSettingsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Task Settings");

    initVariables();
    initLabels();
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
    layout = new QGridLayout(this);
    layout->addWidget(taskNameLabel, 1, 1);
    layout->addWidget(nameEdit, 1, 2);
    layout->addWidget(durationLabel, 2, 1);
    layout->addWidget(durationSpinBox, 2, 2);
    layout->addWidget(workTimeTextLabel, 3, 1);
    layout->addWidget(workTimeLabel, 3, 2);
    layout->addWidget(restTimeTextLabel, 4, 1);
    layout->addWidget(restTimeLabel, 4, 2);
    layout->addWidget(applyButton, 5, 1);
    layout->addWidget(deleteButton, 5, 2);
    this->setLayout(layout);
}

void TaskSettingsDialog::initLabels() {
    taskNameLabel = new QLabel("Name:", this);
    durationLabel = new QLabel("Duration:", this);
    workTimeTextLabel = new QLabel("Work time:", this);
    workTimeLabel = new QLabel(this);
    restTimeTextLabel = new QLabel("Rest time:", this);
    restTimeLabel = new QLabel(this);
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
