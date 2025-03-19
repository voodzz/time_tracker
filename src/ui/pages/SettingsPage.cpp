#include "SettingsPage.h"
#include "../../Application.h"
#include "../../core/ThemeManager.h"

#include <QDebug>

SettingsPage::SettingsPage(QWidget *parent)
    : BasePage(parent)
{
    setupUi();
}

SettingsPage::~SettingsPage()
{
}

void SettingsPage::setupUi()
{
    m_mainLayout = new QGridLayout(this);

    m_titleLabel = new QLabel(tr("Settings Page"), this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_mainLayout->addWidget(m_titleLabel, 0, 0, 1, 2, Qt::AlignCenter);

    m_themeGroup = new QGroupBox(tr("Theme Settings"), this);
    m_themeLayout = new QGridLayout(m_themeGroup);

    m_themeLabel = new QLabel(tr("Select Theme:"), this);
    m_themeComboBox = new QComboBox(this);
    m_applyButton = new QPushButton(tr("Apply Theme"), this);

    m_themeLayout->addWidget(m_themeLabel, 0, 0);
    m_themeLayout->addWidget(m_themeComboBox, 0, 1);
    m_themeLayout->addWidget(m_applyButton, 1, 0, 1, 2, Qt::AlignCenter);
    m_themeGroup->setLayout(m_themeLayout);

    m_mainLayout->addWidget(m_themeGroup, 1, 0, 1, 2);

    setLayout(m_mainLayout);

    Application *app = qobject_cast<Application *>(qApp);
    if (app && app->themeManager()) {
        m_themeComboBox->addItems(app->themeManager()->availableThemes());
        m_themeComboBox->setCurrentText(app->themeManager()->currentTheme());
    }

    connect(m_applyButton, &QPushButton::clicked, this, &SettingsPage::changeTheme);
}

void SettingsPage::changeTheme()
{
    Application *app = qobject_cast<Application *>(qApp);
    if (app && app->themeManager()) {
        QString selectedTheme = m_themeComboBox->currentText();
        if (!selectedTheme.isEmpty()) {
            bool result = app->themeManager()->applyTheme(selectedTheme);
            if (!result)
                qWarning() << "SPE: Failed to apply theme:" << selectedTheme;
        }
    }
}
