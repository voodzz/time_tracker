#include "TaskManager.h"
#include "DatabaseManager.h"
#include <QDateTime>
#include <QDebug>

TaskManager::TaskManager(int userId, QObject *parent)
    : QObject(parent), m_userId(userId)
{
}

QList<QVariantMap> TaskManager::getAllTasks() const
{
    return DatabaseManager::instance().getTasks(m_userId);
}

int TaskManager::createTask(const QString &name)
{
    int taskId = DatabaseManager::instance().addTask(
        m_userId,
        name,
        tr("New task description..."),
        QDateTime(QDate::currentDate().addDays(1), QTime(23, 59, 59)),
        1, // plannedCycles
        1, // remainingCycles
        Task::toString(TaskStatus::Active)
    );

    if (taskId != -1) {
        QVariantMap taskData;
        taskData["id"] = taskId;
        taskData["name"] = name;
        taskData["description"] = tr("New task description...");
        taskData["deadline"] = QDateTime(QDate::currentDate().addDays(1), QTime(23, 59, 59));
        taskData["planned_cycles"] = 1;
        taskData["remaining_cycles"] = 1;
        taskData["status"] = static_cast<int>(TaskStatus::Active);
        emit taskCreated(taskId, taskData);
        return taskId;
    } else {
        emit error(tr("Failed to create task"), taskId);
        return -1;
    }
}

bool TaskManager::updateTask(int taskId, const QVariantMap &data)
{
    // Проверяем наличие обязательных полей
    if (!data.contains("name") || !data.contains("status")) {
        emit error(tr("Missing required fields for task update"), taskId);
        return false;
    }

    // Получаем текущие данные задачи
    QVariantMap currentTask = getTaskData(taskId);
    if (currentTask.isEmpty()) {
        emit error(tr("Task not found"), taskId);
        return false;
    }

    // Подготавливаем данные для обновления
    QVariantMap updateData;
    updateData["id"] = taskId;
    updateData["name"] = data.contains("name") ? data["name"] : currentTask["name"];
    updateData["description"] = data.contains("description") ? data["description"] : currentTask["description"];
    updateData["deadline"] = data.contains("deadline") ? data["deadline"] : currentTask["deadline"];
    updateData["planned_cycles"] = data.contains("planned_cycles") ? data["planned_cycles"] : currentTask["planned_cycles"];
    updateData["remaining_cycles"] = data.contains("remaining_cycles") ? data["remaining_cycles"] : currentTask["remaining_cycles"];
    updateData["status"] = Task::toString(statusFromString(data["status"].toString())); // Convert TaskStatus to string before storing in QVariantMap

    try {
        if (DatabaseManager::instance().updateTask(updateData)) {
            emit taskUpdated(taskId, updateData);
            return true;
        } else {
            emit error(tr("Failed to update task in database"), taskId);
            return false;
        }
    } catch (const std::exception& e) {
        emit error(tr("Failed to update task: ") + QString::fromStdString(e.what()), taskId);
        return false;
    }
}

bool TaskManager::deleteTask(int taskId)
{
    QVariantMap currentTask = getTaskData(taskId);
    if (currentTask.isEmpty()) {
        emit error(tr("Task not found"), taskId);
        return false;
    }

    try {
        if (DatabaseManager::instance().deleteTask(taskId)) {
            emit taskDeleted(taskId, currentTask);
            return true;
        } else {
            emit error(tr("Failed to delete task from database"), taskId);
            return false;
        }
    } catch (const std::exception& e) {
        emit error(tr("Failed to delete task: ") + QString::fromStdString(e.what()), taskId);
        return false;
    }
}

bool TaskManager::completeTask(int taskId)
{
    QVariantMap taskData;
    taskData["id"] = taskId;
    taskData["user_id"] = m_userId;
    taskData["status"] = Task::toString(TaskStatus::Completed);
    taskData["completed_at"] = QDateTime::currentDateTime();

    try {
        if (DatabaseManager::instance().updateTask(taskData)) {
            emit taskCompleted(taskId, taskData);
            return true;
        } else {
            emit error(tr("Failed to complete task"), taskId);
            return false;
        }
    } catch (const std::exception& e) {
        emit error(tr("Failed to complete task"), taskId);
        return false;
    }
}

bool TaskManager::recordPomodoro(int taskId)
{
    QVariantMap pomodoroData;
    pomodoroData["task_id"] = taskId;
    pomodoroData["timestamp"] = QDateTime::currentDateTime();
    
    try {
        if (DatabaseManager::instance().recordPomodoro(pomodoroData)) {
            // Update task's remaining cycles
            QVariantMap taskData;
            taskData["id"] = taskId;
            taskData["user_id"] = m_userId;
            
            // Get current task data to update remaining cycles
            QVariantMap currentTask = getTaskData(taskId);
            if (!currentTask.isEmpty()) {
                int remainingCycles = currentTask["remaining_cycles"].toInt();
                if (remainingCycles > 0) {
                    taskData["remaining_cycles"] = remainingCycles - 1;
                    
                    // Update task status if all cycles are completed
                    if (remainingCycles == 1) {
                        taskData["status"] = Task::toString(TaskStatus::Completed);
                    }
                    
                    // Update task in database
                    if (DatabaseManager::instance().updateTask(taskData)) {
                        QVariantMap stats;
                        stats["completed_pomodoros"] = getCompletedPomodoros(taskId);
                        stats["timestamp"] = pomodoroData["timestamp"];
                        emit pomodoroRecorded(taskId, stats);
                        return true;
                    }
                }
            }
            return false;
        } else {
            emit error(tr("Failed to record pomodoro: DB error"), taskId);
            return false;
        }
    } catch (const std::exception& e) {
        emit error(tr("Failed to record pomodoro: TM error"), taskId);
        return false;
    }
}

int TaskManager::getCompletedPomodoros(int taskId) 
{
    try {
        return DatabaseManager::instance().getCompletedPomodoros(taskId);
    } catch (const std::exception& e) {
        emit error(tr("Failed to get completed pomodoros"), taskId);
        return -1;
    }
}

QList<QVariantMap> TaskManager::getPomodoroStats(int taskId)
{
    try {
        return DatabaseManager::instance().getPomodoroStats(taskId);
    } catch (const std::exception& e) {
        emit error(tr("Failed to get pomodoro stats"), taskId);
        return QList<QVariantMap>();
    }
}

QVariantMap TaskManager::getTaskData(int taskId) const
{
    QList<QVariantMap> tasks = DatabaseManager::instance().getTasks(m_userId);
    for (const QVariantMap &task : tasks) {
        if (task["id"].toInt() == taskId) {
            return task;
        }
    }
    return QVariantMap();
}

TaskStatus TaskManager::statusFromString(const QString &statusStr)
{
    if (statusStr == "Active")
        return TaskStatus::Active;
    else if (statusStr == "Completed")
        return TaskStatus::Completed;
    else if (statusStr == "Cancelled")
        return TaskStatus::Cancelled;
    return TaskStatus::Active; // Default status
}
