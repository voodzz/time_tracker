#include "ProfilePage.h"

ProfilePage::ProfilePage(QWidget *parent)
    : BasePage(parent)
{
    setupUi();
}

ProfilePage::~ProfilePage()
{
}

void ProfilePage::setupUi()
{
    m_layout = new QVBoxLayout(this);
    m_titleLabel = new QLabel(tr("Profile Page"), this);
    m_layout->addWidget(m_titleLabel);
    
    setLayout(m_layout);
}
