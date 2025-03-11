#ifndef TIMERDIALOG_H
#define TIMERDIALOG_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class TimerDialog : public QWidget
{
    Q_OBJECT

public:
    // Constructor accepts work and break durations (in minutes) and number of cycles
    explicit TimerDialog(int workDuration, int breakDuration, int cycles, QWidget *parent = nullptr);
    ~TimerDialog();

signals:
    // Emitted when a work cycle is completed (to notify Task to remove one star)
    void cycleCompleted();

protected:
    // Override paintEvent to draw the circular progress indicator
    void paintEvent(QPaintEvent *event) override;

private slots:
    // Slot to start/resume the timer
    void startTimer();
    // Slot to pause the timer and switch phase
    void pauseAndSwitch();
    // Slot to immediately skip the current phase
    void skipPhase();
    // Slot called every second to update timer
    void updateTimer();

private:
    QTimer *timer; // Timer with 1-sec interval
    QPushButton *playButton;         // "Start" / "Resume" button
    QPushButton *pauseSwitchButton;    // "Pause & Switch" button
    QPushButton *skipButton;           // "Skip Phase" button
    QLabel *phaseLabel;                // Label to display current phase ("Work Phase" / "Break Phase")
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;

    int workDuration;   // work duration in minutes
    int breakDuration;  // break duration in minutes
    int totalSeconds;   // total seconds for the current phase
    int remainingSeconds; // remaining seconds in the current phase
    int remainingCycles; // remaining work cycles (each cycle = one work phase)

    bool isWorkPhase; // true if current phase is work, false if break
    bool isRunning;   // true if timer is currently running

    // Switches the phase (work <-> break) and resets timers accordingly
    void switchPhase();
};

#endif // TIMERDIALOG_H
