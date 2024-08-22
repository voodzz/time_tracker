#ifndef TASKSETTINGSDIALOG_H
#define TASKSETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

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
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonsLayout;

    // Init Functions
    void initVariables();
    void initLayouts();
    void initConnections();
};

#endif // TASKSETTINGSDIALOG_H
