#include <QApplication>
#include <QPushButton>
#include <QtWidgets>

#include "FenetreKana.h"
#include "GestionLangue.h"
int main(int argc, char *argv[])
{   QApplication app(argc,argv);


    QCoreApplication::setOrganizationName("Najimi");
    QCoreApplication::setApplicationName("Kana Learning");
    //après les noms : QSettings en dépend pour retrouver la langue choisie
    appliqueLangue(langueInitiale());

    FenetreKana fenetre;
    fenetre.show();
    return app.exec();
}
