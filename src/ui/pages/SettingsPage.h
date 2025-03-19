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

private:
    QGridLayout *m_mainLayout;
    QLabel *m_titleLabel;
    
    QGroupBox *m_themeGroup;
    QGridLayout *m_themeLayout;
    QLabel *m_themeLabel;
    QComboBox *m_themeComboBox;
    QPushButton *m_applyButton;
};

#endif // SETTINGSPAGE_H
