#ifndef POMODORO_TIMER_H
#define POMODORO_TIMER_H

#include <QWidget>
#include <QTimer>

class QPushButton;
class QLabel;

enum class PomodoroPhase {
    Work,           // 25 minutes
    ShortBreak,     // 5 minutes
    LongBreak       // 20 minutes
};

class PomodoroTimer : public QWidget
{
    Q_OBJECT
public:
    explicit PomodoroTimer(QWidget *parent = nullptr);
    ~PomodoroTimer();

signals:
    void phaseCompleted(PomodoroPhase phase);
    void cycleCompleted(); // Emitted after 4 work phases

public slots:
    void startTimer();
    void pauseTimer();
    void resetTimer();
    void skipPhase();
    void setActiveTaskName(const QString &name);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    static const int WORK_DURATION = 25 * 60;        // 25 minutes in seconds
    static const int SHORT_BREAK_DURATION = 5 * 60;  // 5 minutes in seconds
    static const int LONG_BREAK_DURATION = 20 * 60;  // 20 minutes in seconds
    static const int POMODOROS_BEFORE_LONG_BREAK = 4;

    void setupUi();
    void updateTimerDisplay();
    void updateProgressCircle();
    void startNextPhase();
    QString formatTime(int seconds) const;
    void updateButtonStates();

    QTimer m_timer;
    PomodoroPhase m_currentPhase;
    int m_remainingSeconds;
    int m_completedPomodoros;
    bool m_isRunning;
    QString m_activeTaskName;

    // UI elements
    QPushButton *m_startPauseButton;
    QPushButton *m_resetButton;
    QPushButton *m_skipButton;
    QLabel *m_timeLabel;
    QLabel *m_phaseLabel;
    QLabel *m_cycleLabel;
    QLabel *m_activeTaskNameLabel;

    // Circle progress parameters
    int m_radius;
    QPoint m_center;
    int m_currentPhaseDuration;
};

#endif // POMODORO_TIMER_H
