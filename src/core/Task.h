#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QDate>
#include <QVariantMap>

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

    void setTaskName(const QString &name) { m_taskName = name; updateDisplay(); }
    void setDescription(const QString &description) { m_description = description; updateDisplay(); }
    void setDeadline(const QDate &deadline) { m_deadline = deadline; updateDisplay(); }
    void setPlannedCycles(int cycles) { m_plannedCycles = cycles; updateDisplay(); }
    void setRemainingCycles(int cycles) { m_remainingCycles = cycles; updateDisplay(); emit remainingCyclesChanged(cycles); }
    void setStatus(TaskStatus status) { m_status = status; updateDisplay(); }
    void setId(int id) { m_id = id; }
    void updateTask(const QString &name, const QString &description, const QDate &deadline, int plannedCycles);
    void updateCycles(int cycles);
    void updateDisplay();

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
    void taskDeleted(int taskId);
    void taskUpdated(int taskId, const QVariantMap &data);
    void taskCompleted(int taskId, const QVariantMap &taskData);
    void startTimer();
    void remainingCyclesChanged(int cycles);

private slots:
    void openTaskSettings();

private:
    void setupUi();

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
