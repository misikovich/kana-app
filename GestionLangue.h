#ifndef GESTIONLANGUE_H
#define GESTIONLANGUE_H
#include <QString>
#include <QStringList>

//codes des langues disponibles ( "fr" est la langue des sources, sans traduction )
QStringList languesDisponibles();

//nom de la langue dans cette langue, pour le menu ( ex: "Русский" )
QString nomLangue(QString code);

//langue choisie lors d'un précédent lancement, sinon celle du système si elle est disponible, sinon "fr"
QString langueInitiale();

//installe la traduction de la langue en paramètre et mémorise le choix
//les widgets reçoivent ensuite un QEvent::LanguageChange
void appliqueLangue(QString code);

//langue actuellement appliquée
QString langueActuelle();

#endif // GESTIONLANGUE_H
