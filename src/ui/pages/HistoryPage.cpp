#include "HistoryPage.h"
#include "../../core/DatabaseManager.h"
#include "../components/DayCard.h"
#include <QCalendarWidget>
#include <QMessageBox>
#include <QDebug>
#include <algorithm>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QScrollBar>
#include <QApplication>
#include <QStyle>
#include <QVariantMap>

HistoryPage::HistoryPage(int userId, QWidget *parent)
    : QWidget(parent), m_userId(userId), m_firstCardIndex(0)
{
    if (userId <= 0) {
        qDebug() << "Warning: Invalid user ID in HistoryPage constructor";
    }
    m_baseDate = QDate::currentDate();
    setupUi();
    loadHistory();
}

HistoryPage::~HistoryPage() { }

void HistoryPage::setupUi()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(20);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);

    // Header
    QLabel* headerLabel = new QLabel("Task History", this);
    headerLabel->setObjectName("historyHeader");
    headerLabel->setProperty("class", "historyHeader");
    headerLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    m_mainLayout->addWidget(headerLabel);

    // Main content container
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(10);
    
    // Left navigation button
    m_leftButton = new QPushButton(this);
    m_leftButton->setIcon(QIcon(":/icons/arrow-left.png"));
    m_leftButton->setIconSize(QSize(24, 24));
    m_leftButton->setFixedSize(40, 200);
    m_leftButton->setObjectName("navButton");
    m_leftButton->setProperty("class", "navButton");
    m_leftButton->setCursor(Qt::PointingHandCursor);
    
    // Cards container
    QWidget* cardsWidget = new QWidget(this);
    cardsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_dayCardsLayout = new QHBoxLayout(cardsWidget);
    m_dayCardsLayout->setSpacing(15);  // Increased spacing between cards
    m_dayCardsLayout->setContentsMargins(0, 0, 0, 0);
    m_dayCardsLayout->setAlignment(Qt::AlignCenter);  // Center the cards
    
    // Create 5 day cards
    for (int i = 0; i < 5; ++i) {
        DayCard* card = new DayCard(m_baseDate.addDays(i), this);
        m_dayCards.append(card);
        m_dayCardsLayout->addWidget(card, 1);  // Add stretch factor of 1 to make cards equal width
        connect(card, &DayCard::taskClicked, this, &HistoryPage::onTaskClicked);
    }
    
    // Right navigation button
    m_rightButton = new QPushButton(this);
    m_rightButton->setIcon(QIcon(":/icons/arrow-right.png"));
    m_rightButton->setIconSize(QSize(24, 24));
    m_rightButton->setFixedSize(40, 200);
    m_rightButton->setObjectName("navButton");
    m_rightButton->setProperty("class", "navButton");
    m_rightButton->setCursor(Qt::PointingHandCursor);

    contentLayout->addWidget(m_leftButton);
    contentLayout->addWidget(cardsWidget, 1);
    contentLayout->addWidget(m_rightButton);
    
    // Date selection
    QHBoxLayout* dateSelectionLayout = new QHBoxLayout();
    m_chooseDayButton = new QPushButton("Select Date", this);
    m_chooseDayButton->setObjectName("chooseDayButton");
    m_chooseDayButton->setProperty("class", "chooseDayButton");
    m_chooseDayButton->setCursor(Qt::PointingHandCursor);
    dateSelectionLayout->addStretch();
    dateSelectionLayout->addWidget(m_chooseDayButton);
    dateSelectionLayout->addStretch();

    // Task details frame
    m_detailsFrame = new QFrame(this);
    m_detailsFrame->setObjectName("detailsFrame");
    m_detailsFrame->setProperty("class", "detailsFrame");
    m_detailsFrame->setFrameShape(QFrame::StyledPanel);
    
    QVBoxLayout* detailsLayout = new QVBoxLayout(m_detailsFrame);
    detailsLayout->setSpacing(10);
    detailsLayout->setContentsMargins(15, 15, 15, 15);

    QLabel* detailsHeaderLabel = new QLabel("Task Details", m_detailsFrame);
    detailsHeaderLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    
    m_detailsLabel = new QLabel(m_detailsFrame);
    m_detailsLabel->setWordWrap(true);
    m_detailsLabel->setTextFormat(Qt::RichText);
    m_detailsLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_detailsLabel->setOpenExternalLinks(true);
    
    detailsLayout->addWidget(detailsHeaderLabel);
    detailsLayout->addWidget(m_detailsLabel);

    m_mainLayout->addLayout(contentLayout);
    m_mainLayout->addLayout(dateSelectionLayout);
    m_mainLayout->addWidget(m_detailsFrame);

    // Connect signals
    connect(m_leftButton, &QPushButton::clicked, this, &HistoryPage::shiftLeft);
    connect(m_rightButton, &QPushButton::clicked, this, &HistoryPage::shiftRight);
    connect(m_chooseDayButton, &QPushButton::clicked, this, &HistoryPage::chooseDay);

    // Subscribe to task updates
    connect(&DatabaseManager::instance(), &DatabaseManager::taskUpdated,
            this, &HistoryPage::onTaskUpdated);
}

void HistoryPage::loadHistory()
{
    if (m_userId <= 0) {
        qDebug() << "Error: Invalid user ID in loadHistory";
        return;
    }

    // Get active tasks from tasks table
    QList<QVariantMap> tasksData = DatabaseManager::instance().getTasks(m_userId);
    QMap<QDate, QList<TaskInfo>> historyMap;
    
    for (const QVariantMap &data : tasksData) {
        QString status = data.value("status").toString();
        if (status == "Active") {
            QDate deadline = data.value("deadline").toDate();
            if (!deadline.isValid()) {
                qDebug() << "Warning: Invalid deadline for task:" << data.value("name").toString();
                continue;
            }

            TaskInfo info;
            info.taskId = data.value("id").toInt();
            info.taskName = data.value("name").toString();
            info.cycles = data.value("planned_cycles").toInt();
            info.status = status;
            info.priority = data.value("priority").toString();
            info.deadline = deadline;
            info.description = data.value("description").toString();
            info.details = formatTaskDetails(info);
            historyMap[deadline].append(info);
        }
    }

    // Get completed tasks from task_history
    QList<QVariantMap> historyData = DatabaseManager::instance().getTaskHistory(m_userId);
    for (const QVariantMap &record : historyData) {
        QDateTime executedAt = record.value("executed_at").toDateTime();
        if (!executedAt.isValid()) {
            qDebug() << "Warning: Invalid execution time for task:" << record.value("taskName").toString();
            continue;
        }

        TaskInfo info;
        info.taskId = record.value("task_id").toInt();
        info.taskName = record.value("taskName").toString();
        info.cycles = record.value("completed_cycles").toInt();
        info.status = "Completed";
        info.completedAt = executedAt;
        info.details = formatTaskDetails(info);
        historyMap[executedAt.date()].append(info);
    }

    m_historyData = historyMap;
    updateDayCards();
}

QString HistoryPage::formatTaskDetails(const TaskInfo &task) const
{
    QString details = QString("<b>Name:</b> %1<br>").arg(task.taskName.toHtmlEscaped());
    details += QString("<b>Cycles:</b> %1<br>").arg(task.cycles);
    details += QString("<b>Status:</b> %1<br>").arg(task.status.toHtmlEscaped());
    
    if (!task.priority.isEmpty())
        details += QString("<b>Priority:</b> %1<br>").arg(task.priority.toHtmlEscaped());
    
    if (task.deadline.isValid()) {
        QString deadlineColor = task.deadline < QDate::currentDate() ? "red" : "inherit";
        details += QString("<b>Deadline:</b> <span style='color: %1'>%2</span><br>")
                    .arg(deadlineColor)
                    .arg(task.deadline.toString("MMMM d, yyyy"));
    }
    
    if (!task.description.isEmpty())
        details += QString("<b>Description:</b> %1<br>").arg(task.description.toHtmlEscaped());
    
    if (task.completedAt.isValid())
        details += QString("<b>Completed:</b> %1")
                    .arg(task.completedAt.toString("MMMM d, yyyy hh:mm:ss"));
    
    return details;
}

void HistoryPage::updateDayCards()
{
    for (int i = 0; i < m_dayCards.size(); ++i) {
        QDate cardDate = m_baseDate.addDays(i + m_firstCardIndex);
        if (!cardDate.isValid()) {
            qDebug() << "Warning: Invalid date calculated in updateDayCards";
            continue;
        }
        
        DayCard *card = m_dayCards[i];
        if (!card) {
            qDebug() << "Error: Null card pointer at index" << i;
            continue;
        }
        
        card->setDate(cardDate);
        card->setTasks(m_historyData.value(cardDate));
    }
}

void HistoryPage::shiftLeft()
{
    m_firstCardIndex--;
    updateDayCards();
}

void HistoryPage::shiftRight()
{
    m_firstCardIndex++;
    updateDayCards();
}

void HistoryPage::chooseDay()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Select Date");
    dialog.setModal(true);
    
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);
    
    QCalendarWidget* calendar = new QCalendarWidget(&dialog);
    calendar->setSelectedDate(m_baseDate);
    calendar->setGridVisible(true);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar->setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
    
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        &dialog
    );
    
    layout->addWidget(calendar);
    layout->addWidget(buttonBox);
    
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        m_baseDate = calendar->selectedDate();
        m_firstCardIndex = 0;
        updateDayCards();
    }
}

void HistoryPage::onTaskClicked(const TaskInfo &task)
{
    if (!task.taskName.isEmpty()) {
        m_detailsLabel->setText(task.details);
    }
}

void HistoryPage::onTaskUpdated(const QVariantMap &taskData)
{
    if (taskData.contains("id")) {
        loadHistory();  // Reload all history when a task is updated
        updateDayCards();
    }
}
