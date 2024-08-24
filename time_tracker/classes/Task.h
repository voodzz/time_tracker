#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QIcon>
#include <QScrollArea>
#include "TaskSettingsDialog.h"
#include "DeadlineDialog.h"

class Task : public QWidget
{
    Q_OBJECT

public:
    // Constructors & Destructors
    explicit Task(int taskNumber, QWidget *parent = nullptr);

    // Setters
    void setTaskName(const QString &name);
    void setDuration(int duration); // duration should be between 1 and 7
    void setDeadline(const QString &deadline);

signals:
    // Signals
    void taskNameClicked();
    void taskButtonClicked();
    void taskDeleted(Task *task);

private slots:
    // On task clicked functions
    void onTaskNameClicked();

    // Deadline slot
    void showDeadlineDialog();

    // Settings dialog functions
    void openSettingsDialog();
    void applySettings(const QString &name, int duration, const QString& deadline);
    void deleteTask();

private:

    // Variables
    QPushButton *deadlineButton; // Button to left of task name
    QLabel *taskNameLabel; // Name
    QLabel *durationLabel; // Duration

    // Layouts
    QHBoxLayout *mainLayout;

    int taskNumber; // Temporary

    // Init functions
    void initVariables();
    void initLayouts();
    void initConnections();

    // Other functions
    void updateDurationIcons(int duration);
};

#endif // TASK_H
