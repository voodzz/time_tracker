#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QList>
#include "Task.h"
#include <QDate>

// Структура для хранения элемента истории задачи (можно использовать ту же, что в HistoryPage)
struct TaskHistoryItem {
    QDate date;
    QString taskName;
    int cycles;
};

class TaskManager : public QObject
{
    Q_OBJECT
public:
    // Конструктор принимает идентификатор пользователя, для синхронизации с БД
    explicit TaskManager(int userId, QObject *parent = nullptr);
    ~TaskManager();

    // Добавление задачи – теперь синхронизируется с БД и возвращает объект Task
    Task* addTask(const QString &name, const QDate &deadline, int plannedCycles, const QString &description);
    bool deleteTask(Task *task);

    QList<Task*> tasks() const { return m_tasks; }
    void loadTasksFromDB();

signals:
    void taskAdded(Task *task);
    void taskUpdated(Task *task);
    void taskRemoved(Task *task);

    // Новый сигнал для уведомления об элементе истории
    void taskHistoryItemCreated(const TaskHistoryItem &item);

public slots:
    bool s_updateTask(Task* task, const QString &name, const QString &description, const QDate &deadline, int plannedCycles);


private slots:
    void onTaskUpdated(Task *task);
    void onTaskDeleted(Task *task);

private:
    QList<Task*> m_tasks;
    int m_nextId;
    int m_userId;
};

#endif // TASKMANAGER_H
