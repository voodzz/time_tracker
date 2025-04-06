#include "MainWindow.h"
#include <QStackedWidget>
#include <QWidget>
#include <QHBoxLayout>
#include <QDebug>

#include "SideMenu.h"
#include "pages/TaskPage.h"
#include "pages/HistoryPage.h"
#include "pages/SettingsPage.h"
#include "pages/ProfilePage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupConnections();

    setAttribute(Qt::WA_StyledBackground, true);
    setObjectName("time_tracker");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_sideMenu = new SideMenu(this);
    m_sideMenu->setObjectName("sideMenu");

    m_pages = new QStackedWidget(this);

    // Create pages with userId = -1 (unauthorized user)
    // In ProfilePage, when userId == -1, the login form will be displayed.
    TaskPage *tasksPage = new TaskPage(-1, this, new HistoryPage(-1, this));
    tasksPage->setObjectName("taskPage");

    HistoryPage *historyPage = new HistoryPage(-1, this);
    historyPage->setObjectName("historyPage");

    SettingsPage *settingsPage = new SettingsPage(this);
    settingsPage->setObjectName("settingsPage");

    ProfilePage *profilePage = new ProfilePage(-1, this);
    profilePage->setObjectName("profilePage");

    // Add pages to QStackedWidget:
    // Indices: 0 - TaskPage, 1 - HistoryPage, 2 - SettingsPage, 3 - ProfilePage
    m_pages->addWidget(tasksPage);        
    m_pages->addWidget(historyPage);      
    m_pages->addWidget(settingsPage);     
    m_pages->addWidget(profilePage);      

    mainLayout->addWidget(m_sideMenu);
    mainLayout->addWidget(m_pages);
    setCentralWidget(centralWidget);

    // Connect the login success signal from ProfilePage
    connect(profilePage, &ProfilePage::loginSuccess, this, &MainWindow::onLoginSuccess);
}

void MainWindow::setupConnections()
{
    connect(m_sideMenu, &SideMenu::menuItemClicked, this, &MainWindow::onMenuItemClicked);
}

void MainWindow::onMenuItemClicked(int index)
{
    if (index >= 0 && index < m_pages->count()) {
        m_pages->setCurrentIndex(index);
    } else {
        qDebug() << "Invalid menu index:" << index;
    }
}

void MainWindow::onLoginSuccess(int userId)
{
    // After successful login, update pages that depend on userId.
    // Create new instances of pages with the correct userId.

    // Create a new HistoryPage, passing userId
    HistoryPage *historyPage = new HistoryPage(userId, this);
    historyPage->setObjectName("historyPage");

    // Create a new TaskPage and pass the new HistoryPage to it
    TaskPage *taskPage = new TaskPage(userId, this, historyPage);
    taskPage->setObjectName("taskPage");

    // Create a new ProfilePage using userId
    ProfilePage *profilePage = new ProfilePage(userId, this);
    profilePage->setObjectName("profilePage");

    // Remove old versions of pages that will be replaced.
    QWidget *oldTaskPage = m_pages->widget(0);
    QWidget *oldHistoryPage = m_pages->widget(1);
    QWidget *oldProfilePage = m_pages->widget(3);

    m_pages->removeWidget(oldTaskPage);
    m_pages->removeWidget(oldHistoryPage);
    m_pages->removeWidget(oldProfilePage);

    oldTaskPage->deleteLater();
    oldHistoryPage->deleteLater();
    oldProfilePage->deleteLater();

    // Insert new pages at the same indices
    m_pages->insertWidget(0, taskPage);
    m_pages->insertWidget(1, historyPage);
    m_pages->insertWidget(3, profilePage);

    // Switch to the task page after login
    m_pages->setCurrentIndex(0);
}
