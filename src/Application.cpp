#include "Application.h"
#include "ui/MainWindow.h"
#include "core/DatabaseManager.h"
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QFile>

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setupTranslations();
    m_themeManager = new ThemeManager(this);

    QCoreApplication::setOrganizationName("voodz_d1sh0w");
    QCoreApplication::setApplicationName("time_tracker");

    // Check if config file exists
    if (!QFile::exists("../config.ini")) {
        QMessageBox::critical(nullptr, "Error", 
            "Configuration file not found!\n"
            "Please copy config.ini.example to config.ini and update the settings.");
        return;
    }

    // Load database configuration
    QSettings settings("../config.ini", QSettings::IniFormat);
    settings.beginGroup("Database");
    QString host = settings.value("host", "localhost").toString();
    int port = settings.value("port", 5432).toInt();
    QString dbName = settings.value("name", "").toString();
    QString user = settings.value("user", "").toString();
    QString password = settings.value("password", "").toString();
    settings.endGroup();

    qDebug() << "Database settings:";
    qDebug() << "Host:" << host;
    qDebug() << "Port:" << port;
    qDebug() << "Database:" << dbName;
    qDebug() << "User:" << user;
    qDebug() << "Password length:" << password.length();

    // Try to connect to database
    bool ok = DatabaseManager::instance().openDB(host, port, dbName, user, password);
    if (!ok) {
        QMessageBox::critical(nullptr, "Error", "Failed to connect to database. Check your configuration.");
        return;
    }

    // Create and show main window
    MainWindow *w = new MainWindow;
    w->show();
}

Application::~Application()
{
}

void Application::setupTranslations()
{
    
}
