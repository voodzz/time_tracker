#include "TimerDialog.h"
#include <QApplication>
#include <QDesktopWidget>
//#include <QSound>


// =================================== Constructors & Destructors ===================================
TimerDialog::TimerDialog(int workDuration, int breakDuration, QWidget *parent)
    : QDialog(parent), workDuration(workDuration), breakDuration(breakDuration),
    remainingWorkTime(workDuration * 60 * 25), remainingBreakTime(breakDuration * 60 * 5),
    onWorkPhase(true), isRunning(false)
{
    setWindowTitle("Task Timer");
    workTimer = new QTimer(this);
    breakTimer = new QTimer(this);

    initButtons();
    initLabels();
    initLayouts();
    initConnections();

    minimizeMainWindow();
}

TimerDialog::~TimerDialog()
{
    restoreMainWindow();
}

// ====================================== Init Functions =========================================

void TimerDialog::initButtons()
{
    playPauseButton = new QPushButton("Play", this);
}

void TimerDialog::initLabels()
{
    workTimerLabel = new QLabel("00:00", this);
    breakTimerLabel = new QLabel("00:00", this);
    totalWorkTimeLabel = new QLabel("Total work time: 0 min", this);
    totalBreakTimeLabel = new QLabel("Total break time: 0 min", this);
}

void TimerDialog::initLayouts()
{

    layout = new QVBoxLayout(this);
    layout->addWidget(playPauseButton, 0, Qt::AlignCenter);
    layout->addWidget(workTimerLabel, 0, Qt::AlignCenter);
    layout->addWidget(breakTimerLabel, 0, Qt::AlignCenter);
    layout->addWidget(totalWorkTimeLabel);
    layout->addWidget(totalBreakTimeLabel);

    setLayout(layout);
}

void TimerDialog::initConnections()
{
    connect(playPauseButton, &QPushButton::clicked, this, &TimerDialog::toggleTimer);
    connect(workTimer, &QTimer::timeout, this, &TimerDialog::updateTimer);
}

// ======================================== Slots ================================================
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
    if (onWorkPhase)
    {
        if (remainingWorkTime > 0)
        {
            remainingWorkTime--;
            int minutes = remainingWorkTime / 60;
            int seconds = remainingWorkTime % 60;
            workTimerLabel->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
        }
        else
        {
            // Work phase finished
            onTimerFinished();
        }
    }
    else
    {
        if (remainingBreakTime > 0)
        {
            remainingBreakTime--;
            int minutes = remainingBreakTime / 60;
            int seconds = remainingBreakTime % 60;
            workTimerLabel->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
        }
        else
        {
            // Break phase finished
            onTimerFinished();
        }
    }
}

void TimerDialog::onTimerFinished()
{
    workTimer->stop();
    isRunning = false;

    // Play a sound (optional, you can add sound files)
    // QSound::play("path_to_sound_file");

    if (onWorkPhase)
    {
        // Switch to break phase
        onWorkPhase = false;
        remainingBreakTime = breakDuration * 60;
        playPauseButton->setText("Start Break");
    }
    else
    {
        // Switch to work phase
        onWorkPhase = true;
        remainingWorkTime = workDuration * 60;
        playPauseButton->setText("Start Work");
    }

    updateLabels();

    // Restore the timer window if it was minimized
    showNormal();
    activateWindow();
}

// ======================================= Other Functions ====================================
void TimerDialog::updateLabels()
{
    int totalWorkTime = (workDuration * (remainingWorkTime / 60));
    int totalBreakTime = (breakDuration * ((breakDuration * 60 - remainingBreakTime) / 60));

    totalWorkTimeLabel->setText(QString("Total work time: %1 min").arg(totalWorkTime));
    totalBreakTimeLabel->setText(QString("Total break time: %1 min").arg(totalBreakTime));
}

void TimerDialog::minimizeMainWindow()
{
    QWidget *mainWindow = parentWidget();
    if (mainWindow)
    {
        mainWindow->showMinimized();
    }
}

void TimerDialog::restoreMainWindow()
{
    QWidget *mainWindow = parentWidget();
    if (mainWindow)
    {
        mainWindow->showNormal();
    }
}
