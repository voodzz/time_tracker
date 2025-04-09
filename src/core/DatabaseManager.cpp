#include "DatabaseManager.h"
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
}

DatabaseManager::~DatabaseManager()
{
    if(m_db.isOpen()){
        m_db.close();
    }
}

bool DatabaseManager::openDB(const QString &host, int port, const QString &dbName,
                       const QString &user, const QString &password)
{
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(host);
    m_db.setPort(port);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);
    if(!m_db.open()){
        qDebug() << "Database connection error:" << m_db.lastError().text();
        return false;
    }
    return true;
}

int DatabaseManager::registerUser(const QString &username, const QString &passwordHash, const QString &email)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO users (username, password_hash, email) VALUES (:username, :password_hash, :email) RETURNING id");
    query.bindValue(":username", username);
    query.bindValue(":password_hash", passwordHash);
    query.bindValue(":email", email);
    if(!query.exec()){
        qDebug() << "Registration error:" << query.lastError().text();
        return -1;
    }
    if(query.next()){
        return query.value(0).toInt();
    }
    return -1;
}

int DatabaseManager::loginUser(const QString &username, const QString &passwordHash)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM users WHERE username = :username AND password_hash = :password_hash");
    query.bindValue(":username", username);
    query.bindValue(":password_hash", passwordHash);
    if(!query.exec()){
        qDebug() << "Login error:" << query.lastError().text();
        return -1;
    }
    if(query.next()){
        return query.value(0).toInt();
    }
    return -1;
}

QVariantMap DatabaseManager::getUserProfile(int userId)
{
    QVariantMap profile;
    QSqlQuery query(m_db);
    query.prepare("SELECT id, username, email, created_at FROM users WHERE id = :id");
    query.bindValue(":id", userId);
    if(query.exec() && query.next()){
        profile["id"] = query.value("id");
        profile["username"] = query.value("username");
        profile["email"] = query.value("email");
        profile["created_at"] = query.value("created_at");
    }
    return profile;
}

bool DatabaseManager::updateUserProfile(int userId, const QVariantMap &profile)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE users SET email = :email, updated_at = NOW() WHERE id = :id");
    query.bindValue(":email", profile.value("email"));
    query.bindValue(":id", userId);
    if(!query.exec()){
        qDebug() << "Profile update error:" << query.lastError().text();
        return false;
    }
    return true;
}

// Новый метод: добавление задачи в БД.
// Поля tasks: user_id, name, description, deadline, planned_cycles, remaining_cycles, status.
int DatabaseManager::addTask(int userId,
                             const QString &name,
                             const QString &description,
                             const QDateTime &deadline,
                             int plannedCycles,
                             int remainingCycles,
                             const QString &status)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO tasks (user_id, name, description, deadline, planned_cycles, remaining_cycles, status) "
                  "VALUES (:user_id, :name, :description, :deadline, :planned_cycles, :remaining_cycles, :status) RETURNING id");
    query.bindValue(":user_id", userId);
    query.bindValue(":name", name);
    query.bindValue(":description", description);
    query.bindValue(":deadline", deadline);
    query.bindValue(":planned_cycles", plannedCycles);
    query.bindValue(":remaining_cycles", remainingCycles);
    query.bindValue(":status", status);
    if(!query.exec()){
        qDebug() << "Task addition error:" << query.lastError().text();
        return -1;
    }
    if(query.next()){
        return query.value(0).toInt();
    }
    return -1;
}

// Новый метод: получение списка задач для пользователя.
QList<QVariantMap> DatabaseManager::getTasks(int userId)
{
    QList<QVariantMap> tasks;
    QSqlQuery query(m_db);
    query.prepare("SELECT id, name, description, deadline, planned_cycles, remaining_cycles, status FROM tasks WHERE user_id = :user_id ORDER BY deadline");
    query.bindValue(":user_id", userId);
    if(query.exec()){
         while(query.next()){
             QVariantMap task;
             task["id"] = query.value("id");
             task["name"] = query.value("name");
             task["description"] = query.value("description");
             task["deadline"] = query.value("deadline");
             task["planned_cycles"] = query.value("planned_cycles");
             task["remaining_cycles"] = query.value("remaining_cycles");
             task["status"] = query.value("status");
             tasks.append(task);
         }
    }
    else {
         qDebug() << "Error getting tasks:" << query.lastError().text();
    }
    return tasks;
}

// Новый метод: получение истории выполнения задач для пользователя.
// Здесь объединяем информацию о задаче и истории (упрощённо).
QList<QVariantMap> DatabaseManager::getTaskHistory(int userId)
{
    QList<QVariantMap> history;
    QSqlQuery query(m_db);
    query.prepare("SELECT t.name AS taskName, th.completed_cycles, th.executed_at "
                  "FROM task_history th "
                  "JOIN tasks t ON th.task_id = t.id "
                  "WHERE t.user_id = :user_id "
                  "ORDER BY th.executed_at DESC");
    query.bindValue(":user_id", userId);
    if(query.exec()){
         while(query.next()){
             QVariantMap record;
             record["taskName"] = query.value("taskName");
             record["completed_cycles"] = query.value("completed_cycles");
             record["executed_at"] = query.value("executed_at");
             history.append(record);
         }
    }
    else {
         qDebug() << "Error getting history:" << query.lastError().text();
    }
    return history;
}

bool DatabaseManager::updateTask(const QVariantMap &taskData)
{
    if (!taskData.contains("id")) {
        qDebug() << "Error: missing 'id' field for task update.";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("UPDATE tasks SET "
                  "name = :name, "
                  "description = :description, "
                  "deadline = :deadline, "
                  "planned_cycles = :planned_cycles, "
                  "remaining_cycles = :remaining_cycles, "
                  "status = :status "
                  "WHERE id = :id");

    query.bindValue(":id", taskData["id"]);
    query.bindValue(":name", taskData["name"]);
    query.bindValue(":description", taskData["description"]);
    query.bindValue(":deadline", taskData["deadline"]);
    query.bindValue(":planned_cycles", taskData["planned_cycles"]);
    query.bindValue(":remaining_cycles", taskData["remaining_cycles"]);
    query.bindValue(":status", taskData["status"]);

    if (!query.exec()) {
        qDebug() << "ERROR: update task is failed" << query.lastError().text();
        return false;
    }

    emit taskUpdated(taskData);
    return true;
}

bool DatabaseManager::recordPomodoro(const QVariantMap &pomodoroData)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO task_history (task_id, completed_cycles, interruptions, executed_at) "
                 "VALUES (:task_id, 1, 0, :timestamp) RETURNING id");
    query.bindValue(":task_id", pomodoroData["task_id"]);
    query.bindValue(":timestamp", pomodoroData["timestamp"]);

    if (!query.exec()) {
        qDebug() << "Failed to record pomodoro:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int historyId = query.value(0).toInt();
        
        // Update statistics
        QSqlQuery statsQuery(m_db);
        statsQuery.prepare(
            "UPDATE statistics "
            "SET total_tasks = total_tasks + 1, "
            "    last_updated = NOW() "
            "WHERE user_id = (SELECT user_id FROM tasks WHERE id = :task_id)"
        );
        statsQuery.bindValue(":task_id", pomodoroData["task_id"]);
        
        if (!statsQuery.exec()) {
            qDebug() << "Failed to update statistics:" << statsQuery.lastError().text();
            return false;
        }

        return true;
    }
    return false;
}

int DatabaseManager::getCompletedPomodoros(int taskId) {
    QSqlQuery query(m_db);
    query.prepare("SELECT SUM(completed_cycles) FROM task_history WHERE task_id = :taskId");
    query.bindValue(":taskId", taskId);
    
    if (!query.exec()) {
        qDebug() << "Failed to get completed pomodoros:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

QList<QVariantMap> DatabaseManager::getPomodoroStats(int taskId)
{
    QList<QVariantMap> stats;
    QSqlQuery query(m_db);
    query.prepare("SELECT executed_at, completed_cycles, interruptions "
                 "FROM task_history "
                 "WHERE task_id = :taskId "
                 "ORDER BY executed_at DESC");
    query.bindValue(":taskId", taskId);
    
    if (query.exec()) {
        while (query.next()) {
            QVariantMap record;
            record["timestamp"] = query.value("executed_at").toDateTime();
            record["completed_cycles"] = query.value("completed_cycles").toInt();
            record["interruptions"] = query.value("interruptions").toInt();
            stats.append(record);
        }
    }
    
    return stats;
}

bool DatabaseManager::deleteTask(int taskId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM tasks WHERE id = :id");
    query.bindValue(":id", taskId);

    if (!query.exec()) {
        qWarning() << "Error deleting task:" << query.lastError().text();
        return false;
    }

    // Update statistics for the user
    QSqlQuery statsQuery(m_db);
    statsQuery.prepare(
        "UPDATE statistics "
        "SET total_tasks = total_tasks - 1, "
        "pending_tasks = pending_tasks - 1 "
        "WHERE user_id = (SELECT user_id FROM tasks WHERE id = :id)"
    );
    statsQuery.bindValue(":id", taskId);

    if (!statsQuery.exec()) {
        qWarning() << "Error updating statistics:" << statsQuery.lastError().text();
    }

    return true;
}
