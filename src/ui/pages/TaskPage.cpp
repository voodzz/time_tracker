#include "TaskPage.h"

TaskPage::TaskPage(QWidget *parent)
    : BasePage(parent)
{
    setupUi();
}

TaskPage::~TaskPage()
{
}

void TaskPage::setupUi()
{
    m_layout = new QVBoxLayout(this);
    m_titleLabel = new QLabel(tr("Task Page"), this);
    m_layout->addWidget(m_titleLabel);
    
    setLayout(m_layout);
}
