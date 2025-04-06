#ifndef DAYCARD_H
#define DAYCARD_H

#include <QWidget>
#include <QDate>
#include <QDateTime>
#include "../pages/HistoryPage.h"  // For TaskInfo structure

class QVBoxLayout;
class QLabel;
class QFrame;
class QScrollArea;

class DayCard : public QWidget {
    Q_OBJECT
public:
    explicit DayCard(const QDate &date, QWidget *parent = nullptr);
    ~DayCard();

    void setTasks(const QList<TaskInfo> &tasks);
    void setDate(const QDate &date);

signals:
    void taskClicked(const TaskInfo &task);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void setupUi();
    void updateDisplay();

    QDate m_date;
    QList<TaskInfo> m_tasks;
    QLabel *m_dayLabel;
    QFrame *m_separator;
    QScrollArea *m_tasksContainer;
    QVBoxLayout *m_tasksLayout;
};

#endif // DAYCARD_H
