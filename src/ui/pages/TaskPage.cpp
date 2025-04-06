#include "TaskPage.h"
#include "../../core/Task.h"
#include "../TimerWindow.h"
#include <QDate>
#include <QLabel>
#include <QDebug>
#include <algorithm>

TaskPage::TaskPage(int userId, QWidget *parent, HistoryPage *historyPage)
    : BasePage(parent)
{
    // Создаем TaskManager с идентификатором пользователя
    m_taskManager = new TaskManager(userId, this);
    setupUi();
    connect(m_taskManager, &TaskManager::taskAdded, this, &TaskPage::onTaskAdded);
    connect(m_taskManager, &TaskManager::taskRemoved, this, &TaskPage::onTaskRemoved);

    // Загружаем задачи из БД при старте
    m_taskManager->loadTasksFromDB();
}

TaskPage::~TaskPage() { }

void TaskPage::setupUi()
{
    m_mainLayout = new QVBoxLayout(this);

    m_addTaskButton = new QPushButton(tr("+"), this);
    m_addTaskButton->setObjectName("addTaskButton");
    connect(m_addTaskButton, &QPushButton::clicked, this, &TaskPage::onAddTask);

    m_taskContainer = new QWidget(this);
    m_taskLayout = new QVBoxLayout(m_taskContainer);
    m_taskLayout->setSpacing(10);
    m_taskLayout->setContentsMargins(10, 10, 10, 10);
    m_taskLayout->addStretch();
    m_taskContainer->setLayout(m_taskLayout);
    m_taskContainer->setObjectName("scrollAreaList");

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(m_taskContainer);
    m_scrollArea->setObjectName("scrollAreaList");

    m_mainLayout->addWidget(m_scrollArea);
    m_mainLayout->addWidget(m_addTaskButton, 0, Qt::AlignCenter);
    setLayout(m_mainLayout);
}

void TaskPage::onAddTask()
{
    // Пример: добавить новую задачу с текущей датой + 1 день, 3 цикла и описанием по умолчанию.
    QString name = tr("Task %1").arg(m_taskManager->tasks().size() + 1);
    QDate deadline = QDate::currentDate().addDays(1);
    int cycles = 3;
    QString description = tr("Task description...");
    m_taskManager->addTask(name, deadline, cycles, description);
    sortTasks();
}

void TaskPage::onTaskAdded(Task *task)
{
    connect(task, &Task::startTimer, this, [this, task]() {
        TimerWindow *timerWindow = new TimerWindow(nullptr, task->remainingCycles());

        connect(timerWindow, &TimerWindow::phaseCompleted, this, [this, task]() {
            task->updateCycles(1);
        });

        connect(timerWindow, &TimerWindow::workEnd, this, [this, timerWindow](){
           timerWindow->deleteLater();
        });
        
        timerWindow->show();
    });

    task->setParent(m_taskContainer);
    m_taskLayout->addWidget(task);
}

void TaskPage::onTaskRemoved(Task *task)
{
    m_taskLayout->removeWidget(task);
}

void TaskPage::sortTasks()
{
    auto tasks = m_taskManager->tasks();
    std::sort(tasks.begin(), tasks.end(), [](Task* a, Task* b) {
        return a->deadline() < b->deadline();
    });

    QLayoutItem *child;
    while ((child = m_taskLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            m_taskLayout->removeWidget(child->widget());
        }
        delete child;
    }

    for (Task* task : tasks) {
        m_taskLayout->addWidget(task);
    }
}
