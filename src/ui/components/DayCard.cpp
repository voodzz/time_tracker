#include "DayCard.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QDebug>
#include <QApplication>
#include <QStyle>

DayCard::DayCard(const QDate &date, QWidget *parent)
    : QWidget(parent), m_date(date)
{
    setupUi();
    updateDisplay();
}

DayCard::~DayCard() { }

void DayCard::setupUi()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);

    // Set fixed width and expanding height
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    setMinimumWidth(50);  // Minimum width for the card
    setMaximumWidth(300);  // Maximum width to maintain readability

    // Card styling
    setObjectName("dayCard");
    setProperty("class", "dayCard");

    // Day label setup
    m_dayLabel = new QLabel(this);
    m_dayLabel->setAlignment(Qt::AlignCenter);
    m_dayLabel->setObjectName("dayLabel");
    m_dayLabel->setProperty("class", "dayLabel");
    m_dayLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    m_dayLabel->setWordWrap(true);

    // Separator setup
    m_separator = new QFrame(this);
    m_separator->setFrameShape(QFrame::HLine);
    m_separator->setObjectName("daySeparator");
    m_separator->setFixedHeight(2);

    // Tasks container setup
    m_tasksContainer = new QScrollArea(this);
    m_tasksContainer->setObjectName("tasksContainer");
    m_tasksContainer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tasksContainer->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tasksContainer->setFrameShape(QFrame::NoFrame);

    QWidget* tasksWidget = new QWidget(m_tasksContainer);
    tasksWidget->setObjectName("tasksWidget");
    
    m_tasksLayout = new QVBoxLayout(tasksWidget);
    m_tasksLayout->setSpacing(8);
    m_tasksLayout->setContentsMargins(5, 5, 5, 5);
    m_tasksLayout->setAlignment(Qt::AlignTop);
    
    tasksWidget->setLayout(m_tasksLayout);
    m_tasksContainer->setWidget(tasksWidget);
    m_tasksContainer->setWidgetResizable(true);

    layout->addWidget(m_dayLabel);
    layout->addWidget(m_separator);
    layout->addWidget(m_tasksContainer, 1);
    setLayout(layout);
}

void DayCard::updateDisplay()
{
    if (!m_date.isValid()) {
        qDebug() << "Warning: Invalid date in DayCard::updateDisplay";
        m_dayLabel->setText("Invalid Date");
        return;
    }

    m_dayLabel->setText(m_date.toString("dddd\nMMM d, yyyy"));

    // Clear existing tasks
    QLayoutItem *child;
    while ((child = m_tasksLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    // Add new tasks
    for (const TaskInfo &task : m_tasks) {
        QFrame* taskFrame = new QFrame(this);
        taskFrame->setObjectName("taskFrame");
        taskFrame->setProperty("class", "taskFrame");
        taskFrame->setFrameShape(QFrame::StyledPanel);
        taskFrame->setCursor(Qt::PointingHandCursor);

        QVBoxLayout* taskLayout = new QVBoxLayout(taskFrame);
        taskLayout->setSpacing(5);
        taskLayout->setContentsMargins(8, 8, 8, 8);

        QLabel* nameLabel = new QLabel(task.taskName, taskFrame);
        nameLabel->setStyleSheet("font-weight: bold;");
        nameLabel->setWordWrap(true);

        QLabel* statusLabel = new QLabel(task.status, taskFrame);
        statusLabel->setStyleSheet("color: gray;");

        // Cycles display with icon
        QHBoxLayout* cyclesLayout = new QHBoxLayout();
        QLabel* cyclesIcon = new QLabel(taskFrame);
        cyclesIcon->setFixedSize(16, 16);
        cyclesIcon->setPixmap(QApplication::style()->standardPixmap(QStyle::SP_BrowserReload).scaled(16, 16));
        QLabel* cyclesLabel = new QLabel(QString::number(task.cycles), taskFrame);
        cyclesLayout->addWidget(cyclesIcon);
        cyclesLayout->addWidget(cyclesLabel);
        cyclesLayout->addStretch();

        taskLayout->addWidget(nameLabel);
        taskLayout->addWidget(statusLabel);
        taskLayout->addLayout(cyclesLayout);

        m_tasksLayout->addWidget(taskFrame);
        taskFrame->installEventFilter(this);
    }

    // Add stretch at the end
    m_tasksLayout->addStretch();
}

void DayCard::setTasks(const QList<TaskInfo> &tasks)
{
    m_tasks = tasks;
    updateDisplay();
}

void DayCard::setDate(const QDate &date)
{
    if (date.isValid()) {
        m_date = date;
        updateDisplay();
    } else {
        qDebug() << "Warning: Attempt to set invalid date in DayCard";
    }
}

bool DayCard::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QFrame *frame = qobject_cast<QFrame*>(obj);
        if (frame && frame->objectName() == "taskFrame") {
            int index = m_tasksLayout->indexOf(frame);
            if (index >= 0 && index < m_tasks.size()) {
                emit taskClicked(m_tasks[index]);
                return true;
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
