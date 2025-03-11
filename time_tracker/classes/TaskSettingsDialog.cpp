#include "TaskSettingsDialog.h"
#include "DeadlineDialog.h" // If there's a separate dialog for deadline selection
#include "TimerDialog.h"
#include <QDate>

TaskSettingsDialog::TaskSettingsDialog(int taskDuration, QWidget *parent)
    : QDialog(parent),
    taskDuration(taskDuration)
{
    // Set default work/break durations based on taskDuration cycles
    // For example: each cycle = 25 minutes work and 5 minutes break
    workDuration = taskDuration * 25;
    breakDuration = taskDuration * 5;

    setWindowTitle("Task Settings");
    initVariables();
    initLabels();
    initLayouts();
    initConnections();
}

TaskSettingsDialog::~TaskSettingsDialog()
{
}

void TaskSettingsDialog::initVariables()
{
    nameEdit = new QLineEdit(this);
    durationSpinBox = new QSpinBox(this);
    durationSpinBox->setRange(1, 7);
    durationSpinBox->setValue(taskDuration);

    applyButton = new QPushButton("Apply", this);
    deleteButton = new QPushButton("Delete Task", this);
    timerStartButton = new QPushButton("Start Timer", this);
    deadlineButton = new QPushButton("Select Deadline", this);
}

void TaskSettingsDialog::initLabels()
{
    taskNameLabel = new QLabel("Name:", this);
    durationLabel = new QLabel("Duration (cycles):", this);
    deadlineLabel = new QLabel("Deadline:", this);
    workTimeTextLabel = new QLabel("Work time:", this);
    workTimeLabel = new QLabel(QString::number(workDuration) + " minutes", this);
    restTimeTextLabel = new QLabel("Break time:", this);
    restTimeLabel = new QLabel(QString::number(breakDuration) + " minutes", this);
}

void TaskSettingsDialog::initLayouts()
{
    layout = new QGridLayout(this);
    layout->addWidget(taskNameLabel, 0, 0);
    layout->addWidget(nameEdit, 0, 1);
    layout->addWidget(durationLabel, 1, 0);
    layout->addWidget(durationSpinBox, 1, 1);
    layout->addWidget(deadlineLabel, 2, 0);
    layout->addWidget(deadlineButton, 2, 1);
    layout->addWidget(workTimeTextLabel, 3, 0);
    layout->addWidget(workTimeLabel, 3, 1);
    layout->addWidget(restTimeTextLabel, 4, 0);
    layout->addWidget(restTimeLabel, 4, 1);
    layout->addWidget(applyButton, 5, 0);
    layout->addWidget(deleteButton, 5, 1);
    layout->addWidget(timerStartButton, 6, 0, 1, 2);
    setLayout(layout);
}

void TaskSettingsDialog::initConnections()
{
    connect(applyButton, &QPushButton::clicked, this, &TaskSettingsDialog::applyChanges);
    connect(deleteButton, &QPushButton::clicked, this, &TaskSettingsDialog::deleteTask);
    connect(deadlineButton, &QPushButton::clicked, this, &TaskSettingsDialog::showDeadlineDialog);
    connect(timerStartButton, &QPushButton::clicked, this, &TaskSettingsDialog::openTimerDialog);
}

void TaskSettingsDialog::setTaskName(const QString &name)
{
    nameEdit->setText(name);
}

void TaskSettingsDialog::setDuration(int duration)
{
    durationSpinBox->setValue(duration);
}

void TaskSettingsDialog::setDeadline(const QString &deadline)
{
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

void TaskSettingsDialog::applyChanges()
{
    emit taskUpdated(nameEdit->text(), durationSpinBox->value(), deadlineButton->text());
    accept();
}

void TaskSettingsDialog::deleteTask()
{
    emit taskDeleted();
    accept();
}

void TaskSettingsDialog::showDeadlineDialog()
{
    // For demonstration, set current date as deadline
    QDate selectedDate = QDate::currentDate();
    deadlineButton->setText(selectedDate.toString("dd.MM.yyyy"));
}

void TaskSettingsDialog::openTimerDialog()
{
    // Create a new TimerDialog, passing workDuration, breakDuration, and the number of cycles (durationSpinBox value)
    TimerDialog *timerDialog = new TimerDialog(workDuration, breakDuration, durationSpinBox->value());
    timerDialog->setAttribute(Qt::WA_DeleteOnClose);
    timerDialog->show();
}
