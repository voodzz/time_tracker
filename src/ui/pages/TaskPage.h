#ifndef TASKPAGE_H
#define TASKPAGE_H

#include "BasePage.h"
#include "HistoryPage.h"
#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>
#include "../../core/TaskManager.h"

// Страница задач теперь принимает userId для загрузки данных из БД.
class TaskPage : public BasePage
{
    Q_OBJECT
public:
    // Конструктор принимает идентификатор пользователя и указатель на HistoryPage (если требуется синхронизация истории)
    explicit TaskPage(int userId = -1, QWidget *parent = nullptr, HistoryPage *historyPage = nullptr);
    ~TaskPage();

protected:
    void setupUi() override;

private slots:
    void onAddTask();
    void onTaskAdded(Task* task);
    void onTaskRemoved(Task* task);

private:
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_taskContainer;
    QVBoxLayout *m_taskLayout;
    QPushButton *m_addTaskButton;

    TaskManager *m_taskManager;
    HistoryPage *m_historyPage;

    // Метод для сортировки и отображения задач по дедлайну
    void sortTasks();
};

#endif // TASKPAGE_H
