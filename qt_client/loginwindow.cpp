#include "loginwindow.h"
#include "networkclient.h"
#include <QMessageBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QApplication>
#include <QScreen>
#include <QInputMethod>

LoginWindow::LoginWindow(NetworkClient *client, QWidget *parent)
    : QWidget(parent), m_client(client)
{
    setWindowTitle("Chat Application - Nhom 11");
    setFixedSize(420, 480);
    
    // Center window on screen
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
    
    // Main widget styling
    setStyleSheet(
        "QWidget { background-color: #f5f5f5; }"
        "QLineEdit { "
        "   padding: 12px; "
        "   border: 1px solid #ccc; "
        "   border-radius: 6px; "
        "   font-size: 14px; "
        "   background-color: white; "
        "}"
        "QLineEdit:focus { border: 2px solid #2196F3; }"
        "QPushButton { "
        "   padding: 12px; "
        "   border: none; "
        "   border-radius: 6px; "
        "   font-size: 14px; "
        "   font-weight: bold; "
        "   color: white; "
        "}"
        "QTabWidget::pane { "
        "   border: 1px solid #ddd; "
        "   background-color: white; "
        "   border-radius: 8px; "
        "   padding: 15px; "
        "}"
        "QTabBar::tab { "
        "   padding: 12px 40px; "
        "   font-size: 14px; "
        "   font-weight: bold; "
        "   background-color: #e0e0e0; "
        "   border-top-left-radius: 6px; "
        "   border-top-right-radius: 6px; "
        "   margin-right: 2px; "
        "   min-width: 80px; "
        "}"
        "QTabBar::tab:selected { "
        "   background-color: white; "
        "   color: #2196F3; "
        "}"
    );
    
    // Main layout - center content both horizontally and vertically
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    
    // Vertical centering
    rootLayout->addStretch(1);
    
    // Horizontal centering container
    QHBoxLayout *hCenterLayout = new QHBoxLayout;
    hCenterLayout->addStretch(1);
    
    // Center container with fixed width
    QWidget *centerContainer = new QWidget;
    centerContainer->setFixedWidth(320);
    centerContainer->setStyleSheet("background: transparent;");
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centerContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(12);
    
    // Title
    QLabel *titleLabel = new QLabel("CHAT APPLICATION");
    titleLabel->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: #2196F3; "
        "background: transparent; padding: 5px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    QLabel *subtitleLabel = new QLabel("Nhom 11 - Lap trinh mang");
    subtitleLabel->setStyleSheet(
        "font-size: 12px; color: #666; background: transparent;");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subtitleLabel);
    
    mainLayout->addSpacing(15);
    
    // Tab widget
    QTabWidget *tabWidget = new QTabWidget;
    
    // ===== LOGIN TAB =====
    QWidget *loginTab = new QWidget;
    loginTab->setStyleSheet("background-color: white;");
    QVBoxLayout *loginLayout = new QVBoxLayout(loginTab);
    loginLayout->setSpacing(12);
    loginLayout->setContentsMargins(15, 15, 15, 15);
    
    m_loginUsername = new QLineEdit;
    m_loginUsername->setPlaceholderText("Username");
    m_loginUsername->setMinimumHeight(40);
    m_loginUsername->setAttribute(Qt::WA_InputMethodEnabled, true);
    m_loginUsername->setInputMethodHints(Qt::ImhNone);
    loginLayout->addWidget(m_loginUsername);
    
    m_loginPassword = new QLineEdit;
    m_loginPassword->setEchoMode(QLineEdit::Password);
    m_loginPassword->setPlaceholderText("Password");
    m_loginPassword->setMinimumHeight(40);
    m_loginPassword->setAttribute(Qt::WA_InputMethodEnabled, true);
    m_loginPassword->setInputMethodHints(Qt::ImhNone);
    loginLayout->addWidget(m_loginPassword);
    
    loginLayout->addSpacing(5);
    
    m_loginButton = new QPushButton("Login");
    m_loginButton->setMinimumHeight(42);
    m_loginButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; }"
        "QPushButton:hover { background-color: #1976D2; }"
        "QPushButton:pressed { background-color: #1565C0; }"
        "QPushButton:disabled { background-color: #90CAF9; }");
    loginLayout->addWidget(m_loginButton);
    
    loginLayout->addStretch();
    tabWidget->addTab(loginTab, "Login");
    
    // ===== REGISTER TAB =====
    QWidget *registerTab = new QWidget;
    registerTab->setStyleSheet("background-color: white;");
    QVBoxLayout *registerLayout = new QVBoxLayout(registerTab);
    registerLayout->setSpacing(12);
    registerLayout->setContentsMargins(15, 15, 15, 15);
    
    m_regUsername = new QLineEdit;
    m_regUsername->setPlaceholderText("New Username");
    m_regUsername->setMinimumHeight(40);
    m_regUsername->setAttribute(Qt::WA_InputMethodEnabled, true);
    m_regUsername->setInputMethodHints(Qt::ImhNone);
    registerLayout->addWidget(m_regUsername);
    
    m_regPassword = new QLineEdit;
    m_regPassword->setEchoMode(QLineEdit::Password);
    m_regPassword->setPlaceholderText("Password");
    m_regPassword->setMinimumHeight(40);
    m_regPassword->setAttribute(Qt::WA_InputMethodEnabled, true);
    m_regPassword->setInputMethodHints(Qt::ImhNone);
    registerLayout->addWidget(m_regPassword);
    
    m_regConfirmPassword = new QLineEdit;
    m_regConfirmPassword->setEchoMode(QLineEdit::Password);
    m_regConfirmPassword->setPlaceholderText("Confirm Password");
    m_regConfirmPassword->setMinimumHeight(40);
    m_regConfirmPassword->setAttribute(Qt::WA_InputMethodEnabled, true);
    m_regConfirmPassword->setInputMethodHints(Qt::ImhNone);
    registerLayout->addWidget(m_regConfirmPassword);
    
    registerLayout->addSpacing(5);
    
    m_registerButton = new QPushButton("Create Account");
    m_registerButton->setMinimumHeight(42);
    m_registerButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; }"
        "QPushButton:hover { background-color: #43A047; }"
        "QPushButton:pressed { background-color: #388E3C; }"
        "QPushButton:disabled { background-color: #A5D6A7; }");
    registerLayout->addWidget(m_registerButton);
    
    registerLayout->addStretch();
    tabWidget->addTab(registerTab, "Register");
    
    mainLayout->addWidget(tabWidget);
    
    // Status label
    m_statusLabel = new QLabel;
    m_statusLabel->setStyleSheet(
        "color: #f44336; font-size: 12px; background: transparent; padding: 5px;");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setWordWrap(true);
    mainLayout->addWidget(m_statusLabel);
    
    // Add center container to horizontal center layout
    hCenterLayout->addWidget(centerContainer);
    hCenterLayout->addStretch(1);
    
    // Add horizontal layout to root
    rootLayout->addLayout(hCenterLayout);
    rootLayout->addStretch(1);
    
    // Connections
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(m_registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connect(m_loginPassword, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    connect(m_regConfirmPassword, &QLineEdit::returnPressed, this, &LoginWindow::onRegisterClicked);
    
    connect(m_client, &NetworkClient::loginResponse, this, &LoginWindow::onLoginResponse);
    connect(m_client, &NetworkClient::registerResponse, this, &LoginWindow::onRegisterResponse);
}

void LoginWindow::onLoginClicked()
{
    QString username = m_loginUsername->text().trimmed();
    QString password = m_loginPassword->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        m_statusLabel->setText("Please enter username and password");
        return;
    }
    
    m_currentUsername = username;
    m_statusLabel->setText("Logging in...");
    m_statusLabel->setStyleSheet("color: #2196F3; font-size: 12px; background: transparent;");
    m_loginButton->setEnabled(false);
    
    m_client->sendLogin(username, password);
}

void LoginWindow::onRegisterClicked()
{
    QString username = m_regUsername->text().trimmed();
    QString password = m_regPassword->text();
    QString confirmPassword = m_regConfirmPassword->text();
    
    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        m_statusLabel->setText("Please fill in all fields");
        m_statusLabel->setStyleSheet("color: #f44336; font-size: 12px; background: transparent;");
        return;
    }
    
    if (password != confirmPassword) {
        m_statusLabel->setText("Passwords do not match");
        m_statusLabel->setStyleSheet("color: #f44336; font-size: 12px; background: transparent;");
        return;
    }
    
    if (password.length() < 3) {
        m_statusLabel->setText("Password must be at least 3 characters");
        m_statusLabel->setStyleSheet("color: #f44336; font-size: 12px; background: transparent;");
        return;
    }
    
    m_statusLabel->setText("Creating account...");
    m_statusLabel->setStyleSheet("color: #4CAF50; font-size: 12px; background: transparent;");
    m_registerButton->setEnabled(false);
    
    m_client->sendRegister(username, password);
}

void LoginWindow::onLoginResponse(bool success, const QString &message, const QString &token)
{
    m_loginButton->setEnabled(true);
    
    if (success) {
        m_client->setToken(token);
        m_statusLabel->setText("Login successful!");
        m_statusLabel->setStyleSheet("color: #4CAF50; font-size: 12px; background: transparent;");
        emit loginSuccess(m_currentUsername);
    } else {
        m_statusLabel->setText(message.isEmpty() ? "Login failed" : message);
        m_statusLabel->setStyleSheet("color: #f44336; font-size: 12px; background: transparent;");
    }
}

void LoginWindow::onRegisterResponse(bool success, const QString &message)
{
    m_registerButton->setEnabled(true);
    
    if (success) {
        m_statusLabel->setText("Account created! You can now login.");
        m_statusLabel->setStyleSheet("color: #4CAF50; font-size: 12px; background: transparent;");
        
        // Clear register fields
        m_regUsername->clear();
        m_regPassword->clear();
        m_regConfirmPassword->clear();
    } else {
        m_statusLabel->setText(message.isEmpty() ? "Registration failed" : message);
        m_statusLabel->setStyleSheet("color: #f44336; font-size: 12px; background: transparent;");
    }
}
