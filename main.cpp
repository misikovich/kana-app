#include <QApplication>
#include <QPushButton>
#include <QtWidgets>

#include "FenetreKana.h"
int main(int argc, char *argv[])
{   QApplication app(argc,argv);


    QCoreApplication::setOrganizationName("Najimi");
    QCoreApplication::setApplicationName("Kana Learning");

    FenetreKana fenetre;
    fenetre.show();
    return app.exec();
}
