#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SideMenu.h"

class QStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onMenuItemClicked(int index);

private:
    void setupUi();
    void setupConnections();

    SideMenu* m_sideMenu;
    QStackedWidget* m_pages;
};

#endif // MAINWINDOW_H
