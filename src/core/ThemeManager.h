#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>

class ThemeManager : public QObject
{
    Q_OBJECT
public:
    explicit ThemeManager(QObject *parent = nullptr);

    QStringList availableThemes() const;

    bool applyTheme(const QString &themeName);

    QString currentTheme() const { return m_currentTheme; }

    QString baseThemePath() const { return m_baseThemePath; }

private:
    QString m_currentTheme;
    QString m_baseThemePath;
};

#endif // THEMEMANAGER_H
