#include <QApplication>
#include <QPushButton>
#include <QtWidgets>

#include "MainWindow.h"
#include "Language.h"
#include "Theme.h"
int main(int argc, char *argv[])
{   QApplication app(argc,argv);


    QCoreApplication::setOrganizationName("Najimi");
    QCoreApplication::setApplicationName("Kana Learning");
    //after the names: QSettings depends on them to find the chosen language
    applyLanguage(initialLanguage());
    loadFonts();
    applyTheme(initialTheme());

    MainWindow window;
    window.resize(760,680);
    window.show();
    return app.exec();
}
