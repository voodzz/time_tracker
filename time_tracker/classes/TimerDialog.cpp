#include "TimerDialog.h"
#include <QPainter>
#include <QFont>
#include <QDebug>

TimerDialog::TimerDialog(int workDuration, int breakDuration, int cycles, QWidget *parent)
    : QWidget(parent),
    workDuration(workDuration),
    breakDuration(breakDuration),
    remainingCycles(cycles),
    isWorkPhase(true),
    isRunning(false)
{
    setWindowTitle("Task Timer");
    resize(400, 400);

    // Set initial phase to work phase
    totalSeconds = workDuration * 60;
    remainingSeconds = totalSeconds;

    timer = new QTimer(this);
    timer->setInterval(1000); // 1 second
    connect(timer, &QTimer::timeout, this, &TimerDialog::updateTimer);

    // Initialize UI elements
    phaseLabel = new QLabel("Work Phase", this);
    phaseLabel->setAlignment(Qt::AlignCenter);
    phaseLabel->setStyleSheet("font-size: 20px;");

    playButton = new QPushButton("Start", this);
    playButton->setObjectName("playButton");
    connect(playButton, &QPushButton::clicked, this, &TimerDialog::startTimer);

    pauseSwitchButton = new QPushButton("Pause & Switch", this);
    pauseSwitchButton->setObjectName("pauseSwitchButton");
    connect(pauseSwitchButton, &QPushButton::clicked, this, &TimerDialog::pauseAndSwitch);

    skipButton = new QPushButton("Skip Phase", this);
    skipButton->setObjectName("skipButton");
    connect(skipButton, &QPushButton::clicked, this, &TimerDialog::skipPhase);

    // Layout for buttons
    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(playButton);
    buttonLayout->addWidget(pauseSwitchButton);
    buttonLayout->addWidget(skipButton);

    // Main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(phaseLabel);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

TimerDialog::~TimerDialog()
{
}

void TimerDialog::startTimer()
{
    if (!isRunning) {
        timer->start();
        isRunning = true;
        playButton->setText("Running");
    }
}

void TimerDialog::pauseAndSwitch()
{
    if (isRunning) {
        timer->stop();
        isRunning = false;
        playButton->setText("Resume");
    }
    // Switch phase when Pause button is pressed
    switchPhase();
}

void TimerDialog::skipPhase()
{
    // Immediately switch phase regardless of remaining time
    if (isRunning) {
        timer->stop();
        isRunning = false;
        playButton->setText("Resume");
    }
    switchPhase();
}

void TimerDialog::updateTimer()
{
    if (remainingSeconds > 0) {
        --remainingSeconds;
    } else {
        // Time's up: automatically switch phase
        switchPhase();
    }
    update(); // trigger repaint of circular indicator
}

void TimerDialog::switchPhase()
{
    if (isWorkPhase) {
        // End of work phase: emit signal to indicate one cycle is completed
        emit cycleCompleted();
        // Decrement remaining cycles (star removed)
        --remainingCycles;
        // If no cycles left, stop timer and exit
        if (remainingCycles <= 0) {
            timer->stop();
            isRunning = false;
            phaseLabel->setText("Completed");
            playButton->setEnabled(false);
            pauseSwitchButton->setEnabled(false);
            skipButton->setEnabled(false);
            remainingSeconds = 0;
            update();
            return;
        }
        // Switch to break phase
        isWorkPhase = false;
        phaseLabel->setText("Break Phase");
        totalSeconds = breakDuration * 60;
    } else {
        // Switch back to work phase
        isWorkPhase = true;
        phaseLabel->setText("Work Phase");
        totalSeconds = workDuration * 60;
    }
    remainingSeconds = totalSeconds;
    update();
    // Optionally, auto-restart timer after phase switch:
    // Uncomment следующую строку, если требуется автостарт новой фазы:
    // startTimer();
}

void TimerDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Define the drawing area (a square in the center)
    int side = qMin(width(), height()) - 80;
    QRect rect((width() - side) / 2, (height() - side) / 2 - 30, side, side);

    // Draw background circle
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(230, 230, 230));
    painter.drawEllipse(rect);

    // Calculate angle for the remaining time
    double progress = (totalSeconds > 0) ? static_cast<double>(remainingSeconds) / totalSeconds : 0;
    int spanAngle = static_cast<int>(360 * progress);

    // Draw progress pie
    painter.setBrush(QColor(0, 150, 0));
    // Start at top (90°) and move counter-clockwise
    painter.drawPie(rect, 90 * 16, -spanAngle * 16);

    // Draw the remaining time as text in the center
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    QString timeText = QString("%1:%2")
                           .arg(minutes, 2, 10, QChar('0'))
                           .arg(seconds, 2, 10, QChar('0'));

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 24, QFont::Bold));
    painter.drawText(rect, Qt::AlignCenter, timeText);
}
