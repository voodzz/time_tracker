#ifndef TIMERDIALOG_H
#define TIMERDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class TimerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimerDialog(int* workDuration, int* breakDuration, QWidget *parent = nullptr);

private slots:
    void toggleTimer();
    void updateTimer();

private:
    QTimer *workTimer;
    QTimer *breakTimer;
    QLabel *workTimerLabel;
    QLabel *breakTimerLabel;
    QPushButton *playPauseButton;
    QVBoxLayout *mainLayout;

    int* workDuration;
    int* breakDuration;
    int remainingWorkTime;
    int remainingBreakTime;
    bool onWorkPhase;
    bool isRunning;

    void initUI();
    void updateLabels();
};

#endif // TIMERDIALOG_H
