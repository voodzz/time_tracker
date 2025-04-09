#include "StartPage.h"
#include "../../core/DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSettings>

StartPage::StartPage(QWidget *parent) : QWidget(parent)
{
    m_stack = new QStackedWidget(this);

    // Create login widget
    m_loginWidget = new QWidget(this);
    QVBoxLayout *loginLayout = new QVBoxLayout(m_loginWidget);
    QLabel *loginLabel = new QLabel(tr("Sign in to the system"), m_loginWidget);
    m_loginUsername = new QLineEdit(m_loginWidget);
    m_loginUsername->setPlaceholderText(tr("Username"));
    m_loginPassword = new QLineEdit(m_loginWidget);
    m_loginPassword->setPlaceholderText(tr("Password"));
    m_loginPassword->setEchoMode(QLineEdit::Password);
    m_loginButton = new QPushButton(tr("Sign in"), m_loginWidget);
    m_rememberMe = new QCheckBox(tr("Remember me"), m_loginWidget);
    m_switchToRegisterButton = new QPushButton(tr("Register"), m_loginWidget);
    loginLayout->addWidget(loginLabel);
    loginLayout->addWidget(m_loginUsername);
    loginLayout->addWidget(m_loginPassword);
    loginLayout->addWidget(m_rememberMe);
    loginLayout->addWidget(m_loginButton);
    loginLayout->addWidget(m_switchToRegisterButton);
    m_loginWidget->setLayout(loginLayout);

    // Create registration widget
    m_registerWidget = new QWidget(this);
    QVBoxLayout *regLayout = new QVBoxLayout(m_registerWidget);
    QLabel *regLabel = new QLabel(tr("Registration of a new account"), m_registerWidget);
    m_regUsername = new QLineEdit(m_registerWidget);
    m_regUsername->setPlaceholderText(tr("Username"));
    m_regPassword = new QLineEdit(m_registerWidget);
    m_regPassword->setPlaceholderText(tr("Password"));
    m_regPassword->setEchoMode(QLineEdit::Password);
    m_regEmail = new QLineEdit(m_registerWidget);
    m_regEmail->setPlaceholderText(tr("Email"));
    m_registerButton = new QPushButton(tr("Register"), m_registerWidget);
    m_switchToLoginButton = new QPushButton(tr("Sign in"), m_registerWidget);
    regLayout->addWidget(regLabel);
    regLayout->addWidget(m_regUsername);
    regLayout->addWidget(m_regPassword);
    regLayout->addWidget(m_regEmail);
    regLayout->addWidget(m_registerButton);
    regLayout->addWidget(m_switchToLoginButton);
    m_registerWidget->setLayout(regLayout);

    m_stack->addWidget(m_loginWidget);
    m_stack->addWidget(m_registerWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_stack);
    setLayout(mainLayout);

    connect(m_loginButton, &QPushButton::clicked, this, &StartPage::onLoginClicked);
    connect(m_registerButton, &QPushButton::clicked, this, &StartPage::onRegisterClicked);
    connect(m_switchToRegisterButton, &QPushButton::clicked, [this](){
        switchPage(1);
    });
    connect(m_switchToLoginButton, &QPushButton::clicked, [this](){
        switchPage(0);
    });

    if (!tryAutoLogin()) {
        // No auto-login, do nothing
    }
}

void StartPage::switchPage(int index)
{
    m_stack->setCurrentIndex(index);
}

void StartPage::onLoginClicked()
{
    QString username = m_loginUsername->text();
    QString password = m_loginPassword->text();
    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this, "Error", "Please fill in all fields");
        return;
    }
    
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString passwordHash = hash.toHex();
    int userId = DatabaseManager::instance().loginUser(username, passwordHash);
    
    if(userId != -1){
        if (m_rememberMe->isChecked()) {
            saveLoginCredentials(username, passwordHash);
        }
        emit loginSuccess(userId);
    } else {
        QMessageBox::warning(this, "Error", "Invalid login credentials");
    }
}

void StartPage::onRegisterClicked()
{
    QString username = m_regUsername->text();
    QString password = m_regPassword->text();
    QString email = m_regEmail->text();
    if(username.isEmpty() || password.isEmpty() || email.isEmpty()){
        QMessageBox::warning(this, "Error", "Please fill in all fields");
        return;
    }
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    int userId = DatabaseManager::instance().registerUser(username, hash.toHex(), email);
    if(userId != -1){
        QMessageBox::information(this, "Success", "Account registered. Please log in.");
        switchPage(0);
    } else {
        QMessageBox::warning(this, "Error", "Failed to register user");
    }
}

void StartPage::saveLoginCredentials(const QString &username, const QString &passwordHash)
{
    QSettings settings;
    settings.setValue("auth/username", username);
    settings.setValue("auth/passwordHash", passwordHash);
    settings.setValue("auth/rememberMe", true);
}

void StartPage::clearSavedCredentials()
{
    QSettings settings;
    settings.remove("auth/username");
    settings.remove("auth/passwordHash");
    settings.setValue("auth/rememberMe", false);
}

bool StartPage::tryAutoLogin()
{
    QSettings settings;
    if (!settings.value("auth/rememberMe", false).toBool()) {
        return false;
    }

    QString username = settings.value("auth/username").toString();
    QString passwordHash = settings.value("auth/passwordHash").toString();
    
    if (username.isEmpty() || passwordHash.isEmpty()) {
        return false;
    }

    int userId = DatabaseManager::instance().loginUser(username, passwordHash);
    if (userId != -1) {
        emit loginSuccess(userId);
        return true;
    }
    
    return false;
}
