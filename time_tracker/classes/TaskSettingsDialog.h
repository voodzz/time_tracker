#ifndef TASKSETTINGSDIALOG_H
#define TASKSETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class TaskSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructors & Destructors
    explicit TaskSettingsDialog(QWidget *parent = nullptr);

    // Setters
    void setTaskName(const QString &name);
    void setDuration(int duration);

signals:
    // Signals
    void taskUpdated(const QString &name, int duration);
    void taskDeleted();

private slots:
    // Slots
    void applyChanges();
    void deleteTask();

private:
    // Variables
    QLineEdit *nameEdit;
    QSpinBox *durationSpinBox;
    QPushButton *applyButton;
    QPushButton *deleteButton;

    // Layouts
    QGridLayout *layout;

    // Labels
    QLabel *taskNameLabel;
    QLabel *durationLabel;
    QLabel *workTimeTextLabel;
    QLabel *workTimeLabel;
    QLabel *restTimeTextLabel;
    QLabel *restTimeLabel;

    // Init Functions
    void initVariables();
    void initLayouts();
    void initLabels();
    void initConnections();
};

#endif // TASKSETTINGSDIALOG_H
