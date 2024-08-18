#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include <QWidget>
#include <QTabWidget>
#include <QTabBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QSpacerItem>
#include <QFrame>
#include <QPushButton>
#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui {
class TimeTracker;
}
QT_END_NAMESPACE

class TimeTracker : public QWidget
{
    Q_OBJECT

public:
    //Constructors & Destructors
    TimeTracker(QWidget *parent = nullptr);
    ~TimeTracker();

private slots:
    //Style Slots
    void toggleStyle();

private:
    Ui::TimeTracker *ui;

    // Necessary pointers
    QPropertyAnimation *animation;
    QVBoxLayout *menuLayout; //Part of sideMenu

    //Buttons
    QPushButton *option1;
    QPushButton *option2;
    QPushButton *styleButton;

    //Style Regulator
    QString currentStyle;

    // Init Functions
    void initButtons();
    void initMenues();
    void setAnimations();
    void initQVBoxLayout();
    void initConnections();

    //Style Functions
    void applyStyle(const QString &styleFile);


};
#endif // TIMETRACKER_H
