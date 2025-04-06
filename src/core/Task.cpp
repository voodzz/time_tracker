#include "Task.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSettings>
#include "TaskSettingsDialog.h"

// Constructor initializes default values and sets up the UI
Task::Task(int id, QWidget *parent)
    : QWidget(parent),
      m_id(id),
      m_taskName("New Task"),
      m_description("Description..."),
      m_deadline(QDate::currentDate().addDays(1)),
      m_plannedCycles(1),
      m_remainingCycles(1),
      m_status(TaskStatus::Active)
{
    setupUi();
    updateDisplay();
}

Task::~Task() { }

// Set up task widget UI elements
void Task::setupUi()
{
    m_layout = new QHBoxLayout(this);
    m_layout->setSpacing(10);
    m_layout->setContentsMargins(5, 5, 5, 5);

    // Timer start button with a play symbol
    tt_startButton = new QPushButton(this);
    tt_startButton->setObjectName("tt_startButton");
    tt_startButton->setText("▶");

    // Task name button (opens the settings dialog)
    tt_nameButton = new QPushButton(this);
    tt_nameButton->setObjectName("tt_nameButton");
    connect(tt_nameButton, &QPushButton::clicked, this, &Task::openTaskSettings);

    tt_deadlineLabel = new QLabel(this);
    tt_deadlineLabel->setObjectName("tt_deadlineLabel");

    tt_cyclesLabel = new QLabel(this);
    tt_cyclesLabel->setObjectName("tt_cyclesLabel");

    tt_descriptionLabel = new QLabel(this);
    tt_descriptionLabel->setObjectName("tt_descriptionLabel");

    m_layout->addWidget(tt_startButton);
    m_layout->addWidget(tt_nameButton);
    m_layout->addWidget(tt_deadlineLabel);
    m_layout->addWidget(tt_cyclesLabel);
    m_layout->addWidget(tt_descriptionLabel);
    m_layout->addStretch();

    // Emit signal to start timer when the button is clicked
    connect(tt_startButton, &QPushButton::clicked, [this]() {
        emit startTimer(this);
    });

    setLayout(m_layout);
}

// Update the displayed information for the task widget
void Task::updateDisplay()
{
    tt_nameButton->setText(m_taskName);
    tt_deadlineLabel->setText(m_deadline.toString("dd.MM.yyyy"));

    // Retrieve star emoji from QSettings (default is "★")
    QSettings settings;
    QString starEmoji = settings.value("starEmoji", "★").toString();
    QString stars;
    for (int i = 0; i < m_remainingCycles; i++) {
        stars.append(starEmoji);
    }
    tt_cyclesLabel->setText(stars);
    tt_descriptionLabel->setText(m_description);
}

// Open task settings dialog; instead of updating immediately, emit updateRequested signal
void Task::openTaskSettings()
{
    TaskSettingsDialog dialog(m_taskName, m_description, m_deadline, m_plannedCycles, this);
    if (dialog.exec() == QDialog::Accepted) {
        QString newName = dialog.taskName();
        QString newDesc = dialog.taskDescription();
        QDate newDeadline = dialog.taskDeadline();
        int newCycles = dialog.taskCycles();
        // Emit updateRequested signal so TaskManager can update DB

        qDebug() << "Updating task with data:" << newName << newDesc << newDeadline << newCycles;
        emit updateRequested(this, newName, newDesc, newDeadline, newCycles);
    }
}

// Update the task's internal state and refresh the UI
void Task::updateTask(const QString &name, const QString &description, const QDate &deadline, int plannedCycles)
{
    m_taskName = name;
    m_description = description;
    m_deadline = deadline;
    m_plannedCycles = plannedCycles;
    m_remainingCycles = plannedCycles;
    updateDisplay();
    emit taskUpdated(this);
}

// Update remaining cycles and status accordingly, then refresh display
void Task::updateCycles(int cycles)
{
    m_remainingCycles -= cycles;
    if (m_remainingCycles <= 0) {
        m_status = TaskStatus::Completed;
        emit statusChanged(m_status);
    }
    updateDisplay();
}
