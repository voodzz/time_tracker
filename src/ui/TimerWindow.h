#ifndef TIMERWINDOW_H
#define TIMERWINDOW_H

#include <QWidget>
#include <QTimer>

class QPushButton;
class QLabel;

class TimerWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TimerWindow(QWidget *parent = nullptr, int plannedCycles = 3);
    ~TimerWindow();

    void startTimer();
    void pauseTimer();
    void skipPhase();

signals:
    void phaseCompleted();
    void workEnd();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateTimer();

private:
    void setupUi();
    void updateDisplay();
    void switchPhase();

    QTimer *m_timer;
    int m_totalSeconds;
    int m_remainingSeconds;

    // Pomodoro durations in seconds
    int m_workDuration;
    int m_breakDuration;
    int m_completedCycles;
    int m_plannedCycles;

    bool m_isWorkPhase;
    bool m_isRunning;

    // UI элементы с префиксом tt_
    QPushButton *tt_playButton;
    QPushButton *tt_pauseButton;
    QPushButton *tt_skipButton;
    QLabel *tt_timeLabel;
};

#endif // TIMERWINDOW_H
