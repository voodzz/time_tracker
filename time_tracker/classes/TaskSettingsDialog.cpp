#include "DeadlineDialog.h"
#include "TaskSettingsDialog.h"
#include "TimerDialog.h"
#include <QTime>

// =========================== Constructors & Destructors =============================
TaskSettingsDialog::TaskSettingsDialog(int taskDuration, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Task Settings");

    this->taskDuration = taskDuration;
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
    timerStartButton = new QPushButton("Start", this);

    deadlineButton= new QPushButton(this);
}

void TaskSettingsDialog::initLayouts() {
    layout = new QGridLayout(this);
    layout->addWidget(taskNameLabel, 1, 1);
    layout->addWidget(nameEdit, 1, 2);
    layout->addWidget(durationLabel, 2, 1);
    layout->addWidget(durationSpinBox, 2, 2);
    layout->addWidget(deadlineLabel, 3, 1);
    layout->addWidget(deadlineButton, 3, 2);
    layout->addWidget(workTimeTextLabel, 4, 1);
    layout->addWidget(workTimeLabel, 4, 2);
    layout->addWidget(restTimeTextLabel, 5, 1);
    layout->addWidget(restTimeLabel, 5, 2);
    layout->addWidget(applyButton, 6, 1);
    layout->addWidget(deleteButton, 6, 2);
    layout->addWidget(timerStartButton, 7, 1, 1, 2);
    this->setLayout(layout);
}

void TaskSettingsDialog::initLabels() {
    taskNameLabel = new QLabel("Name:", this);
    durationLabel = new QLabel("Duration:", this);
    deadlineLabel = new QLabel("Deadline:", this);
    workTimeTextLabel = new QLabel("Work time:", this);
    workTimeLabel = new QLabel(this);
    restTimeTextLabel = new QLabel("Rest time:", this);
    restTimeLabel = new QLabel(this);
}

void TaskSettingsDialog::initConnections()
{
    connect(applyButton, &QPushButton::clicked, this, &TaskSettingsDialog::applyChanges);
    connect(deleteButton, &QPushButton::clicked, this, &TaskSettingsDialog::deleteTask);
    connect(deadlineButton, &QPushButton::clicked, this, &TaskSettingsDialog::showDeadlineDialog);
    connect(timerStartButton, &QPushButton::clicked, this, &TaskSettingsDialog::openTimerDialog);
}

// ============================== Main Functionality =====================================

void TaskSettingsDialog::openTimerDialog()
{
    TimerDialog dialog(&workDuration, &breakDuration, this); // Передаем указатели
    dialog.exec();
}

void TaskSettingsDialog::applyChanges() {
    emit taskUpdated(nameEdit->text(), durationSpinBox->value(), deadlineButton->text());
    accept();
}

void TaskSettingsDialog::showDeadlineDialog() {
    DeadlineDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QDate selectedDate = dialog.selectedDate();
        deadlineButton->setText(selectedDate.toString("dd.MM"));
    }
}

// ============================== Setters ===========================================
void TaskSettingsDialog::setTaskName(const QString &name) {
    nameEdit->setText(name);
}

void TaskSettingsDialog::setDuration(int duration) {
    durationSpinBox->setValue(duration);
}

void TaskSettingsDialog::setDeadline(const QString &deadline) {
    deadlineButton->setText(deadline);
}

void TaskSettingsDialog::setWorkDuration(int duration)
{
    workDuration = duration;
    workTimeLabel->setText(QString::number(duration) + " minutes");
}

void TaskSettingsDialog::setBreakDuration(int duration)
{
    breakDuration = duration;
    restTimeLabel->setText(QString::number(duration) + " minutes");
}

// ============================= Slots ===================================================


void TaskSettingsDialog::deleteTask()
{
    emit taskDeleted();
    accept();
}

