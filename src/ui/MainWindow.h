#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SideMenu.h"

class QStackedWidget;
class StartPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onMenuItemClicked(int index);
    void onLoginSuccess(int userId);
    void onLogoutRequested();

private:
    void setupUi();
    void setupConnections();
    void showLoginPage();
    void showMainContent(int userId);

    SideMenu* m_sideMenu;
    QStackedWidget* m_pages;
    StartPage* m_startPage;
    int m_currentUserId;
};

#endif // MAINWINDOW_H
