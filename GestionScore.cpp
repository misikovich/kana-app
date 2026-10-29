#include "GestionScore.h"

//chemin du fichier de score, dans le dossier de données de l'utilisateur
//( le dossier de l'exécutable n'est pas toujours accessible en écriture, ex: Program Files )
QString cheminScore()
{
    QString dossier = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dossier);
    return dossier+"/score_data";
}

//prend en paramètre le chemin vers le fichier et le score
// enregistre seulement si supérieur au meilleur score
void sauverMeilleurScore(QString fichier_nom,int score)
{
    // on n'ouvre le fichier en écriture ( ce qui l'efface ) que si le score est meilleur
    if (score <= chargerScore(fichier_nom))
        return;

    QFile file_w(fichier_nom);
    if (!file_w.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "impossible d'écrire le score dans" << fichier_nom;
        return;
    }
    QTextStream out(&file_w);
    out << score;
}


//retourne le score situé dans le fichier en paramètre
int chargerScore(QString fichier_nom)
{
    int score=0;
    QFile fichier(fichier_nom);
    // pas encore de fichier : aucune partie jouée
    if (!fichier.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;
    QTextStream flux(&fichier);
    flux >> score;
    fichier.close();
    return score;

}

void sauverScore(QString fichier_nom,int score)
{
    QFile file_log(fichier_nom);

    // Ouverture / Création du fichier
    if(!file_log.exists()){
         file_log.open(QIODevice::WriteOnly | QIODevice::Text);
    } else {
         file_log.open(QIODevice::Append | QIODevice::Text);
    }

    QTextStream stream(&file_log);
    // On écrit dans le fichier
    stream << QString::number(score) << "\n";
    // Fermeture du fichier
    file_log.close();


}
