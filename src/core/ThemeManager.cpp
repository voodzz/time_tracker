#include "ThemeManager.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QApplication>
#include <QSettings>


ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
{
    m_baseThemePath = "C:\\Instruments\\Qt Projects\\time_tracker\\src\\resources\\themes\\";
    
    QSettings settings;
    m_currentTheme = settings.value("theme", "").toString();
    if (!m_currentTheme.isEmpty()) {
        applyTheme(m_currentTheme);
    }
}

QStringList ThemeManager::availableThemes() const
{
    QStringList themes;
    QDir baseDir(m_baseThemePath);
    if (!baseDir.exists()) {
        qWarning() << "TME: Theme folder not found:" << m_baseThemePath;
        return themes;
    }
    foreach (QString folderName, baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        themes << folderName;
    }
    return themes;
}

bool ThemeManager::applyTheme(const QString &themeName)
{
    QDir themeDir(m_baseThemePath + themeName + "/");
    if (!themeDir.exists()) {
        qWarning() << "TME: Theme folder not found:" << themeName;
        return false;
    }

    QString jsonFilePath = themeDir.absoluteFilePath(themeName + ".json");
    QFile jsonFile(jsonFilePath);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qWarning() << "TME: Cannot open JSON file:" << jsonFilePath;
        return false;
    }
    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "TME: JSON parse error:" << parseError.errorString();
        return false;
    }
    QJsonObject jsonObj = doc.object();

    // Читаем QSS-файл
    QString qssFileName = jsonObj.value("qssFile").toString();
    if (qssFileName.isEmpty()) {
        qWarning() << "TME: QSS file not found in JSON:" << jsonFilePath;
        return false;
    }
    QString qssFilePath = themeDir.absoluteFilePath(qssFileName);
    QFile qssFile(qssFilePath);
    if (!qssFile.open(QIODevice::ReadOnly)) {
        qWarning() << "TME: Cannot open QSS file:" << qssFilePath;
        return false;
    }
    QString qss = qssFile.readAll();
    qssFile.close();

QRegularExpression re("\\{\\{(\\w+)\\}\\}");
QRegularExpressionMatchIterator i = re.globalMatch(qss);
while (i.hasNext()) {
    QRegularExpressionMatch match = i.next();
    QString key = match.captured(1);
    qDebug() << "TME: json key: " << key;
    
    QString replacement = jsonObj.value(key).toString();
    if (replacement.isEmpty()) continue;

    QFileInfo iconFileInfo(themeDir.filePath(replacement));
    if (iconFileInfo.exists() && iconFileInfo.isFile()) {
        replacement = QUrl::fromLocalFile(iconFileInfo.absoluteFilePath()).toString();
    } else if (replacement.startsWith("file:///")) {
        replacement = QUrl(replacement).toString();
    }

    if (replacement.startsWith("file:///")) {
        replacement.remove(0, 8);
    }

    qDebug() << "TME: replacement: " << replacement;
    qss.replace(match.captured(0), replacement);
}


    qApp->setStyleSheet(qss);

    m_currentTheme = themeName;
    QSettings settings;
    settings.setValue("theme", themeName);
    qDebug() << "Theme" << themeName << "applied.";
    return true;
}