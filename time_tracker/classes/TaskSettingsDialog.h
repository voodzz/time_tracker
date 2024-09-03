#ifndef TASKSETTINGSDIALOG_H
#define TASKSETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include "TimerDialog.h"

class TaskSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructors & Destructors
    explicit TaskSettingsDialog(int taskDuration, QWidget *parent = nullptr);

    // Setters
    void setTaskName(const QString &name);
    void setDuration(int duration);
    void setDeadline(const QString& deadline);
    void setWorkDuration(int duration);
    void setBreakDuration(int duration);

signals:
    // Signals
    void taskUpdated(const QString &name, int duration, const QString &deadline);
    void taskDeleted();

private slots:
    // Slots
    void applyChanges();
    void deleteTask();
    void showDeadlineDialog();
    void openTimerDialog();

private:
    // Variables
    QLineEdit *nameEdit;
    QSpinBox *durationSpinBox;
    QPushButton *applyButton;
    QPushButton *deleteButton;
    QPushButton *deadlineButton;
    QPushButton *timerStartButton;

    // Layouts
    QGridLayout *layout;

    // Labels
    QLabel *taskNameLabel;
    QLabel *durationLabel;
    QLabel *deadlineLabel;
    QLabel *workTimeTextLabel;
    QLabel *workTimeLabel;
    QLabel *restTimeTextLabel;
    QLabel *restTimeLabel;

    int taskDuration;
    int workDuration;
    int breakDuration;

    // Init Functions
    void initVariables();
    void initLayouts();
    void initLabels();
    void initConnections();
};

#endif // TASKSETTINGSDIALOG_H
