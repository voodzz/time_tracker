#include "TaskManager.h"
#include "DatabaseManager.h"
#include <QDateTime>
#include <QDebug>

// Constructor: initialize with userId and starting task id
TaskManager::TaskManager(int userId, QObject *parent)
    : QObject(parent), m_userId(userId), m_nextId(1)
{
}

TaskManager::~TaskManager()
{
    qDeleteAll(m_tasks);
    m_tasks.clear();
}

// Add a new task to the database and create the corresponding Task object
Task* TaskManager::addTask(const QString &name, const QDate &deadline, int plannedCycles, const QString &description)
{
    // Insert task into the database and get its ID
    int dbId = DatabaseManager::instance().addTask(m_userId,
                                                   name,
                                                   description,
                                                   QDateTime(deadline, QTime(0,0)),
                                                   plannedCycles,
                                                   plannedCycles, // initial remaining cycles equals plannedCycles
                                                   "Active");
    if (dbId == -1) {
        qDebug() << "Failed to add task to the database.";
        return nullptr;
    }

    // Create a new Task object using the database id
    Task *task = new Task(dbId, nullptr);
    task->updateTask(name, description, deadline, plannedCycles);

    m_tasks.append(task);
    // Connect the updateRequested signal from Task to our updateTask slot for DB synchronization
    connect(task, &Task::updateRequested, this, &TaskManager::s_updateTask);
    connect(task, &Task::taskDeleted, this, &TaskManager::onTaskDeleted);

    emit taskAdded(task);

    // Emit history signal (optional)
    TaskHistoryItem historyItem;
    historyItem.date = QDate::currentDate(); // can be adjusted as needed
    historyItem.taskName = task->taskName();
    historyItem.cycles = plannedCycles;
    emit taskHistoryItemCreated(historyItem);

    return task;
}

// Update the task by synchronizing changes with the database.
// This slot now matches the signal signature: (Task*, name, description, deadline, plannedCycles)
bool TaskManager::s_updateTask(Task *task, const QString &name, const QString &description, const QDate &deadline, int plannedCycles)
{
    qDebug() << "Updating task with id:" << task->id();
    if (!m_tasks.contains(task))
        return false;


    // Build a QVariantMap for the database update
    QVariantMap taskData;
    taskData["id"] = task->id();
    taskData["name"] = name;
    taskData["description"] = description;
    taskData["deadline"] = QDateTime(deadline, QTime(0,0));
    taskData["planned_cycles"] = plannedCycles;
    // For simplicity, assume remaining_cycles resets to plannedCycles on update
    taskData["remaining_cycles"] = plannedCycles;
    taskData["status"] = Task::toString(task->status());

    // Update the task in the database
    if (!DatabaseManager::instance().updateTask(taskData)) {
        qDebug() << "Database update failed for task id:" << task->id();
        return false;
    }

    // Update the task object (UI) if the database update succeeded
    task->updateTask(name, description, deadline, plannedCycles);
    emit taskUpdated(task);
    return true;
}

bool TaskManager::deleteTask(Task *task)
{
    if (!m_tasks.contains(task))
        return false;
    m_tasks.removeOne(task);
    emit taskRemoved(task);
    task->deleteLater();

    // Optionally, call DatabaseManager::deleteTask(task->id()) here.
    return true;
}

void TaskManager::onTaskUpdated(Task *task)
{
    emit taskUpdated(task);
}

void TaskManager::onTaskDeleted(Task *task)
{
    if (m_tasks.contains(task)) {
        m_tasks.removeOne(task);
        emit taskRemoved(task);
    }
}

// Load tasks from the database and create Task objects accordingly
void TaskManager::loadTasksFromDB()
{
    QList<QVariantMap> tasksData = DatabaseManager::instance().getTasks(m_userId);
    for (const QVariantMap &data : tasksData) {
        int id = data.value("id").toInt();
        QString name = data.value("name").toString();
        QString description = data.value("description").toString();
        QDate deadline = data.value("deadline").toDate();
        int plannedCycles = data.value("planned_cycles").toInt();
        // Create a Task object with the retrieved data
        Task *task = new Task(id, nullptr);
        task->updateTask(name, description, deadline, plannedCycles);
        connect(task, &Task::updateRequested, this, &TaskManager::s_updateTask);
        m_tasks.append(task);
        emit taskAdded(task);
    }
}
