#include "TimeCompteur.h"

//animation du timer
TimeCompteur::TimeCompteur()
{
    derniere_valeur=0;

}


// debut : premier chiffre du décompte, time : temps à chaque décompte
TimeCompteur::TimeCompteur(int debut,int time)
{
    m_debut=debut;
    debut_compteur=debut;
    derniere_valeur=0;

//création des labels servant à l'animation
    QLabel *label1 = new QLabel(QString::number(m_debut));
    label1->setAlignment(Qt::AlignHCenter);

    //QLabel label2(QString::number(m_debut));

    stacked_widget = new QStackedWidget;

    stacked_widget->addWidget(label1);

    QFont police = QFont("Ubuntu",20);
    label1->setFont(police);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(stacked_widget);



    // initialisation du timer
    timer=new QTimer();
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(decrementeCompteur()));


}

void TimeCompteur::setBegin(int bg)
{
    m_debut=bg;
    initialiseCompteur();
}

void TimeCompteur::stoppeCompteur()
{
    timer->stop();
}

int TimeCompteur::getDerniereValeur()
{
    return derniere_valeur;
}

void TimeCompteur::initialiseCompteur()
{
    QLabel *label = (QLabel*)(stacked_widget->widget(0));
    label->setText(QString::number(m_debut));
    derniere_valeur=m_debut;
}

void TimeCompteur::lanceCompteur(int time)
{
    timer->start(time);
}

void TimeCompteur::decrementeCompteur()
{
    //récupération du nombre actuel dans le label
    QLabel *label = (QLabel*)(stacked_widget->widget(0));
    int compteur = label->text().toInt();
    compteur--;
    derniere_valeur=compteur;
    // modification du label
    label->setText(QString::number(compteur));
    //animation : changement du widget
    stacked_widget->setCurrentIndex(0);

    /* si le compteur est négatif ( donc qu'il est arrivé à sa fin )
     * on stoppe le compteur et on l'initialise à la valeur
     * initiale */

    if (compteur<0)
    {
        stoppeCompteur();
        emit finCompteur();
        initialiseCompteur();
    }

}




