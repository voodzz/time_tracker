#ifndef TASKSETTINGSDIALOG_H
#define TASKSETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include "TimerDialog.h"

class TaskSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskSettingsDialog(int taskDuration, QWidget *parent = nullptr);
    ~TaskSettingsDialog();

    void setTaskName(const QString &name);
    void setDuration(int duration);
    void setDeadline(const QString &deadline);
    void setWorkDuration(int duration);
    void setBreakDuration(int duration);

signals:
    // Emitted when settings are applied; Task listens to update its values (including removing a star)
    void taskUpdated(const QString &name, int duration, const QString &deadline);
    // Emitted when a task should be deleted
    void taskDeleted();

private slots:
    void applyChanges();
    void deleteTask();
    void showDeadlineDialog();
    void openTimerDialog();

private:
    // UI elements for task settings
    QLineEdit *nameEdit;
    QSpinBox *durationSpinBox;
    QPushButton *applyButton;
    QPushButton *deleteButton;
    QPushButton *deadlineButton;
    QPushButton *timerStartButton;

    // Labels for displaying additional info
    QLabel *taskNameLabel;
    QLabel *durationLabel;
    QLabel *deadlineLabel;
    QLabel *workTimeTextLabel;
    QLabel *workTimeLabel;
    QLabel *restTimeTextLabel;
    QLabel *restTimeLabel;

    QGridLayout *layout;

    // Task parameters
    int taskDuration;  // Number of cycles (stars)
    int workDuration;  // Work duration in minutes
    int breakDuration; // Break duration in minutes

    void initVariables();
    void initLabels();
    void initLayouts();
    void initConnections();
};

#endif // TASKSETTINGSDIALOG_H
