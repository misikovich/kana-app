#include "FenetreKana.h"

FenetreKana::FenetreKana()
{
    widget_kana = new WidgetKana();
    setCentralWidget(widget_kana);
    widget_quizz= new WidgetQuizz();
    WidgetPrincipal *widget_principal= new WidgetPrincipal();

    QObject::connect(widget_principal->getButtonLearn(),SIGNAL(clicked()),this,SLOT(lanceWidgetLearning()));
    QObject::connect(widget_principal->getButtonQuizz(),SIGNAL(clicked()),this,SLOT(lanceWidgetQuizz()));


    setWindowTitle("Kana Learning");
    widget_fin_quizz = new WidgetFinQuizz;
    stacked_widgets = new QStackedWidget;
    stacked_widgets->addWidget(widget_principal);
    stacked_widgets->addWidget(widget_kana);
    stacked_widgets->addWidget(widget_quizz);
    stacked_widgets->addWidget(widget_fin_quizz);
    setCentralWidget(stacked_widgets);
    QObject::connect(stacked_widgets, SIGNAL(currentChanged(int)), this, SLOT(fadeInWidget(int)));

    QObject::connect(widget_quizz, SIGNAL(finPartie()), this, SLOT(lanceFinQuiz()));

    creationMenu();
    creationToolBarKana();
    creationToolBarQuizz();

}

void FenetreKana::lanceWidgetLearning()
{
    stacked_widgets->setCurrentIndex(1);
    toolBarKana->setVisible(true);


}
//permet l'animation
void FenetreKana::fadeInWidget(int index)
{
    if (faderWidget)
        faderWidget->close();
    faderWidget = new FaderWidget(stacked_widgets->widget(index));
    faderWidget->start();
}

void FenetreKana::lanceWidgetQuizz()
{
    stacked_widgets->setCurrentIndex(2);
    WidgetQuizz *quizz = (WidgetQuizz*)stacked_widgets->widget(2);
    // chargement du score
    quizz->initialisation(chargerScore(cheminScore()));
    toolBarQuizz->setVisible(true);
}

void FenetreKana::retourFenetrePrincipale()
{
    //arrête le quizz en cours pour que le timer ne termine pas la partie en arrière-plan
    widget_quizz->stoppePartie();
    stacked_widgets->setCurrentIndex(0);
    toolBarKana->setVisible(false);
    toolBarQuizz->setVisible(false);
    //récupère le focus du clavier et évite les conflits entre les différents widgets
    setFocus();
}

void FenetreKana::lanceFinQuiz()
{
    //affichage du score
    widget_fin_quizz->setScore(widget_quizz->getScore());
    stacked_widgets->setCurrentIndex(3);

    //enregistrement du score
    sauverMeilleurScore(cheminScore(),widget_quizz->getScore());
}



void FenetreKana::creationMenu()
{

    QMenu *menu_file = menuBar()->addMenu("About");
    QAction *action_note = menu_file->addAction("Notes");
    QObject::connect(action_note,SIGNAL(triggered()),this,SLOT(affichePopupNote()));
}

/* fonction permettant la gestion des évènements clavier */
void FenetreKana::keyPressEvent(QKeyEvent* event)
{
    if (stacked_widgets->currentWidget()==widget_kana)
    {
        if ( event->key() == Qt::Key_Left )
            widget_kana->moveToLeft();
        else if (event->key() == Qt::Key_Right)
            widget_kana->moveToRight();
        else if (event->key() == Qt::Key_Up)
        {
            int index = liste_deroulante_kana->findText("Hiragana");
            liste_deroulante_kana->setCurrentIndex(index);
        }
        else if (event->key() == Qt::Key_Down)
        {
            int index = liste_deroulante_kana->findText("Katakana");
            liste_deroulante_kana->setCurrentIndex(index);
        }

        else if (event->key() == Qt::Key_Space)
            widget_kana->afficheSpoil();
    }
}


void FenetreKana::affichePopupNote()
{
    QMessageBox::information(this,"About","Adonis Najimi \n DUT INFORMATIQUE \n NANCY CHARLEMAGNE");
}

void FenetreKana::creationToolBarKana()
{
    toolBarKana = addToolBar("Choix Kana");
    toolBarKana->setMovable(false);
    toolBarKana->setFloatable(false);
    toolBarKana->setVisible(false);

    //cette ligne empêche la barre d'être supprimée
    toolBarKana->setContextMenuPolicy(Qt::PreventContextMenu);

    liste_deroulante_kana = new QComboBox();
    liste_deroulante_kana->addItem("Hiragana");
    liste_deroulante_kana->addItem("Katakana");


    // layout toolbar
    QPushButton *bouton_retour = new QPushButton("Retour");



    bouton_retour->setIcon(QIcon(":/images/ressources/images/retour.png"));


    toolBarKana->addWidget(bouton_retour);
    QObject::connect(bouton_retour,SIGNAL(clicked()),this,SLOT(retourFenetrePrincipale()));
    toolBarKana->addWidget(new QLabel("<i>Choix Kana</i>"));
    toolBarKana->addWidget(liste_deroulante_kana);

    //liaison de la liste avec le widget gérant les kanas
    QObject::connect(liste_deroulante_kana,SIGNAL(currentTextChanged(QString)),widget_kana,SLOT(setKana(QString)));



}
void FenetreKana::creationToolBarQuizz()
{
    toolBarQuizz = addToolBar("Retour Menu");
    toolBarQuizz->setMovable(false);
    toolBarQuizz->setFloatable(false);
    toolBarQuizz->setVisible(false);

    //cette ligne empêche la barre d'être supprimée
    toolBarQuizz->setContextMenuPolicy(Qt::PreventContextMenu);

    QPushButton *bouton_retour = new QPushButton("Retour");
      bouton_retour->setIcon(QIcon(":/images/ressources/images/retour.png"));
      QWidget *bg = new QWidget;
      QVBoxLayout *bg_layout = new QVBoxLayout;
      bg_layout->addWidget(bouton_retour,0,Qt::AlignCenter | Qt::AlignHCenter);
      bg->setLayout(bg_layout);
      toolBarQuizz->addWidget(bg);

      QObject::connect(bouton_retour,SIGNAL(clicked()),this,SLOT(retourFenetrePrincipale()));
}



