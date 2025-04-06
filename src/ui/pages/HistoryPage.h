#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QWidget>
#include <QDate>
#include <QDateTime>
#include <QMap>

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLabel;
class QFrame;
class DayCard;

// Structure to hold task information
struct TaskInfo {
    int taskId;
    QString taskName;
    int cycles;
    QString status;
    QString priority;
    QString description;
    QDate deadline;
    QDateTime completedAt;
    QString details;
};

// Main history page widget
class HistoryPage : public QWidget {
    Q_OBJECT
public:
    explicit HistoryPage(int userId, QWidget *parent = nullptr);
    ~HistoryPage();

private slots:
    // Navigation
    void shiftLeft();
    void shiftRight();
    void chooseDay();
    
    // Task interaction
    void onTaskClicked(const TaskInfo &task);
    void onTaskUpdated(const QVariantMap &taskData);

private:
    void setupUi();
    void loadHistory();
    void updateDayCards();
    QString formatTaskDetails(const TaskInfo &task) const;

    int m_userId;
    QDate m_baseDate;
    int m_firstCardIndex;

    // Layout components
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_dayCardsLayout;

    // Navigation buttons
    QPushButton *m_leftButton;
    QPushButton *m_rightButton;
    QPushButton *m_chooseDayButton;

    // Day cards
    QList<DayCard*> m_dayCards;

    // Task details display
    QFrame *m_detailsFrame;
    QLabel *m_detailsLabel;

    // History data storage
    QMap<QDate, QList<TaskInfo>> m_historyData;
};

#endif // HISTORYPAGE_H
