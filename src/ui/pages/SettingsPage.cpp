#include "SettingsPage.h"
#include "../../Application.h"
#include "../../core/ThemeManager.h"

#include <QSettings>
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

    // Header
    m_titleLabel = new QLabel(tr("Settings Page"), this);
    m_titleLabel->setStyleSheet("font-size: 22px; font-weight: bold;");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_mainLayout->addWidget(m_titleLabel, 0, 0, 1, 2, Qt::AlignCenter);

    // Theme settings group
    m_themeGroup = new QGroupBox(tr("Theme Settings"), this);
    m_themeLayout = new QGridLayout(m_themeGroup);
    m_themeLabel = new QLabel(tr("Select Theme:"), this);
    m_themeComboBox = new QComboBox(this);
    m_applyThemeButton = new QPushButton(tr("Apply Theme"), this);
    m_themeLayout->addWidget(m_themeLabel, 0, 0);
    m_themeLayout->addWidget(m_themeComboBox, 0, 1);
    m_themeLayout->addWidget(m_applyThemeButton, 1, 0, 1, 2, Qt::AlignCenter);
    m_themeGroup->setLayout(m_themeLayout);
    m_mainLayout->addWidget(m_themeGroup, 1, 0, 1, 2);

    // Star emoji settings group
    m_emojiGroup = new QGroupBox(tr("Star Emoji Settings"), this);
    m_emojiLayout = new QGridLayout(m_emojiGroup);
    m_emojiLabel = new QLabel(tr("Select Star Emoji:"), this);
    m_emojiComboBox = new QComboBox(this);
    // Fill the list with example emojis (can be expanded)
    m_emojiComboBox->addItem("★");  // classic star
    m_emojiComboBox->addItem("⭐");  // white star
    m_emojiComboBox->addItem("✶");  // alternative variant
    m_applyEmojiButton = new QPushButton(tr("Apply Star Emoji"), this);
    m_emojiLayout->addWidget(m_emojiLabel, 0, 0);
    m_emojiLayout->addWidget(m_emojiComboBox, 0, 1);
    m_emojiLayout->addWidget(m_applyEmojiButton, 1, 0, 1, 2, Qt::AlignCenter);
    m_emojiGroup->setLayout(m_emojiLayout);
    m_mainLayout->addWidget(m_emojiGroup, 2, 0, 1, 2);

    setLayout(m_mainLayout);

    // Fill theme data
    Application *app = qobject_cast<Application *>(qApp);
    if (app && app->themeManager()) {
        m_themeComboBox->addItems(app->themeManager()->availableThemes());
        m_themeComboBox->setCurrentText(app->themeManager()->currentTheme());
    }

    // Read saved emoji from QSettings (if exists)
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "voodz_d1sh0w", "time_tracker");
    QString savedEmoji = settings.value("starEmoji", "★").toString();
    m_emojiComboBox->setCurrentText(savedEmoji);

    connect(m_applyThemeButton, &QPushButton::clicked, this, &SettingsPage::changeTheme);
    connect(m_applyEmojiButton, &QPushButton::clicked, this, &SettingsPage::changeStarEmoji);
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

void SettingsPage::changeStarEmoji()
{
    QString selectedEmoji = m_emojiComboBox->currentText();
    if (!selectedEmoji.isEmpty()) {
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "voodz_d1sh0w", "time_tracker");
        settings.setValue("starEmoji", selectedEmoji);
        // You can emit a signal to update star display in all tasks, for example:
        // emit starEmojiChanged(selectedEmoji);
        qDebug() << "SPE: Star emoji changed to:" << selectedEmoji;
    }
}
