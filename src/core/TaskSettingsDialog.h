#ifndef TASKSETTINGSDIALOG_H
#define TASKSETTINGSDIALOG_H

#include <QDialog>
#include <QDate>

class QLineEdit;
class QTextEdit;
class QSpinBox;
class QDateEdit;
class QPushButton;

class TaskSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TaskSettingsDialog(const QString &name,
                                const QString &description,
                                const QDate &deadline,
                                int cycles,
                                QWidget *parent = nullptr);
    ~TaskSettingsDialog();

    QString taskName() const;
    QString taskDescription() const;
    QDate taskDeadline() const;
    int taskCycles() const;

signals:
    void taskDeleted();

private slots:
    void onApply();
    void onDelete();

private:
    void setupUi();

    QLineEdit *m_nameEdit;
    QTextEdit *m_descriptionEdit;
    QDateEdit *m_deadlineEdit;
    QSpinBox *m_cyclesSpinBox;
    QPushButton *m_applyButton;
    QPushButton *m_deleteButton;
};

#endif // TASKSETTINGSDIALOG_H
