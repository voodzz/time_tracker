#ifndef TASKPAGE_H
#define TASKPAGE_H

#include "BasePage.h"
#include "HistoryPage.h"
#include "../../core/TaskManager.h"
#include "../../core/Task.h"
#include "../widgets/PomodoroTimer.h"
#include <QHash>

class QVBoxLayout;
class QLineEdit;
class QPushButton;
class PomodoroTimer;

class TaskPage : public BasePage
{
    Q_OBJECT
public:
    explicit TaskPage(int userId, QWidget *parent = nullptr, HistoryPage *historyPage = nullptr);

private slots:
    void onAddTaskClicked();
    void onTaskCreated(int taskId, const QVariantMap &taskData);
    void onTaskUpdated(int taskId, const QVariantMap &taskData);
    void onTaskDeleted(int taskId, const QVariantMap &taskData);
    void onTaskCompleted(int taskId, const QVariantMap &taskData);
    void onPomodoroPhaseCompleted(PomodoroPhase phase);
    void showError(const QString &message);

private:
    void setupUi();
    void setupConnections();
    void loadTasks();
    void createTaskWidget(const QVariantMap &taskData);

    int m_userId;
    int m_activeTaskId = -1;
    TaskManager *m_taskManager;
    HistoryPage *m_historyPage;

    QLineEdit *m_taskInput;
    QPushButton *m_addButton;
    QWidget *m_tasksContainer;
    QVBoxLayout *m_tasksLayout;
    PomodoroTimer *m_pomodoroTimer;
    QHash<int, Task*> m_tasks;  // Stores task objects by their IDs
};

#endif // TASKPAGE_H
