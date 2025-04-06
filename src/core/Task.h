#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QDate>

enum class TaskStatus {
    Active,
    Completed,
    Cancelled
};

class QPushButton;
class QLabel;
class QHBoxLayout;

class Task : public QWidget
{
    Q_OBJECT
public:
    explicit Task(int id, QWidget *parent = nullptr);
    ~Task();

    int id() const { return m_id; }
    QString taskName() const { return m_taskName; }
    QString description() const { return m_description; }
    QDate deadline() const { return m_deadline; }
    int plannedCycles() const { return m_plannedCycles; }
    int remainingCycles() const { return m_remainingCycles; }
    TaskStatus status() const { return m_status; }

    void updateTask(const QString &name, const QString &description, const QDate &deadline, int plannedCycles);
    void updateCycles(int cycles);
    void setId(int id) { m_id = id; };

    static QString toString(TaskStatus status) {
        switch (status) {
        case TaskStatus::Active:
            return "Active";
        case TaskStatus::Completed:
            return "Completed";
        case TaskStatus::Cancelled:
            return "Cancelled";
        default:
            return "Unknown";
        }
    }

signals:
    void taskUpdated(Task* task);
    void taskDeleted(Task* task);
    void startTimer(Task* task);
    void statusChanged(TaskStatus status);
    void updateRequested(Task* task, const QString &name, const QString &description, const QDate &deadline, int plannedCycles);


private slots:
    void openTaskSettings();

private:
    void setupUi();
    void updateDisplay();

    int m_id;
    QString m_taskName;
    QString m_description;
    QDate m_deadline;
    int m_plannedCycles;
    int m_remainingCycles;
    TaskStatus m_status;

    QPushButton *tt_startButton;
    QPushButton *tt_nameButton;
    QLabel *tt_deadlineLabel;
    QLabel *tt_cyclesLabel;
    QLabel *tt_descriptionLabel;
    QHBoxLayout *m_layout;
};

#endif // TASK_H
