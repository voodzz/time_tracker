#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include "BasePage.h"
#include <QVariantMap>

class QLabel;
class QLineEdit;
class QPushButton;
class QStackedWidget;
class QWidget;

class ProfilePage : public BasePage
{
    Q_OBJECT
public:
    // If user is not authorized, pass userId = -1
    explicit ProfilePage(int userId = -1, QWidget *parent = nullptr);

    // Loads profile data (if authorized)
    void loadProfile();

signals:
    // Signal that can be used to notify MainWindow of successful login
    void loginSuccess(int userId);
    void logoutRequested();

protected:
    void setupUi() override;

private slots:
    void onSaveClicked();
    void onLoginClicked();
    void onSignUpClicked();
    void onShowLoginClicked();
    void onShowSignUpClicked();
    void onLogoutClicked();

private:
    int m_userId;
    QVariantMap m_profile;

    // Elements for displaying profile
    QLabel *m_usernameLabel;
    QLabel *m_createdLabel;
    QLineEdit *m_emailEdit;
    QPushButton *m_saveButton;
    QPushButton *m_logoutButton;

    // Elements for login form
    QLineEdit *m_loginUsername;
    QLineEdit *m_loginPassword;
    QPushButton *m_loginButton;
    QPushButton *m_showSignUpButton;

    // Elements for registration form
    QWidget *m_signUpPage;
    QLineEdit *m_signUpUsername;
    QLineEdit *m_signUpPassword;
    QLineEdit *m_signUpEmail;
    QPushButton *m_signUpButton;
    QPushButton *m_showLoginButton;

    // Stack of pages: login page, registration page, and profile page
    QStackedWidget *m_stack;
};

#endif // PROFILEPAGE_H
