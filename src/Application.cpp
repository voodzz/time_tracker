#include "Application.h"
#include "ui/MainWindow.h"
#include "core/DatabaseManager.h"
#include <QDebug>
#include <QMessageBox>
#include <QSettings>

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setupTranslations();
    m_themeManager = new ThemeManager(this);

    QCoreApplication::setOrganizationName("voodz_d1sh0w");
    QCoreApplication::setApplicationName("time_tracker");

    bool ok = DatabaseManager::instance().openDB("localhost", 5432, "database", "user", "password");
    if (!ok) {
        QMessageBox::critical(nullptr, "Error", "Failed to connect to database");
    }

    MainWindow *w = new MainWindow;
    w->show();
}

Application::~Application()
{
}

void Application::setupTranslations()
{
    
}
