#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class NetworkClient;
class LoginWindow;
class ChatWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnected();
    void onDisconnected();
    void onConnectionError(const QString &error);
    void onLoginSuccess(const QString &username);
    void onLogout();
    void connectToServer();

private:
    NetworkClient *m_client;
    QStackedWidget *m_stackedWidget;
    LoginWindow *m_loginWindow;
    ChatWidget *m_chatWidget;
    QString m_serverHost;
    int m_serverPort;
};

#endif // MAINWINDOW_H
