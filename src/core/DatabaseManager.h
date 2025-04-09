#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql>
#include <QVariantMap>


/*
    TABLES STRUCTURE
1. Таблица пользователей (users)
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(255) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    email VARCHAR(255) NOT NULL UNIQUE,
    created_at TIMESTAMPTZ DEFAULT NOW(),
    updated_at TIMESTAMPTZ DEFAULT NOW()
);
2. Таблица задач (tasks)
CREATE TABLE tasks (
    id SERIAL PRIMARY KEY,
    user_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    description TEXT,
    deadline TIMESTAMPTZ,
    planned_cycles INTEGER NOT NULL,
    remaining_cycles INTEGER NOT NULL,
    status VARCHAR(20) NOT NULL CHECK (status IN ('Active', 'Completed', 'Cancelled')),
    created_at TIMESTAMPTZ DEFAULT NOW(),
    updated_at TIMESTAMPTZ DEFAULT NOW()
);

3. Таблица истории выполнения задач (task_history)

CREATE TABLE task_history (
    id SERIAL PRIMARY KEY,
    task_id INTEGER NOT NULL REFERENCES tasks(id) ON DELETE CASCADE,
    completed_cycles INTEGER NOT NULL,
    interruptions INTEGER DEFAULT 0,
    executed_at TIMESTAMPTZ DEFAULT NOW()
);

5. Таблица статистики (statistics)
CREATE TABLE statistics (
    id SERIAL PRIMARY KEY,
    user_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    total_tasks INTEGER DEFAULT 0,
    completed_tasks INTEGER DEFAULT 0,
    pending_tasks INTEGER DEFAULT 0,
    last_updated TIMESTAMPTZ DEFAULT NOW()
);
6. Таблица логов (logs)
CREATE TABLE logs (
    id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(id) ON DELETE SET NULL,
    event TEXT NOT NULL,
    event_time TIMESTAMPTZ DEFAULT NOW()
);

*/


class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    static DatabaseManager& instance() {
        static DatabaseManager _instance;
        return _instance;
    }

    bool openDB(const QString &host, int port, const QString &dbName,
                const QString &user, const QString &password);

    // Register a new user
    // Returns the new user's ID (>= 1) or -1 if there's an error
    int registerUser(const QString &username, const QString &passwordHash, const QString &email);

    // Login function. Returns user ID if successful, -1 if failed.
    int loginUser(const QString &username, const QString &passwordHash);

    // Get user profile
    QVariantMap getUserProfile(int userId);

    // Update user profile (e.g., email)
    bool updateUserProfile(int userId, const QVariantMap &profile);

    int addTask(int userId,
        const QString &name,
        const QString &description,
        const QDateTime &deadline,
        int plannedCycles,
        int remainingCycles,
        const QString &status);
    QList<QVariantMap> getTasks(int userId);
    QList<QVariantMap> getTaskHistory(int userId);
    bool updateTask(const QVariantMap &taskData);
    bool deleteTask(int taskId);
    bool recordPomodoro(const QVariantMap &pomodoroData);
    int getCompletedPomodoros(int taskId); // Get count of completed pomodoros for a task
    QList<QVariantMap> getPomodoroStats(int taskId); // Get detailed pomodoro statistics for a task

signals:
    void taskUpdated(const QVariantMap &taskData);

private:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    QSqlDatabase m_db;
};

#endif // DatabaseManager_H
