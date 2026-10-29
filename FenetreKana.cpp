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
    retraduit();

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

    menu_about = menuBar()->addMenu("");
    action_note = menu_about->addAction("");
    QObject::connect(action_note,SIGNAL(triggered()),this,SLOT(affichePopupNote()));

    //menu des langues : une action cochable par langue, le nom de chaque langue n'est pas traduit
    menu_langue = menuBar()->addMenu("");
    QActionGroup *groupe_langues = new QActionGroup(this);
    for (const QString &code : languesDisponibles())
    {
        QAction *action = menu_langue->addAction(nomLangue(code));
        action->setData(code);
        action->setCheckable(true);
        action->setChecked(code==langueActuelle());
        groupe_langues->addAction(action);
    }
    QObject::connect(groupe_langues,SIGNAL(triggered(QAction*)),this,SLOT(changeLangue(QAction*)));
}

void FenetreKana::changeLangue(QAction* action)
{
    appliqueLangue(action->data().toString());
}

void FenetreKana::changeEvent(QEvent* event)
{
    if (event->type()==QEvent::LanguageChange)
        retraduit();
    QMainWindow::changeEvent(event);
}

void FenetreKana::retraduit()
{
    menu_about->setTitle(tr("À propos"));
    action_note->setText(tr("Notes"));
    menu_langue->setTitle(tr("Langue"));
    toolBarKana->setWindowTitle(tr("Choix Kana"));
    toolBarQuizz->setWindowTitle(tr("Retour Menu"));
    label_choix_kana->setText("<i>"+tr("Choix Kana")+"</i>");
    liste_deroulante_kana->setItemText(0,tr("Hiragana"));
    liste_deroulante_kana->setItemText(1,tr("Katakana"));
    bouton_retour_kana->setText(tr("Retour"));
    bouton_retour_quizz->setText(tr("Retour"));
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
            liste_deroulante_kana->setCurrentIndex(0);
        }
        else if (event->key() == Qt::Key_Down)
        {
            liste_deroulante_kana->setCurrentIndex(1);
        }

        else if (event->key() == Qt::Key_Space)
            widget_kana->afficheSpoil();
    }
}


void FenetreKana::affichePopupNote()
{
    QMessageBox::information(this,tr("À propos"),"Adonis Najimi \n DUT INFORMATIQUE \n NANCY CHARLEMAGNE");
}

void FenetreKana::creationToolBarKana()
{
    toolBarKana = addToolBar("");
    toolBarKana->setMovable(false);
    toolBarKana->setFloatable(false);
    toolBarKana->setVisible(false);

    //cette ligne empêche la barre d'être supprimée
    toolBarKana->setContextMenuPolicy(Qt::PreventContextMenu);

    liste_deroulante_kana = new QComboBox();
    //index 0 : hiragana, index 1 : katakana ( les textes sont traduits dans retraduit() )
    liste_deroulante_kana->addItem("");
    liste_deroulante_kana->addItem("");


    // layout toolbar
    bouton_retour_kana = new QPushButton;
    bouton_retour_kana->setIcon(QIcon(":/images/ressources/images/retour.png"));

    toolBarKana->addWidget(bouton_retour_kana);
    QObject::connect(bouton_retour_kana,SIGNAL(clicked()),this,SLOT(retourFenetrePrincipale()));
    label_choix_kana = new QLabel;
    toolBarKana->addWidget(label_choix_kana);
    toolBarKana->addWidget(liste_deroulante_kana);

    //liaison de la liste avec le widget gérant les kanas
    QObject::connect(liste_deroulante_kana,SIGNAL(currentIndexChanged(int)),widget_kana,SLOT(setKana(int)));



}
void FenetreKana::creationToolBarQuizz()
{
    toolBarQuizz = addToolBar("");
    toolBarQuizz->setMovable(false);
    toolBarQuizz->setFloatable(false);
    toolBarQuizz->setVisible(false);

    //cette ligne empêche la barre d'être supprimée
    toolBarQuizz->setContextMenuPolicy(Qt::PreventContextMenu);

    bouton_retour_quizz = new QPushButton;
      bouton_retour_quizz->setIcon(QIcon(":/images/ressources/images/retour.png"));
      QWidget *bg = new QWidget;
      QVBoxLayout *bg_layout = new QVBoxLayout;
      bg_layout->addWidget(bouton_retour_quizz,0,Qt::AlignCenter | Qt::AlignHCenter);
      bg->setLayout(bg_layout);
      toolBarQuizz->addWidget(bg);

      QObject::connect(bouton_retour_quizz,SIGNAL(clicked()),this,SLOT(retourFenetrePrincipale()));
}



