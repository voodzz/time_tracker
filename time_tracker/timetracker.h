#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include "classes/Task.h"
#include "classes/DeadlineDialog.h"

#include <QTabWidget>
#include <QTabBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QSpacerItem>
#include <QFrame>
#include <QFile>
#include <QTextStream>
#include <QStackedWidget>
#include <QBoxLayout>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollBar>


QT_BEGIN_NAMESPACE
namespace Ui {
class TimeTracker;
}
QT_END_NAMESPACE

class TimeTracker : public QWidget
{
    Q_OBJECT

public:
    // Constructors & Destructors
    TimeTracker(QWidget *parent = nullptr);
    ~TimeTracker();

protected:
    // Events
    void resizeEvent(QResizeEvent *event) override;

private slots:

    // Task slots
    void addTask();

    // Style slots
    void toggleStyle();

    // Animation slots
    void performMenuAnimations();
private:
    Ui::TimeTracker *ui;

    bool isMenuOpen = false;

    // Animations
    QPropertyAnimation *menuAnimation;
    QPropertyAnimation *widgetGeometryAnimation;

    // Labels (temporary)
    QLabel *listLabel;
    QLabel *statLabel;

    // Scroll areas
    QScrollArea *scrollArea;

    // Labels (NOT temporary)
    QLabel *themeLabel;
    QLabel *currentThemeLabel;

    // Buttons
    QPushButton *listButton;
    QPushButton *statButton;
    QPushButton *settButton;
    QPushButton *styleButton;
    QPushButton *menuButton;

    QPushButton *leftArrowThemeSettingButton;
    QPushButton *rightArrowThemeSettingButton;

    QPushButton *addTaskButton;

    //Stacked Widgets
    QStackedWidget *stackedWidget;

    //Widgets
    QWidget *sideMenu;
    QWidget *listPage;
    QWidget *statPage;
    QWidget *settPage;
    QWidget *taskContainer;


    //Layouts
    QGridLayout *taskLayout;
    QVBoxLayout *menuLayout; //Part of sideMenu
    QGridLayout *listLayout;
    QGridLayout *statLayout;
    QGridLayout *settLayout;
    QHBoxLayout *themeSettingLayout;


    // Style Regulator
    QString currentStyle;

    int taskCount;

    // Init Functions
    void initButtons();
    void initWidgets();
    void setAnimations();
    void initLayouts();
    void initLabels();
    void initConnections();

    // Style Functions
    void applyStyle(const QString &styleFile);
    void setStyleBlack();
    void setStyleWhite();

    // Additional Functions
    void enableButtons();
    void disableButtons();
};
#endif // TIMETRACKER_H
