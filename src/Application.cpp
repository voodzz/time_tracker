#include "Application.h"
#include "ui/MainWindow.h"
#include <QDebug>

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setupTranslations();
    m_themeManager = new ThemeManager(this);

    MainWindow *w = new MainWindow;
    w->show();
}

Application::~Application()
{
}

void Application::setupTranslations()
{
    
}
