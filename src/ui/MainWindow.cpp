#include "MainWindow.h"
#include "pages/TaskPage.h"
#include "pages/HistoryPage.h"
#include "pages/SettingsPage.h"
#include "pages/ProfilePage.h"
#include "pages/StartPage.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_currentUserId(-1), m_startPage(nullptr)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setObjectName("time_tracker");
    
    setupUi();
    setupConnections();
    showLoginPage();
}

MainWindow::~MainWindow() { }

void MainWindow::setupUi()
{
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    m_sideMenu = new SideMenu(this);
    m_sideMenu->setObjectName("sideMenu");
    m_sideMenu->hide();
    
    m_pages = new QStackedWidget(this);
    m_pages->setObjectName("stackedWidget");
    
    m_startPage = new StartPage(this);
    m_startPage->setObjectName("startPage");

    mainLayout->addWidget(m_sideMenu);
    mainLayout->addWidget(m_pages);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::setupConnections()
{
    connect(m_sideMenu, &SideMenu::menuItemClicked, this, &MainWindow::onMenuItemClicked);
    connect(m_startPage, &StartPage::loginSuccess, this, &MainWindow::onLoginSuccess);
}

void MainWindow::showLoginPage()
{
    qDebug() << "Showing login page";
    // Hide side menu first
    m_sideMenu->hide();
    
    // Remove all pages except start page
    while (m_pages->count() > 0) {
        QWidget *widget = m_pages->widget(0);
        if (widget != m_startPage) {
            widget->disconnect();
            m_pages->removeWidget(widget);
            widget->deleteLater();
        } else {
            m_pages->removeWidget(widget);
        }
    }
    
    // Add and show start page
    m_pages->addWidget(m_startPage);
    m_pages->setCurrentWidget(m_startPage);

    // Try auto-login
    if (!m_startPage->tryAutoLogin()) {
        // No auto-login possible, stay on login page
    }
}

void MainWindow::showMainContent(int userId)
{
    qDebug() << "Showing main content for user:" << userId;
    m_currentUserId = userId;
    
    // Remove all pages except start page
    while (m_pages->count() > 0) {
        QWidget *widget = m_pages->widget(0);
        if (widget != m_startPage) {
            widget->disconnect();
            m_pages->removeWidget(widget);
            widget->deleteLater();
        } else {
            m_pages->removeWidget(widget);
        }
    }

    // Create and add all main pages
    HistoryPage *historyPage = new HistoryPage(userId, this);
    historyPage->setObjectName("historyPage");
    
    TaskPage *taskPage = new TaskPage(userId, this, historyPage);
    taskPage->setObjectName("taskPage");
    
    SettingsPage *settingsPage = new SettingsPage(this);
    settingsPage->setObjectName("settingsPage");
    
    ProfilePage *profilePage = new ProfilePage(userId, this);
    profilePage->setObjectName("profilePage");
    
    // Connect logout signal from profile page
    connect(profilePage, &ProfilePage::logoutRequested, this, &MainWindow::onLogoutRequested, Qt::QueuedConnection);

    m_pages->addWidget(taskPage);
    m_pages->addWidget(historyPage);
    m_pages->addWidget(settingsPage);
    m_pages->addWidget(profilePage);

    // Show side menu and first page
    m_sideMenu->show();
    m_pages->setCurrentIndex(0);
}

void MainWindow::onMenuItemClicked(int index)
{
    if (m_currentUserId == -1) return;
    m_pages->setCurrentIndex(index);
}

void MainWindow::onLoginSuccess(int userId)
{
    showMainContent(userId);
}

void MainWindow::onLogoutRequested()
{
    qDebug() << "Logout requested";
    // First set current user id to -1
    m_currentUserId = -1;
    
    // Then show login page, which will clean up other pages
    showLoginPage();
}
