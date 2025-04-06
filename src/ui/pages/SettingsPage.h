#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "BasePage.h"
#include <QLabel>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>

class SettingsPage : public BasePage
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = nullptr);
    ~SettingsPage();

protected:
    void setupUi() override;

private slots:
    void changeTheme();
    void changeStarEmoji();

private:
    QGridLayout *m_mainLayout;
    QLabel *m_titleLabel;
    
    QGroupBox *m_themeGroup;
    QGridLayout *m_themeLayout;
    QLabel *m_themeLabel;
    QComboBox *m_themeComboBox;
    QPushButton *m_applyThemeButton;

    QGroupBox *m_emojiGroup;
    QGridLayout *m_emojiLayout;
    QLabel *m_emojiLabel;
    QComboBox *m_emojiComboBox;
    QPushButton *m_applyEmojiButton;
};

#endif // SETTINGSPAGE_H
