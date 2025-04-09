#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QVariantMap>
#include <QList>
#include "Task.h"

class TaskManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskManager(int userId, QObject *parent = nullptr);

    // Task operations
    QList<QVariantMap> getAllTasks() const;
    int createTask(const QString &name);
    bool updateTask(int taskId, const QVariantMap &data);
    bool deleteTask(int taskId);
    bool completeTask(int taskId);
    bool recordPomodoro(int taskId);
    int getCompletedPomodoros(int taskId);
    QList<QVariantMap> getPomodoroStats(int taskId);

signals:
    void taskCreated(int taskId, const QVariantMap &taskData);
    void taskUpdated(int taskId, const QVariantMap &taskData);
    void taskDeleted(int taskId, const QVariantMap &taskData);
    void taskCompleted(int taskId, const QVariantMap &taskData);
    void pomodoroRecorded(int taskId, const QVariantMap &stats);
    void error(const QString &message, int taskId);

private:
    int m_userId;
    QVariantMap getTaskData(int taskId) const;
    TaskStatus statusFromString(const QString &statusStr);
};

#endif // TASKMANAGER_H
