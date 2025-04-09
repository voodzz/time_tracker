#include "ProfilePage.h"
#include "../../core/DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QMessageBox>
#include <QDateTime>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCryptographicHash>

ProfilePage::ProfilePage(int userId, QWidget *parent)
    : BasePage(parent), m_userId(userId)
{
    setupUi();
    // If user is already authorized, show profile immediately
    if(m_userId != -1){
        loadProfile();
        m_stack->setCurrentIndex(2); // Show profile page
    } else {
        m_stack->setCurrentIndex(0); // Show login page
    }
}

void ProfilePage::setupUi()
{
    m_stack = new QStackedWidget(this);

    // --- Login Page ---
    QWidget *loginPage = new QWidget(this);
    QVBoxLayout *loginLayout = new QVBoxLayout(loginPage);

    m_loginUsername = new QLineEdit(loginPage);
    m_loginUsername->setPlaceholderText("Username");
    m_loginPassword = new QLineEdit(loginPage);
    m_loginPassword->setPlaceholderText("Password");
    m_loginPassword->setEchoMode(QLineEdit::Password);
    m_loginButton = new QPushButton("Login", loginPage);
    m_showSignUpButton = new QPushButton("Create New Account", loginPage);

    loginLayout->addWidget(new QLabel("System Login", loginPage));
    loginLayout->addWidget(m_loginUsername);
    loginLayout->addWidget(m_loginPassword);
    loginLayout->addWidget(m_loginButton);
    loginLayout->addSpacing(20);
    loginLayout->addWidget(new QLabel("Don't have an account?", loginPage));
    loginLayout->addWidget(m_showSignUpButton);
    loginLayout->addStretch();
    loginPage->setLayout(loginLayout);

    connect(m_loginButton, &QPushButton::clicked, this, &ProfilePage::onLoginClicked);
    connect(m_showSignUpButton, &QPushButton::clicked, this, &ProfilePage::onShowSignUpClicked);

    // --- Sign Up Page ---
    m_signUpPage = new QWidget(this);
    QVBoxLayout *signUpLayout = new QVBoxLayout(m_signUpPage);

    m_signUpUsername = new QLineEdit(m_signUpPage);
    m_signUpUsername->setPlaceholderText("Username");
    m_signUpPassword = new QLineEdit(m_signUpPage);
    m_signUpPassword->setPlaceholderText("Password");
    m_signUpPassword->setEchoMode(QLineEdit::Password);
    m_signUpEmail = new QLineEdit(m_signUpPage);
    m_signUpEmail->setPlaceholderText("Email");
    m_signUpButton = new QPushButton("Sign Up", m_signUpPage);
    m_showLoginButton = new QPushButton("Back to Login", m_signUpPage);

    signUpLayout->addWidget(new QLabel("Create New Account", m_signUpPage));
    signUpLayout->addWidget(m_signUpUsername);
    signUpLayout->addWidget(m_signUpPassword);
    signUpLayout->addWidget(m_signUpEmail);
    signUpLayout->addWidget(m_signUpButton);
    signUpLayout->addSpacing(20);
    signUpLayout->addWidget(new QLabel("Already have an account?", m_signUpPage));
    signUpLayout->addWidget(m_showLoginButton);
    signUpLayout->addStretch();
    m_signUpPage->setLayout(signUpLayout);

    connect(m_signUpButton, &QPushButton::clicked, this, &ProfilePage::onSignUpClicked);
    connect(m_showLoginButton, &QPushButton::clicked, this, &ProfilePage::onShowLoginClicked);

    // --- Profile Page ---
    QWidget *profilePage = new QWidget(this);
    QVBoxLayout *profileLayout = new QVBoxLayout(profilePage);

    m_usernameLabel = new QLabel(profilePage);
    m_createdLabel = new QLabel(profilePage);
    m_emailEdit = new QLineEdit(profilePage);
    m_saveButton = new QPushButton("Save Changes", profilePage);
    m_logoutButton = new QPushButton("Logout", profilePage);

    profileLayout->addWidget(new QLabel("Username:", profilePage));
    profileLayout->addWidget(m_usernameLabel);
    profileLayout->addWidget(new QLabel("Email:", profilePage));
    profileLayout->addWidget(m_emailEdit);
    profileLayout->addWidget(new QLabel("Registration Date:", profilePage));
    profileLayout->addWidget(m_createdLabel);
    profileLayout->addWidget(m_saveButton);
    profileLayout->addWidget(m_logoutButton);
    profileLayout->addStretch();
    profilePage->setLayout(profileLayout);

    connect(m_saveButton, &QPushButton::clicked, this, &ProfilePage::onSaveClicked);
    connect(m_logoutButton, &QPushButton::clicked, this, &ProfilePage::onLogoutClicked);

    // Add all pages to the stack
    m_stack->addWidget(loginPage);     // Index 0: login page
    m_stack->addWidget(m_signUpPage);  // Index 1: sign up page
    m_stack->addWidget(profilePage);   // Index 2: profile page

    // Set m_stack as the main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_stack);
    setLayout(mainLayout);
}

void ProfilePage::loadProfile()
{
    m_profile = DatabaseManager::instance().getUserProfile(m_userId);
    if(m_profile.isEmpty()){
        QMessageBox::warning(this, "Error", "Failed to load profile");
        return;
    }
    m_usernameLabel->setText(m_profile.value("username").toString());
    m_emailEdit->setText(m_profile.value("email").toString());
    m_createdLabel->setText(m_profile.value("created_at").toDateTime().toString("dd.MM.yyyy hh:mm:ss"));
}

void ProfilePage::onSaveClicked()
{
    m_profile["email"] = m_emailEdit->text();
    if(DatabaseManager::instance().updateUserProfile(m_userId, m_profile)){
        QMessageBox::information(this, "Success", "Profile updated");
        loadProfile();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update profile");
    }
}

void ProfilePage::onLoginClicked()
{
    QString username = m_loginUsername->text();
    QString password = m_loginPassword->text();

    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this, "Error", "Please fill in all fields");
        return;
    }
    // Simple password hash (use more secure methods in production)
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    int userId = DatabaseManager::instance().loginUser(username, hash.toHex());
    if(userId != -1){
        m_userId = userId;
        emit loginSuccess(userId);
        m_stack->setCurrentIndex(2); // Switch to profile page
        loadProfile();
    } else {
        QMessageBox::warning(this, "Error", "Invalid login credentials");
    }
}

void ProfilePage::onSignUpClicked()
{
    QString username = m_signUpUsername->text();
    QString password = m_signUpPassword->text();
    QString email = m_signUpEmail->text();

    if(username.isEmpty() || password.isEmpty() || email.isEmpty()){
        QMessageBox::warning(this, "Error", "Please fill in all fields");
        return;
    }

    // Simple password hash (use more secure methods in production)
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

    int userId = DatabaseManager::instance().registerUser(username, hash.toHex(), email);
    if(userId != -1){
        m_userId = userId;
        emit loginSuccess(userId);
        QMessageBox::information(this, "Success", "Account created successfully!");
        m_stack->setCurrentIndex(2); // Switch to profile page
        loadProfile();
    } else {
        QMessageBox::warning(this, "Error", "Failed to create account. Username might be taken.");
    }
}

void ProfilePage::onShowLoginClicked()
{
    m_stack->setCurrentIndex(0); // Switch to login page
    m_signUpUsername->clear();
    m_signUpPassword->clear();
    m_signUpEmail->clear();
}

void ProfilePage::onShowSignUpClicked()
{
    m_stack->setCurrentIndex(1); // Switch to sign up page
    m_loginUsername->clear();
    m_loginPassword->clear();
}

void ProfilePage::onLogoutClicked()
{
    int result = QMessageBox::question(this, "Logout", "Are you sure you want to logout?",
                                     QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        qDebug() << "User confirmed logout";
        // Clear any saved credentials
        QSettings settings;
        settings.remove("auth/username");
        settings.remove("auth/password");
        settings.remove("auth/remember");
        
        emit logoutRequested();
    }
}
