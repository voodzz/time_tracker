#include "PomodoroTimer.h"
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QDebug>

PomodoroTimer::PomodoroTimer(QWidget *parent)
    : QWidget(parent)
    , m_currentPhase(PomodoroPhase::Work)
    , m_remainingSeconds(WORK_DURATION)
    , m_completedPomodoros(0)
    , m_isRunning(false)
    , m_activeTaskName("")
    , m_currentPhaseDuration(WORK_DURATION)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setObjectName("pomodoroTimer");
    
    setupUi();
    
    connect(&m_timer, &QTimer::timeout, this, [this]() {
        if (m_remainingSeconds > 0) {
            m_remainingSeconds--;
            updateTimerDisplay();
            update(); // Trigger repaint for circle
        } else {
            m_timer.stop();
            m_isRunning = false;
            
            emit phaseCompleted(m_currentPhase);
            
            if (m_currentPhase == PomodoroPhase::Work) {
                m_completedPomodoros++;
                if (m_completedPomodoros % POMODOROS_BEFORE_LONG_BREAK == 0) {
                    emit cycleCompleted();
                }
            }
            
            startNextPhase();
        }
    });
}

PomodoroTimer::~PomodoroTimer() {}

void PomodoroTimer::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Active task label
    QLabel *activeTaskLabel = new QLabel(tr("Current Task:"), this);
    activeTaskLabel->setObjectName("activeTaskLabel");
    activeTaskLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(activeTaskLabel);
    
    m_activeTaskNameLabel = new QLabel(this);
    m_activeTaskNameLabel->setObjectName("activeTaskNameLabel");
    m_activeTaskNameLabel->setAlignment(Qt::AlignCenter);
    m_activeTaskNameLabel->setWordWrap(true);
    mainLayout->addWidget(m_activeTaskNameLabel);
    
    // Phase label
    m_phaseLabel = new QLabel(this);
    m_phaseLabel->setObjectName("phaseLabel");
    m_phaseLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_phaseLabel);
    
    // Time label
    m_timeLabel = new QLabel(this);
    m_timeLabel->setObjectName("timeLabel");
    m_timeLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_timeLabel);
    
    // Cycle label
    m_cycleLabel = new QLabel(this);
    m_cycleLabel->setObjectName("cycleLabel");
    m_cycleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_cycleLabel);
    
    // Add spacer to push buttons to bottom
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(10);
    
    m_startPauseButton = new QPushButton(tr("Start"), this);
    m_startPauseButton->setObjectName("startPauseButton");
    m_startPauseButton->setFixedWidth(80);
    
    m_resetButton = new QPushButton(tr("Reset"), this);
    m_resetButton->setObjectName("resetButton");
    m_resetButton->setFixedWidth(80);
    
    m_skipButton = new QPushButton(tr("Skip"), this);
    m_skipButton->setObjectName("skipButton");
    m_skipButton->setFixedWidth(80);
    
    buttonLayout->addWidget(m_startPauseButton);
    buttonLayout->addWidget(m_resetButton);
    buttonLayout->addWidget(m_skipButton);
    
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(20);  // Add space below buttons
    
    // Connect buttons
    connect(m_startPauseButton, &QPushButton::clicked, this, [this]() {
        if (m_isRunning) {
            pauseTimer();
        } else {
            startTimer();
        }
    });
    
    connect(m_resetButton, &QPushButton::clicked, this, &PomodoroTimer::resetTimer);
    connect(m_skipButton, &QPushButton::clicked, this, &PomodoroTimer::skipPhase);
    
    // Initial update
    updateTimerDisplay();
    updateButtonStates();
}

void PomodoroTimer::setActiveTaskName(const QString &name)
{
    m_activeTaskName = name;
    m_activeTaskNameLabel->setText(name.isEmpty() ? tr("No task selected") : name);
}

void PomodoroTimer::startTimer()
{
    m_isRunning = true;
    m_timer.start(1000); // Update every second
    m_startPauseButton->setText(tr("Pause"));
    updateButtonStates();
}

void PomodoroTimer::pauseTimer()
{
    m_isRunning = false;
    m_timer.stop();
    m_startPauseButton->setText(tr("Resume"));
    updateButtonStates();
}

void PomodoroTimer::resetTimer()
{
    m_timer.stop();
    m_isRunning = false;
    m_currentPhase = PomodoroPhase::Work;
    m_remainingSeconds = WORK_DURATION;
    m_currentPhaseDuration = WORK_DURATION;
    m_completedPomodoros = 0;
    updateTimerDisplay();
    updateButtonStates();
    update();
}

void PomodoroTimer::skipPhase()
{
    m_timer.stop();
    m_isRunning = false;
    
    if (m_currentPhase == PomodoroPhase::Work) {
        m_completedPomodoros++;
        emit phaseCompleted(m_currentPhase);
        if (m_completedPomodoros % POMODOROS_BEFORE_LONG_BREAK == 0) {
            emit cycleCompleted();
        }
    }
    
    startNextPhase();
}

void PomodoroTimer::startNextPhase()
{
    switch (m_currentPhase) {
        case PomodoroPhase::Work:
            m_currentPhase = (m_completedPomodoros % POMODOROS_BEFORE_LONG_BREAK == 0) 
                ? PomodoroPhase::LongBreak 
                : PomodoroPhase::ShortBreak;
            m_currentPhaseDuration = (m_currentPhase == PomodoroPhase::LongBreak) 
                ? LONG_BREAK_DURATION 
                : SHORT_BREAK_DURATION;
            break;
            
        case PomodoroPhase::ShortBreak:
        case PomodoroPhase::LongBreak:
            m_currentPhase = PomodoroPhase::Work;
            m_currentPhaseDuration = WORK_DURATION;
            break;
    }
    
    m_remainingSeconds = m_currentPhaseDuration;
    updateTimerDisplay();
    updateButtonStates();
    update();
}

void PomodoroTimer::updateTimerDisplay()
{
    m_timeLabel->setText(formatTime(m_remainingSeconds));
    
    QString phaseText;
    switch (m_currentPhase) {
        case PomodoroPhase::Work:
            phaseText = tr("Work Time");
            break;
        case PomodoroPhase::ShortBreak:
            phaseText = tr("Short Break");
            break;
        case PomodoroPhase::LongBreak:
            phaseText = tr("Long Break");
            break;
    }
    m_phaseLabel->setText(phaseText);
    
    m_cycleLabel->setText(tr("Pomodoro: %1/4").arg(m_completedPomodoros % POMODOROS_BEFORE_LONG_BREAK + 1));
}

void PomodoroTimer::updateButtonStates()
{
    m_resetButton->setEnabled(!m_isRunning || m_remainingSeconds < m_currentPhaseDuration);
    m_skipButton->setEnabled(!m_isRunning);
}

QString PomodoroTimer::formatTime(int seconds) const
{
    int minutes = seconds / 60;
    seconds = seconds % 60;
    return QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

void PomodoroTimer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Calculate circle parameters
    int size = qMin(width(), height()) - 40; // Leave some margin
    m_radius = size / 2;
    m_center = rect().center();
    
    // Draw background circle
    QPen pen(QColor(200, 200, 200));
    pen.setWidth(10);
    painter.setPen(pen);
    painter.drawEllipse(m_center, m_radius, m_radius);
    
    // Draw progress arc
    if (m_currentPhaseDuration > 0) {
        QColor progressColor;
        switch (m_currentPhase) {
            case PomodoroPhase::Work:
                progressColor = QColor(46, 204, 113); // Green
                break;
            case PomodoroPhase::ShortBreak:
                progressColor = QColor(52, 152, 219); // Blue
                break;
            case PomodoroPhase::LongBreak:
                progressColor = QColor(155, 89, 182); // Purple
                break;
        }
        
        pen.setColor(progressColor);
        painter.setPen(pen);
        
        qreal progress = static_cast<qreal>(m_remainingSeconds) / m_currentPhaseDuration;
        int startAngle = 90 * 16; // Start from top (90 degrees * 16 for QPainter angle format)
        int spanAngle = -progress * 360 * 16; // Negative for clockwise, multiply by 16 for QPainter
        
        painter.drawArc(m_center.x() - m_radius, m_center.y() - m_radius,
                       size, size, startAngle, spanAngle);
    }
}

void PomodoroTimer::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    update();
}
