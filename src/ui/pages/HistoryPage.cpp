#include "HistoryPage.h"

HistoryPage::HistoryPage(QWidget *parent)
    : BasePage(parent)
{
    setupUi();
}

HistoryPage::~HistoryPage()
{
}

void HistoryPage::setupUi()
{
    m_layout = new QVBoxLayout(this);
    m_titleLabel = new QLabel(tr("History Page"), this);
    m_layout->addWidget(m_titleLabel);
    
    setLayout(m_layout);
}
