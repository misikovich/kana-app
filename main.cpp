#include <QApplication>
#include <QPushButton>
#include <QtWidgets>

#include "MainWindow.h"
#include "Language.h"
int main(int argc, char *argv[])
{   QApplication app(argc,argv);


    QCoreApplication::setOrganizationName("Najimi");
    QCoreApplication::setApplicationName("Kana Learning");
    //after the names: QSettings depends on them to find the chosen language
    applyLanguage(initialLanguage());

    MainWindow window;
    window.show();
    return app.exec();
}
