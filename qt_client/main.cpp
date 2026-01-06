#include "mainwindow.h"
#include <QApplication>
#include <QMetaType>
#include <QPair>
#include <QList>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Register meta-types used in queued signal/slot connections
    qRegisterMetaType<QList<QPair<QString, QString>>>("QList<QPair<QString, QString>>");
    qRegisterMetaType<QList<QPair<QString, bool>>>("QList<QPair<QString, bool>>");
    
    app.setApplicationName("Chat Application");
    app.setOrganizationName("Nhom11");
    app.setApplicationVersion("1.0");
    
    // Set application style
    app.setStyleSheet(
        "QMainWindow { background-color: #fafafa; }"
        "QMessageBox { background-color: white; }"
    );
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
