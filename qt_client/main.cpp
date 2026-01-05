#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
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
