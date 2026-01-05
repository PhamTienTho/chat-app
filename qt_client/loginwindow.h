#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTabWidget>

class NetworkClient;

class LoginWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWindow(NetworkClient *client, QWidget *parent = nullptr);

signals:
    void loginSuccess(const QString &username);

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onLoginResponse(bool success, const QString &message, const QString &token);
    void onRegisterResponse(bool success, const QString &message);

private:
    NetworkClient *m_client;
    
    // Login tab
    QLineEdit *m_loginUsername;
    QLineEdit *m_loginPassword;
    QPushButton *m_loginButton;
    
    // Register tab
    QLineEdit *m_regUsername;
    QLineEdit *m_regPassword;
    QLineEdit *m_regConfirmPassword;
    QPushButton *m_registerButton;
    
    QLabel *m_statusLabel;
    QString m_currentUsername;
};

#endif // LOGINWINDOW_H
