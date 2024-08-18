#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include <QWidget>
#include <QTabWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TimeTracker;
}
QT_END_NAMESPACE

class TimeTracker : public QWidget
{
    Q_OBJECT

public:
    TimeTracker(QWidget *parent = nullptr);
    ~TimeTracker();

private:
    Ui::TimeTracker *ui;
};
#endif // TIMETRACKER_H
