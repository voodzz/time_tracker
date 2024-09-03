#include "TimerDialog.h"

TimerDialog::TimerDialog(int* workDuration, int* breakDuration, QWidget *parent)
    : QDialog(parent), workDuration(workDuration), breakDuration(breakDuration),
    remainingWorkTime(*workDuration * 60), remainingBreakTime(*breakDuration * 60),
    onWorkPhase(false), isRunning(false)
{
    setWindowTitle("Task Timer");

    workTimer = new QTimer(this);
    connect(workTimer, &QTimer::timeout, this, &TimerDialog::updateTimer);
    breakTimer = new QTimer(this);
    connect(breakTimer, &QTimer::timeout, this, &TimerDialog::updateTimer);

    initUI();
    updateLabels();
}

void TimerDialog::initUI()
{
    workTimerLabel = new QLabel(this);
    breakTimerLabel = new QLabel(this);

    playPauseButton = new QPushButton("Play", this);

    connect(playPauseButton, &QPushButton::clicked, this, &TimerDialog::toggleTimer);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(workTimerLabel);
    mainLayout->addWidget(breakTimerLabel);
    mainLayout->addWidget(playPauseButton);

    setLayout(mainLayout);
}

void TimerDialog::toggleTimer()
{
    if (isRunning)
    {
        breakTimer->start(1000); // Update every second
        workTimer->stop();
        playPauseButton->setText("Play");
        isRunning = false;
    }
    else
    {
        breakTimer->stop();
        workTimer->start(1000); // Update every second
        playPauseButton->setText("Pause");
        isRunning = true;
    }
}


void TimerDialog::updateTimer()
{
    if (isRunning) {
        if (remainingWorkTime > 0) {
            remainingWorkTime--;
        } else {
            remainingBreakTime = *breakDuration * 60;
        }
    } else {
        if (remainingBreakTime > 0) {
            remainingBreakTime--;
        } else {
            remainingWorkTime = *workDuration * 60;
        }
    }

    updateLabels();
}

void TimerDialog::updateLabels()
{
    workTimerLabel->setText(QString("Work: %1:%2").arg(remainingWorkTime / 60, 2, 10, QChar('0')).arg(remainingWorkTime % 60, 2, 10, QChar('0')));
    breakTimerLabel->setText(QString("Break: %1:%2").arg(remainingBreakTime / 60, 2, 10, QChar('0')).arg(remainingBreakTime % 60, 2, 10, QChar('0')));
}
