#include <QGuiApplication>
#include <QMainWindow>

#include 

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Ma première application Qt");
    mainWindow.show();

    return app.exec();
}
