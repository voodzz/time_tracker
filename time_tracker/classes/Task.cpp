#include "Task.h"
#include "TimerDialog.h"
#include <QDebug>


// ================================ Constructors & Destructors ==================================
Task::Task(int taskNumber, QWidget *parent) : QWidget(parent), taskNumber(taskNumber)
{
    initVariables();
    initLayouts();
    initConnections();
}


// ======================== Setters =========================================
void Task::setTaskName(const QString &name)
{
    taskNameLabel->setText(QString("<a href='#'>%1</a>").arg(name));
}

void Task::setDuration(int duration)
{
    if (duration < 1 || duration > 7)
        return;

    updateDurationIcons(duration);
}

// =========================== Init Functions ================================

void Task::initVariables()
{
    deadlineButton = new QPushButton(this);
    taskNameLabel = new QLabel(this);
    durationLabel = new QLabel(this);

    taskNameLabel->setText("<a href='#'>Task Name</a>");
    taskNameLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    taskNameLabel->setOpenExternalLinks(false);

    deadlineButton->setCursor(Qt::PointingHandCursor);
    taskNameLabel->setCursor(Qt::PointingHandCursor);

    setDuration(3);
}

void Task::initLayouts()
{
    mainLayout = new QHBoxLayout(this);

    mainLayout->addWidget(deadlineButton);
    mainLayout->addWidget(taskNameLabel);
    mainLayout->addWidget(durationLabel);

    mainLayout->addStretch();

    setLayout(mainLayout);
}

void Task::initConnections()
{
    connect(taskNameLabel, &QLabel::linkActivated, this, &Task::openSettingsDialog);
    connect(deadlineButton, &QPushButton::clicked, this, &Task::showDeadlineDialog);
}

// =========================== Other Functions ===============================
void Task::updateDurationIcons(int duration)
{
    QString icons = "";
    for (int i = 0; i < duration; ++i)
    {
        icons += "⭐";
    }
    durationLabel->setText(icons);
}


// ====================================== On Task Clicked Funtions ===========================
void Task::onTaskNameClicked()
{
    emit taskNameClicked();
}

// =================== Deadline slot ===================
void Task::showDeadlineDialog() {
    DeadlineDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QDate selectedDate = dialog.selectedDate();
        QPushButton *button = qobject_cast<QPushButton*>(sender());
        if (button) {
            button->setText(selectedDate.toString("dd.MM"));
        }
    }
}


// ===================================== Settings Dialog Functions =========================
void Task::openSettingsDialog()
{
    TaskSettingsDialog dialog(this);
    dialog.setTaskName(taskNameLabel->text().remove(QRegExp("<[^>]*>")));
    dialog.setDuration(durationLabel->text().count("⭐"));

    connect(&dialog, &TaskSettingsDialog::taskUpdated, this, &Task::applySettings);
    connect(&dialog, &TaskSettingsDialog::taskDeleted, this, &Task::deleteTask);

    dialog.exec();
}

void Task::applySettings(const QString &name, int duration)
{
    setTaskName(name);
    setDuration(duration);
}

void Task::deleteTask()
{
    emit taskDeleted(this);
    this->deleteLater();
}
