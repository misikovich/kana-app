#include "ScoreStorage.h"

//path of the score file, in the user data directory
//( the executable's directory is not always writable, e.g. Program Files )
QString scoreFilePath(bool unlimited)
{
    QString directory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(directory);
    return directory+(unlimited ? "/score_data_unlimited" : "/score_data");
}

//takes the path to the file and the score as parameters
// only saves if better than the best score
void saveBestScore(QString file_name,int score)
{
    // the file is opened for writing ( which clears it ) only if the score is better
    if (score <= loadScore(file_name))
        return;

    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "could not write the score to" << file_name;
        return;
    }
    QTextStream out(&file);
    out << score;
}


//returns the score located in the file passed as parameter
int loadScore(QString file_name)
{
    int score=0;
    QFile file(file_name);
    // no file yet: no game played
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;
    QTextStream in(&file);
    in >> score;
    file.close();
    return score;

}

void saveScore(QString file_name,int score)
{
    QFile log_file(file_name);

    // Opening / Creation of the file
    if(!log_file.exists()){
         log_file.open(QIODevice::WriteOnly | QIODevice::Text);
    } else {
         log_file.open(QIODevice::Append | QIODevice::Text);
    }

    QTextStream stream(&log_file);
    // We write into the file
    stream << QString::number(score) << "\n";
    // Closing of the file
    log_file.close();


}
