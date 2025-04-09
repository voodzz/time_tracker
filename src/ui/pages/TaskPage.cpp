#include "TaskPage.h"
#include "../widgets/PomodoroTimer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QDebug>

TaskPage::TaskPage(int userId, QWidget *parent, HistoryPage *historyPage)
    : BasePage(parent), m_userId(userId), m_historyPage(historyPage), m_tasks()
{
    m_taskManager = new TaskManager(userId, this);
    setupUi();
    setupConnections();
    loadTasks();
}

void TaskPage::setupUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Left panel with tasks
    QWidget *tasksPanel = new QWidget(this);
    tasksPanel->setObjectName("tasksPanel");
    QVBoxLayout *tasksPanelLayout = new QVBoxLayout(tasksPanel);

    // Add task section
    QHBoxLayout *addTaskLayout = new QHBoxLayout;
    m_taskInput = new QLineEdit(this);
    m_taskInput->setPlaceholderText(tr("Enter new task"));
    m_addButton = new QPushButton(tr("Add Task"), this);
    addTaskLayout->addWidget(m_taskInput);
    addTaskLayout->addWidget(m_addButton);
    tasksPanelLayout->addLayout(addTaskLayout);

    // Tasks list
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    m_tasksContainer = new QWidget(scrollArea);
    m_tasksContainer->setObjectName("tasksContainer");
    m_tasksLayout = new QVBoxLayout(m_tasksContainer);
    m_tasksLayout->setAlignment(Qt::AlignTop);
    
    scrollArea->setWidget(m_tasksContainer);
    tasksPanelLayout->addWidget(scrollArea);

    // Right panel with timer
    QWidget *timerPanel = new QWidget(this);
    timerPanel->setObjectName("timerPanel");
    QVBoxLayout *timerPanelLayout = new QVBoxLayout(timerPanel);

    m_pomodoroTimer = new PomodoroTimer(this);
    timerPanelLayout->addWidget(m_pomodoroTimer);
    timerPanelLayout->addStretch();

    // Set layout
    mainLayout->addWidget(tasksPanel, 1);
    mainLayout->addWidget(timerPanel, 1);
    setLayout(mainLayout);
}

void TaskPage::setupConnections()
{
    // UI connections
    connect(m_addButton, &QPushButton::clicked, this, &TaskPage::onAddTaskClicked);
    connect(m_taskInput, &QLineEdit::returnPressed, this, &TaskPage::onAddTaskClicked);

    // TaskManager connections
    connect(m_taskManager, &TaskManager::taskCreated, this, &TaskPage::onTaskCreated);
    connect(m_taskManager, &TaskManager::taskUpdated, this, &TaskPage::onTaskUpdated);
    connect(m_taskManager, &TaskManager::taskDeleted, this, &TaskPage::onTaskDeleted);
    connect(m_taskManager, &TaskManager::taskCompleted, this, &TaskPage::onTaskCompleted);
    connect(m_taskManager, &TaskManager::error, this, &TaskPage::showError);

    // Timer connections
    connect(m_pomodoroTimer, &PomodoroTimer::phaseCompleted, this, &TaskPage::onPomodoroPhaseCompleted);
}

void TaskPage::loadTasks()
{
    // Clear existing tasks
    QLayoutItem *child;
    while ((child = m_tasksLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    // Load tasks from TaskManager
    QList<QVariantMap> tasks = m_taskManager->getAllTasks();
    for (const QVariantMap &taskData : tasks) {
        Task *task = new Task(taskData["id"].toInt(), this);
        task->setTaskName(taskData["name"].toString());
        task->setDescription(taskData["description"].toString());
        task->setDeadline(taskData["deadline"].toDateTime().date());
        task->setPlannedCycles(taskData["planned_cycles"].toInt());
        task->setRemainingCycles(taskData["remaining_cycles"].toInt());
        task->setStatus(static_cast<TaskStatus>(taskData["status"].toInt()));
        
        connect(task, &Task::taskCompleted, this, &TaskPage::onTaskCompleted);
        connect(task, &Task::startTimer, m_pomodoroTimer, &PomodoroTimer::startTimer);
        connect(task, &Task::remainingCyclesChanged, this, [this, task](int cycles) {
            task->updateDisplay();
        });
        connect(task, &Task::taskUpdated, m_taskManager, &TaskManager::updateTask);
        connect(task, &Task::taskUpdated, this, &TaskPage::onTaskUpdated);
        connect(task, &Task::taskDeleted, m_taskManager, &TaskManager::deleteTask);
        
        m_tasksLayout->addWidget(task);
        m_tasks[taskData["id"].toInt()] = task;
    }
}

void TaskPage::createTaskWidget(const QVariantMap &taskData)
{
    Task *task = new Task(taskData["id"].toInt(), this);
    task->setTaskName(taskData["name"].toString());
    task->setDescription(taskData["description"].toString());
    task->setDeadline(taskData["deadline"].toDateTime().date());
    task->setPlannedCycles(taskData["planned_cycles"].toInt());
    task->setRemainingCycles(taskData["remaining_cycles"].toInt());
    task->setStatus(static_cast<TaskStatus>(taskData["status"].toInt()));
    
    connect(task, &Task::taskCompleted, this, &TaskPage::onTaskCompleted);
    connect(task, &Task::startTimer, m_pomodoroTimer, &PomodoroTimer::startTimer);
    connect(task, &Task::remainingCyclesChanged, this, [this, task](int cycles) {
        task->updateDisplay();
    });
    connect(task, &Task::taskUpdated, m_taskManager, &TaskManager::updateTask);
    connect(task, &Task::taskUpdated, this, &TaskPage::onTaskUpdated);
    connect(task, &Task::taskDeleted, m_taskManager, &TaskManager::deleteTask);
    
    m_tasksLayout->addWidget(task);
    m_tasks[taskData["id"].toInt()] = task;
}

void TaskPage::onAddTaskClicked()
{
    QString taskName = m_taskInput->text().trimmed();
    if (!taskName.isEmpty()) {
        m_taskManager->createTask(taskName);
        m_taskInput->clear();
    }
}

void TaskPage::onTaskCreated(int taskId, const QVariantMap &taskData)
{
    createTaskWidget(taskData);
}

void TaskPage::onTaskUpdated(int taskId, const QVariantMap &taskData)
{
    bool found = false;
    for (int i = 0; i < m_tasksLayout->count(); ++i) {
        if (Task *task = qobject_cast<Task*>(m_tasksLayout->itemAt(i)->widget())) {
            if (task->id() == taskId) {
                found = true;
                task->setTaskName(taskData["name"].toString());
                task->setDescription(taskData["description"].toString());
                task->setDeadline(taskData["deadline"].toDateTime().date());
                task->setPlannedCycles(taskData["planned_cycles"].toInt());
                task->setRemainingCycles(taskData["remaining_cycles"].toInt());
                if (taskData.contains("status")) {
                    task->setStatus(static_cast<TaskStatus>(taskData["status"].toInt()));
                }
                task->updateDisplay();
                break;
            }
        }
    }

    if (!found) {
        qDebug() << "Warning: Task widget not found for update, creating new widget";
        createTaskWidget(taskData);
    }

    if (m_historyPage) {
        m_historyPage->loadHistory();
    }
}

void TaskPage::onTaskDeleted(int taskId, const QVariantMap &taskData)
{
    // Find and remove the task widget
    for (int i = 0; i < m_tasksLayout->count(); ++i) {
        if (Task *task = qobject_cast<Task*>(m_tasksLayout->itemAt(i)->widget())) {
            if (task->id() == taskId) {
                m_tasksLayout->removeWidget(task);
                task->deleteLater();
                break;
            }
        }
    }
    if (m_historyPage) {
        m_historyPage->loadHistory();
    }
}

void TaskPage::onTaskCompleted(int taskId, const QVariantMap &taskData)
{
    if (m_tasks.contains(taskId)) {
        Task *task = m_tasks[taskId];
        task->setStatus(static_cast<TaskStatus>(taskData["status"].toInt()));
        task->updateDisplay();
    }
}

void TaskPage::onPomodoroPhaseCompleted(PomodoroPhase phase)
{
    if (phase == PomodoroPhase::Work && m_activeTaskId != -1) {
        // Record completed pomodoro for the active task
        m_taskManager->recordPomodoro(m_activeTaskId);
    }
}

void TaskPage::showError(const QString &message)
{
    QMessageBox::warning(this, tr("Error"), message);
}
