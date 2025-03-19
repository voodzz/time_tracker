#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "core/ThemeManager.h"

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int &argc, char **argv);
    ~Application();

    ThemeManager *themeManager() const { return m_themeManager; }

private:
    void setupTranslations();

    ThemeManager *m_themeManager;
};

#endif // APPLICATION_H
