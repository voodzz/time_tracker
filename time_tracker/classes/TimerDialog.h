#ifndef TIMERDIALOG_H
#define TIMERDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>

class TimerDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructors & Destructors
    explicit TimerDialog(int workDuration, int breakDuration, QWidget *parent = nullptr);
    ~TimerDialog();

private slots:
    // Slots
    void toggleTimer();
    void updateTimer();
    void onTimerFinished();

private:

    // Buttons
    QPushButton *playPauseButton;

    // Labels
    QLabel *workTimerLabel;
    QLabel *breakTimerLabel;
    QLabel *totalWorkTimeLabel;
    QLabel *totalBreakTimeLabel;

    // Layouts
    QVBoxLayout *layout;

    // Timers
    QTimer *workTimer;
    QTimer *breakTimer;
    int workDuration;
    int breakDuration;
    int remainingWorkTime;
    int remainingBreakTime;

    // Flags
    bool onWorkPhase;
    bool isRunning;

    // Init Functions
    void initButtons();
    void initLabels();
    void initLayouts();
    void initConnections();

    // Other Functions
    void updateLabels();
    void minimizeMainWindow();
    void restoreMainWindow();
};

#endif // TIMERDIALOG_H
