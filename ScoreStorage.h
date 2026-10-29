#ifndef SCORESTORAGE_H
#define SCORESTORAGE_H
#include <QString>
#include <QApplication>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

//returns the path of the file containing the best score
//the quiz and unlimited time have their own best scores
QString scoreFilePath(bool unlimited=false, bool quiz=false);

//returns the score located in the file passed as parameter
int loadScore(QString);

//takes the path to the file and the score as parameters
// only saves if better than the best score
void saveBestScore(QString,int);


void saveScore(QString,int);

#endif // SCORESTORAGE_H
