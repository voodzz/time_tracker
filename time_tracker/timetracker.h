#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include <QWidget>
#include <QTabWidget>
#include <QTabBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QSpacerItem>
#include <QFrame>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QHBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QBoxLayout>
#include <QGridLayout>

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

private slots:
    //Style Slots
    void toggleStyle();

private:
    Ui::TimeTracker *ui;

    // Animations
    QPropertyAnimation *animation;

    // Labels (temporary)
    QLabel *listLabel;
    QLabel *statLabel;
    QLabel *settLabel;

    // Buttons
    QPushButton *listButton;
    QPushButton *statButton;
    QPushButton *settButton;
    QPushButton *styleButton;
    QPushButton *menuButton;

    //Stacked Widgets
    QStackedWidget *stackedWidget;

    //Widgets
    QWidget *sideMenu;
    QWidget *listPage;
    QWidget *statPage;
    QWidget *settPage;


    //Layouts
    QVBoxLayout *menuLayout; //Part of sideMenu
    QGridLayout *listLayout;
    QGridLayout *statLayout;
    QGridLayout *settLayout;


    // Style Regulator
    QString currentStyle;

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
