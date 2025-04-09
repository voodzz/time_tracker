#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QCheckBox>

class StartPage : public QWidget
{
    Q_OBJECT
public:
    explicit StartPage(QWidget *parent = nullptr);
    
    // Try to auto-login using saved credentials
    bool tryAutoLogin();

signals:
    // Сигнал успешного входа, передается id пользователя
    void loginSuccess(int userId);

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void switchPage(int index);
    void saveLoginCredentials(const QString &username, const QString &passwordHash);
    void clearSavedCredentials();

private:
    // Форма входа
    QWidget *m_loginWidget;
    QLineEdit *m_loginUsername;
    QLineEdit *m_loginPassword;
    QPushButton *m_loginButton;
    QCheckBox *m_rememberMe;

    // Форма регистрации
    QWidget *m_registerWidget;
    QLineEdit *m_regUsername;
    QLineEdit *m_regPassword;
    QLineEdit *m_regEmail;
    QPushButton *m_registerButton;
    QPushButton *m_switchToLoginButton;
    QPushButton *m_switchToRegisterButton;

    QStackedWidget *m_stack;
};

#endif // STARTPAGE_H
