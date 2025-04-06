#include "TimerWindow.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QFont>
#include <QDebug>

TimerWindow::TimerWindow(QWidget *parent, int plannedCycles)
    : QWidget(parent),
      m_timer(new QTimer(this)),
      m_workDuration(25 * 60),
      m_breakDuration(5 * 60),
      m_completedCycles(0),
      m_plannedCycles(plannedCycles),
      m_isWorkPhase(true),
      m_isRunning(false)
{
    m_totalSeconds = m_workDuration;
    m_remainingSeconds = m_totalSeconds;

    setupUi();

    connect(m_timer, &QTimer::timeout, this, &TimerWindow::updateTimer);
}

TimerWindow::~TimerWindow() { }

void TimerWindow::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    tt_timeLabel = new QLabel(this);
    tt_timeLabel->setObjectName("tt_timeLabel");
    tt_timeLabel->setAlignment(Qt::AlignCenter);
    tt_timeLabel->setStyleSheet("font-size: 36px;");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    tt_playButton = new QPushButton(tr("Play"), this);
    tt_playButton->setObjectName("tt_playButton");
    tt_pauseButton = new QPushButton(tr("Pause"), this);
    tt_pauseButton->setObjectName("tt_pauseButton");
    tt_skipButton = new QPushButton(tr("Skip Phase"), this);
    tt_skipButton->setObjectName("tt_skipButton");

    buttonLayout->addWidget(tt_playButton);
    buttonLayout->addWidget(tt_pauseButton);
    buttonLayout->addWidget(tt_skipButton);

    mainLayout->addWidget(tt_timeLabel);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(tt_playButton, &QPushButton::clicked, this, &TimerWindow::startTimer);
    connect(tt_pauseButton, &QPushButton::clicked, this, &TimerWindow::pauseTimer);
    connect(tt_skipButton, &QPushButton::clicked, this, &TimerWindow::skipPhase);

    updateDisplay();
}

void TimerWindow::startTimer()
{
    if (!m_isRunning) {
        m_timer->start(1000);
        m_isRunning = true;
    }
}

void TimerWindow::pauseTimer()
{
    if (m_isRunning) {
        m_timer->stop();
        m_isRunning = false;
    }
}

void TimerWindow::skipPhase()
{
    m_timer->stop();
    m_isRunning = false;
    switchPhase();
}

void TimerWindow::updateTimer()
{
    if (m_remainingSeconds > 0) {
        m_remainingSeconds--;
        updateDisplay();
        update();
    } else {
        switchPhase();
    }
}

void TimerWindow::switchPhase()
{
    if (m_isWorkPhase) {
        m_completedCycles++;
        emit phaseCompleted();
        // После каждых 4 циклов длительный перерыв
        if (m_completedCycles % 4 == 0)
            m_totalSeconds = 15 * 60; // долгий перерыв
        else
            m_totalSeconds = m_breakDuration;
        m_isWorkPhase = false;
    } else {
        m_totalSeconds = m_workDuration;
        m_isWorkPhase = true;
    }
    m_remainingSeconds = m_totalSeconds;
    if (m_completedCycles == m_plannedCycles) {
        emit workEnd();
    }
    updateDisplay();
    update();
}

void TimerWindow::updateDisplay()
{
    int minutes = m_remainingSeconds / 60;
    int seconds = m_remainingSeconds % 60;
    tt_timeLabel->setText(QString("%1:%2")
                          .arg(minutes, 2, 10, QChar('0'))
                          .arg(seconds, 2, 10, QChar('0')));
}

void TimerWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Получаем значения динамических свойств, установленные через QSS, или используем значения по умолчанию
    QColor arcColor = property("arcColor").isValid() ?
                      property("arcColor").value<QColor>() : QColor(Qt::green);
    int lineWidth = property("arcLineWidth").isValid() ?
                    property("arcLineWidth").toInt() : 10;

    QPen pen(arcColor, lineWidth);
    painter.setPen(pen);

    int side = qMin(width(), height()) - 100;
    QRect rect((width() - side) / 2, (height() - side) / 2 - 50, side, side);

    // Вычисляем угол по пропорции оставшегося времени
    int angleSpan = static_cast<int>(360 * (static_cast<double>(m_remainingSeconds) / m_totalSeconds));
    painter.drawArc(rect, 90 * 16, -angleSpan * 16);
}