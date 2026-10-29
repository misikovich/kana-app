#ifndef WIDGETQUIZZ_H
#define WIDGETQUIZZ_H
#include <QWidget>
#include <QTime>
#include <QtGlobal>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ListeKana.h"
#include <QCoreApplication>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include "FaderWidget.h"
#include <QPointer>
#include <QStackedWidget>
#include <QTimer>
#include <QLCDNumber>
#include "TimeCompteur.h"


class WidgetQuizz : public QWidget
{
    Q_OBJECT
public:
    WidgetQuizz();
    void creationMenu();
    void createLevelOne(); //changer nom
    bool verifieReponse(QString);
    void coloreGreenText();
    void coloreRedText();
    void creationCompteur(int debut=10);
    void nouvelleQuestion();
    int calculScore(int x);
    void afficheReponse();
    void initialisation(int); // en paramètre le meilleur score
    int getScore();
    void stoppePartie();
public slots:
    void gereReponse(QString);
    void fadeInWidget(int index);
    void lanceQuestion();
    void choixNiveau();
    void niveau1();
    void niveau2();
    void niveau3();
    void updateScore();

signals:
    void finPartie(); // permet l'affichage d'une nouvelle page dans FenetreKana
private:
    ListeKana* liste_kana;
    QString reponse;
    QString solution;
    QLineEdit *reponse_input;
    int score;
    int niveau; // permet d'augmenter le score plus facilement
    QStackedWidget *choix_niveau;
    //menu des niveaux, créé au premier clic sur "Commencer"
    QWidget *choix_niveau_widget;
    QLabel *kana_cherche;
    QLabel *reponse_instantanee;
    QLabel *score_label;
    //permet l'animation :
    QPointer<FaderWidget> faderWidget;
    //liste de widgets qlabel OK et réponse
    QStackedWidget *stacked_widgets;
    //timer pour chaque kana
    TimeCompteur *compteur;
    //nombre maximal de kanas qui défilent
    int nb_kana;
    //numéro du kana actuel
    int compteur_kana;
    // permet de savoir si les questions sont rentrées correctement d'affilée
    bool affilee;
    // active les bonus
    bool bonus;
    //bouton pour lancer le quizz
    QPushButton *bouton_lancer_timer;
    //compteur de fautes
    int nb_fautes;
    //label qui affiche OK! ou :( en fonction de la réponse
    QLabel *reponse_ok;
};

#endif // WIDGETQUIZZ_H
