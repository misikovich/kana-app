#include "WidgetQuizz.h"


//affichage des quizz
WidgetQuizz::WidgetQuizz()
{
    liste_kana = new ListeKana;
    score=0;
    nb_fautes=0;
    niveau=1;
    affilee=false;
    bonus=false;
    choix_niveau_widget=nullptr;
    QFont police_jap = QFont("Ubuntu",180);
    QFont police_romaji = QFont("Ubuntu",120);
    kana_cherche=new QLabel();
    kana_cherche->setPixmap(QPixmap(":/images/ressources/images/kana_quizz.png"));
    kana_cherche->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    reponse_instantanee = new QLabel();
    reponse_instantanee->setFont(police_romaji);
    kana_cherche->setFont(police_jap);
    reponse_input = new QLineEdit;
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(kana_cherche);
    layout->addWidget(reponse_input);

    reponse_input->setMaxLength(3);
    reponse_instantanee->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    QObject::connect(reponse_input,SIGNAL(textEdited(QString)),reponse_instantanee,SLOT(setText(QString)));
    QObject::connect(reponse_input,SIGNAL(textEdited(QString)),this,SLOT(gereReponse(QString)));

    score_label=new QLabel("Meilleur Score :");
    //lecture d'un fichier score
    layout->addWidget(score_label);

    //gestion animation reponse disparition
    stacked_widgets = new QStackedWidget;
    stacked_widgets->addWidget(reponse_instantanee);

    // label OK! qui devient ":(" quand mauvaise réponse
    reponse_ok = new QLabel("<font color='green'><center>OK!</center></font>");
    reponse_ok->setFont(police_romaji);
    stacked_widgets->addWidget(reponse_ok);

    layout->addWidget(stacked_widgets);
    QObject::connect(stacked_widgets, SIGNAL(currentChanged(int)), this, SLOT(fadeInWidget(int)));


    //gestion du décompte
    compteur = new TimeCompteur(10,1000);
    layout->addWidget(compteur);
    QObject::connect(compteur,SIGNAL(finCompteur()),this,SLOT(lanceQuestion()));

    //apparition une fois le niveau choisi
    compteur->setVisible(false);
    reponse_input->setVisible(false);




    createLevelOne();

}
//initialisation du widget : affichage du bouton commencer et reset du timer
void WidgetQuizz::initialisation(int meilleur_score)
{
    score_label->setText("Meilleur score: "+QString::number(meilleur_score));
    stacked_widgets->setCurrentWidget(bouton_lancer_timer);
    compteur->stoppeCompteur();
    compteur->setVisible(false);
    kana_cherche->setPixmap(QPixmap(":/images/ressources/images/kana_quizz.png"));
    reponse_input->setVisible(false);
    score=0;
    nb_fautes=0;
    affilee=false;
    bonus=false;

}

//arrête la partie en cours ( retour au menu principal )
void WidgetQuizz::stoppePartie()
{
    compteur->stoppeCompteur();
}


// slot activé quand clique sur bouton niveau 1,2,3
void WidgetQuizz::creationCompteur(int debut)
{
    compteur->setBegin(debut);
    compteur->setVisible(true);
    reponse_input->setVisible(true);
}

//affiche la réponse quand correcte ou timer dépassé
void WidgetQuizz::afficheReponse()
{

}

int WidgetQuizz::getScore()
{
    return score;
}

// slot quand le timer est arrivé à sa fin, gère nb faute et initialisation des labels
//cette méthode est déclenchée seulement quand le compteur arrive à sa fin !
void WidgetQuizz::lanceQuestion()
{


    //on augmente le nombre de fautes quand le timer est arrivé à sa fin
    nb_fautes++;
    //la série de bonnes réponses est cassée
    affilee=false;

    // si le nombre de fautes est >5 on stoppe tout
    if (nb_fautes>5)
    {
        compteur->stoppeCompteur();
        emit finPartie();
        return;
    }

    nouvelleQuestion();
}

void WidgetQuizz::updateScore()
{
    //affichage du score et du nombre de fautes
    QString chaine_score("Score : "+QString::number(score));

    if (nb_fautes!=0)
        chaine_score.append("   Fautes: "+QString::number(nb_fautes));
    score_label->setText(chaine_score);
}





// affiche des niveaux
void WidgetQuizz::choixNiveau()
{
    //le menu des niveaux n'est créé qu'une seule fois
    if (choix_niveau_widget)
    {
        stacked_widgets->setCurrentWidget(choix_niveau_widget);
        return;
    }

    QWidget *choix_niveau  = new QWidget;
    choix_niveau_widget = choix_niveau;
    QVBoxLayout *layout = new QVBoxLayout(choix_niveau);
    QPushButton *bouton1 = new QPushButton("10 secondes");
    QPushButton *bouton2 = new QPushButton("20 secondes");
    QPushButton *bouton3 = new QPushButton("30 secondes");


    layout->addWidget(bouton1);
    layout->addWidget(bouton2);
    layout->addWidget(bouton3);

    stacked_widgets->addWidget(choix_niveau);
    stacked_widgets->setCurrentWidget(choix_niveau);


    //chercher une façon de mieux coder :
    QObject::connect(bouton1,SIGNAL(clicked()),this,SLOT(niveau1()));
    QObject::connect(bouton2,SIGNAL(clicked()),this,SLOT(niveau2()));
    QObject::connect(bouton3,SIGNAL(clicked()),this,SLOT(niveau3()));

}

// chercher comment récuperer source signal
void WidgetQuizz::niveau1()
{
    creationCompteur(10);
    stacked_widgets->setCurrentWidget(reponse_instantanee);
    niveau=3;
    score_label->setText("Score:");

    nouvelleQuestion();
}

void WidgetQuizz::niveau2()
{
    creationCompteur(20);
    stacked_widgets->setCurrentWidget(reponse_instantanee);
    niveau=2;
    score_label->setText("Score:");

    nouvelleQuestion();

}

void WidgetQuizz::niveau3()
{
    creationCompteur(30);
    stacked_widgets->setCurrentWidget(reponse_instantanee);
    score_label->setText("Score:");
    niveau=1;

    nouvelleQuestion();



}

//fonction d'affilée ou non avec boolean ou juste le compteur!
int WidgetQuizz::calculScore(int score)
{
    int gain = ((compteur->getDerniereValeur()%10))+(niveau);
    //le bonus double les points de la question, pas le score total
    if (bonus)
    {
        gain*=2;
    }
    return score+gain;
}

// on passe à la question suivante ( le kana suivant ) + initialisation du timer
void WidgetQuizz::nouvelleQuestion()
{
    //on met à jour le score à chaque nouvelle question
    updateScore();


    // initialisation de l'ancien message tapé par l'utilisateur
    reponse_instantanee->setText("");

    // on tire un hasard un kana et on l'affiche
    Kana* kana_hasard = liste_kana->getShuffledKana();
    solution = kana_hasard->getRomaji();
    kana_cherche->setText("<center>"+kana_hasard->getJap()+"</center>");

    //on vide le champ de texte avant de réécrire à nouveau dedans
    reponse_input->setText("");

    // création d'un timer pour chaque question, si dépassement on change la question

    compteur->initialiseCompteur();
    compteur->lanceCompteur();
}

//affiche OK et gère animation des affichages + augmentation du score
void WidgetQuizz::gereReponse(QString reponse)
{
    //animation pour chaque lettre
    stacked_widgets->setCurrentWidget(reponse_ok);
    stacked_widgets->setCurrentWidget(reponse_instantanee);

    //affichage kana complet quand timer fin ou kana juste

    if (verifieReponse(reponse))
    {
       //gestion du booléen affilée puis augmentation du score,
       //avant que nouvelleQuestion() ne réinitialise le compteur
       bonus=affilee;
       score=calculScore(score);
       nouvelleQuestion();
       //mise à jour du score après le calcul
       updateScore();

       //affichage OK
       stacked_widgets->setCurrentWidget(reponse_ok);
       affilee=true;
    }

}



// verifie la réponse et colore les lettres en fonction de leur véracité
bool WidgetQuizz::verifieReponse(QString reponse)
{
    //corriger bug quand solution a 3 lettres

    if (solution.size()==3)
    {
        if (reponse.size()==2 && reponse[0]==solution[0] && reponse[1]==solution[1] )
        {
            coloreGreenText();
        }
        else if (reponse.size()==1 && reponse[0]==solution[0] )
        {
            coloreGreenText();
        }
        else
        {
            coloreRedText();
        }

    }
    else if (solution.size()==2)
    {
        if (reponse.size()==1 && reponse[0]==solution[0])
        {
            coloreGreenText();
        }
        else
        {
            coloreRedText();
        }
     }
    else if (solution.size()==1)
        if (solution==reponse)
            coloreGreenText();
        else coloreRedText();
    else if (solution==reponse)
        coloreGreenText();

    return solution==reponse;
}



//colore la réponse en rouge
void WidgetQuizz::coloreRedText()
{
    reponse_instantanee->setText("<font color='red'>"+reponse_instantanee->text()+"</font>");
}
// colore la réponse en vert
void WidgetQuizz::coloreGreenText()
{

    reponse_instantanee->setText("<font color='green'>"+reponse_instantanee->text()+"</font>");
}


//gestion animation lors du changement de widget dans la pile de widgets
void WidgetQuizz::fadeInWidget(int index)
{
    if (faderWidget)
        faderWidget->close();
    faderWidget = new FaderWidget(stacked_widgets->widget(index));
    faderWidget->start();
}
void WidgetQuizz::creationMenu()
{
    QWidget *niveau1,*niveau2,*niveau3;
    niveau1 = new QWidget;
    niveau2 = new QWidget;
    niveau3 = new QWidget;
    choix_niveau = new QStackedWidget;
    choix_niveau->addWidget(niveau1);
    choix_niveau->addWidget(niveau2);
    choix_niveau->addWidget(niveau3);

    QPushButton *button_1 = new QPushButton("Niveau 1");
    QPushButton *button_2 = new QPushButton("Niveau 2");
    QPushButton *button_3 = new QPushButton("Niveau 3");
    QVBoxLayout *layout = new QVBoxLayout(this);


    //style des boutons, faire une fonction
    button_1->setStyleSheet(" QPushButton {"
                                       "     background-color: #F22C0E;"
                                       "     border-style: outset;"
                                       "     border-width: 2px;"
                                       "     border-radius: 10px;"
                                       "     border-color: beige;"
                                       "     font: bold 14px;"
                                       "     min-width: 10em;"
                                       "     padding: 6px;"
                                       " }"
                                       " QPushButton:pressed {"
                                       "     background-color: #C6240B;"
                                       "     border-style: inset;"
                                       " }");
    button_2->setStyleSheet(" QPushButton {"
                                       "     background-color: #F4C464;"
                                       "     border-style: outset;"
                                       "     border-width: 2px;"
                                       "     border-radius: 10px;"
                                       "     border-color: beige;"
                                       "     font: bold 14px;"
                                       "     min-width: 10em;"
                                       "     padding: 6px;"
                                       " }"
                                       " QPushButton:pressed {"
                                       "     background-color: #D37828;"
                                       "     border-style: inset;"
                                       " }");
    button_3->setStyleSheet(" QPushButton {"
                                       "     background-color: #FAFF00;"
                                       "     border-style: outset;"
                                       "     border-width: 2px;"
                                       "     border-radius: 10px;"
                                       "     border-color: beige;"
                                       "     font: bold 14px;"
                                       "     min-width: 10em;"
                                       "     padding: 6px;"
                                       " }"
                                       " QPushButton:pressed {"
                                       "     background-color: #E1E500;"
                                       "     border-style: inset;"
                                       " }");

    layout->addWidget(button_1);
    layout->addWidget(button_2);
    layout->addWidget(button_3);
}


void WidgetQuizz::createLevelOne()
{
    bouton_lancer_timer = new QPushButton("Commencer");
    QObject::connect(bouton_lancer_timer,SIGNAL(clicked()),this,SLOT(choixNiveau()));
    stacked_widgets->addWidget(bouton_lancer_timer);


    QLabel *bg = new QLabel("<center>Apprenez les kanas en vous amusant ! </center>",bouton_lancer_timer);

    bouton_lancer_timer->setStyleSheet(" QPushButton {"
                                       "     background-color: #22BBD6;"
                                       "     border-style: outset;"
                                       "     border-width: 2px;"
                                       "     border-radius: 10px;"
                                       "     border-color: beige;"
                                       "     font: bold 14px;"
                                       "     min-width: 10em;"
                                       "     padding: 6px;"
                                       "     margin-left:auto;"
                                       "     margin-right:auto;"
                                       "     margin-bottom:auto;"
                                       "     margin-top:auto;"
                                       " }"
                                       " QPushButton:pressed {"
                                       "     background-color: #2e5d7b;"
                                       "     border-style: inset;"
                                       " }");
}
