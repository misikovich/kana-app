#ifndef GESTIONSCORE_H
#define GESTIONSCORE_H
#include <QString>
#include <QApplication>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

//retourne le chemin du fichier contenant le meilleur score
QString cheminScore();

//retourne le score situé dans le fichier en paramètre
int chargerScore(QString);

//prend en paramètre le chemin vers le fichier et le score
// enregistre seulement si supérieur au meilleur score
void sauverMeilleurScore(QString,int);


void sauverScore(QString,int);

#endif // GESTIONSCORE_H
