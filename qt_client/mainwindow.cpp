#include "mainwindow.h"
#include "networkclient.h"
#include "loginwindow.h"
#include "chatwidget.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QStatusBar>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_chatWidget(nullptr)
{
    setWindowTitle("Chat Application - Nhóm 11");
    setMinimumSize(900, 600);
    
    // Default server settings
    m_serverHost = "127.0.0.1";
    m_serverPort = 8888;
    
    // Create network client
    m_client = new NetworkClient(this);
    
    connect(m_client, &NetworkClient::connected, this, &MainWindow::onConnected);
    connect(m_client, &NetworkClient::disconnected, this, &MainWindow::onDisconnected);
    connect(m_client, &NetworkClient::connectionError, this, &MainWindow::onConnectionError);
    
    // Create stacked widget
    m_stackedWidget = new QStackedWidget;
    setCentralWidget(m_stackedWidget);
    
    // Create login window
    m_loginWindow = new LoginWindow(m_client);
    connect(m_loginWindow, &LoginWindow::loginSuccess, this, &MainWindow::onLoginSuccess);
    m_stackedWidget->addWidget(m_loginWindow);
    
    // Status bar
    statusBar()->showMessage("Đang kết nối đến server...");
    
    // Connect to server
    connectToServer();
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectToServer()
{
    statusBar()->showMessage("Đang kết nối đến " + m_serverHost + ":" + QString::number(m_serverPort) + "...");
    
    if (!m_client->connectToServer(m_serverHost, m_serverPort)) {
        // Ask for server address
        bool ok;
        QString address = QInputDialog::getText(this, "Kết nối Server",
            "Không thể kết nối đến server.\nNhập địa chỉ server (host:port):",
            QLineEdit::Normal, m_serverHost + ":" + QString::number(m_serverPort), &ok);
        
        if (ok && !address.isEmpty()) {
            QStringList parts = address.split(":");
            m_serverHost = parts[0];
            if (parts.size() > 1) {
                m_serverPort = parts[1].toInt();
            }
            connectToServer();
        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể kết nối đến server. Ứng dụng sẽ đóng.");
            QApplication::quit();
        }
    }
}

void MainWindow::onConnected()
{
    statusBar()->showMessage("✅ Đã kết nối đến server", 3000);
}

void MainWindow::onDisconnected()
{
    statusBar()->showMessage("❌ Mất kết nối với server");
    
    QMessageBox::warning(this, "Mất kết nối", 
        "Kết nối với server đã bị mất.\nVui lòng thử kết nối lại.");
    
    // Switch back to login
    if (m_chatWidget) {
        m_stackedWidget->removeWidget(m_chatWidget);
        delete m_chatWidget;
        m_chatWidget = nullptr;
    }
    m_stackedWidget->setCurrentWidget(m_loginWindow);
    
    connectToServer();
}

void MainWindow::onConnectionError(const QString &error)
{
    statusBar()->showMessage("❌ Lỗi: " + error);
}

void MainWindow::onLoginSuccess(const QString &username)
{
    setWindowTitle("Chat Application - " + username);
    
    // Create and show chat widget
    m_chatWidget = new ChatWidget(m_client, username);
    connect(m_chatWidget, &ChatWidget::logoutRequested, this, &MainWindow::onLogout);
    m_stackedWidget->addWidget(m_chatWidget);
    m_stackedWidget->setCurrentWidget(m_chatWidget);
    
    statusBar()->showMessage("Logged in as: " + username);
}

void MainWindow::onLogout()
{
    setWindowTitle("Chat Application - Nhom 11");
    
    // Remove and delete chat widget
    if (m_chatWidget) {
        m_stackedWidget->removeWidget(m_chatWidget);
        delete m_chatWidget;
        m_chatWidget = nullptr;
    }
    
    // Switch back to login window
    m_stackedWidget->setCurrentWidget(m_loginWindow);
    statusBar()->showMessage("Logged out");
}
