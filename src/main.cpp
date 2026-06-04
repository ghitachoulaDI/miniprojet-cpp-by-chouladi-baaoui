#include "gui/MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("StudentManager");
    QApplication::setOrganizationName("IMS");

    MainWindow window;
    window.show();
    return app.exec();
}
