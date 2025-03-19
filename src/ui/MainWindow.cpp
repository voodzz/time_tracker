#include "MainWindow.h"
#include <QStackedWidget>
#include <QWidget>
#include <QHBoxLayout>
#include <QDebug>

#include "pages/TaskPage.h"
#include "pages/HistoryPage.h"
#include "pages/SettingsPage.h"
#include "pages/ProfilePage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupConnections();
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

    TaskPage *tasksPage = new TaskPage();
    tasksPage->setObjectName("taskPage");
    tasksPage->setStyleSheet("background-color: #BA68C8;");

    HistoryPage *historyPage = new HistoryPage();
    historyPage->setObjectName("historyPage");
    historyPage->setStyleSheet("background-color: #BA68C8;");

    SettingsPage *settingsPage = new SettingsPage();
    settingsPage->setObjectName("settingsPage");
    settingsPage->setStyleSheet("background-color: #BA68C8;");

    ProfilePage *profilePage = new ProfilePage();
    profilePage->setObjectName("profilePage");
    profilePage->setStyleSheet("background-color: #BA68C8;");

    m_pages->addWidget(tasksPage);      // Index 0
    m_pages->addWidget(historyPage);    // Index 1
    m_pages->addWidget(settingsPage);   // Index 2
    m_pages->addWidget(profilePage);    // Index 3

    mainLayout->addWidget(m_sideMenu);
    mainLayout->addWidget(m_pages);

    setCentralWidget(centralWidget);
}

void MainWindow::setupConnections()
{
    connect(m_sideMenu, &SideMenu::menuItemClicked,
            this, &MainWindow::onMenuItemClicked);
}

void MainWindow::onMenuItemClicked(int index)
{
    if (index >= 0 && index < m_pages->count()) {
        m_pages->setCurrentIndex(index);
    } else {
        qDebug() << "Invalid menu index: " << index;
    }
}
