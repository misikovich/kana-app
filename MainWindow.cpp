#include "MainWindow.h"

MainWindow::MainWindow()
{
    kana_widget = new KanaWidget();
    setCentralWidget(kana_widget);
    practice_widget= new PracticeWidget();
    MainMenuWidget *main_menu_widget= new MainMenuWidget();

    QObject::connect(main_menu_widget->getButtonLearn(),SIGNAL(clicked()),this,SLOT(showLearning()));
    QObject::connect(main_menu_widget->getButtonPractice(),SIGNAL(clicked()),this,SLOT(showPractice()));


    setWindowTitle("Kana Learning");
    practice_end_widget = new PracticeEndWidget;
    stacked_widgets = new QStackedWidget;
    stacked_widgets->addWidget(main_menu_widget);
    stacked_widgets->addWidget(kana_widget);
    stacked_widgets->addWidget(practice_widget);
    stacked_widgets->addWidget(practice_end_widget);
    setCentralWidget(stacked_widgets);
    QObject::connect(stacked_widgets, SIGNAL(currentChanged(int)), this, SLOT(fadeInWidget(int)));

    QObject::connect(practice_widget, SIGNAL(gameOver()), this, SLOT(showPracticeEnd()));

    createMenu();
    createKanaToolBar();
    createPracticeToolBar();
    retranslate();

}

void MainWindow::showLearning()
{
    stacked_widgets->setCurrentIndex(1);
    kanaToolBar->setVisible(true);


}
//enables the animation
void MainWindow::fadeInWidget(int index)
{
    if (faderWidget)
        faderWidget->close();
    faderWidget = new FaderWidget(stacked_widgets->widget(index));
    faderWidget->start();
}

void MainWindow::showPractice()
{
    stacked_widgets->setCurrentIndex(2);
    practice_widget->reset();
    practiceToolBar->setVisible(true);
}

void MainWindow::showMainMenu()
{
    //stops the current game so that the timer does not end the game in the background
    practice_widget->stopGame();
    stacked_widgets->setCurrentIndex(0);
    kanaToolBar->setVisible(false);
    practiceToolBar->setVisible(false);
    //takes back the keyboard focus and avoids conflicts between the different widgets
    setFocus();
}

void MainWindow::showPracticeEnd()
{
    //display of the score ( PracticeWidget saves the best score )
    practice_end_widget->setScore(practice_widget->getScore());
    stacked_widgets->setCurrentIndex(3);
}



void MainWindow::createMenu()
{

    about_menu = menuBar()->addMenu("");
    note_action = about_menu->addAction("");
    QObject::connect(note_action,SIGNAL(triggered()),this,SLOT(showAboutDialog()));

    //language menu: one checkable action per language, the name of each language is not translated
    language_menu = menuBar()->addMenu("");
    QActionGroup *language_group = new QActionGroup(this);
    for (const QString &code : availableLanguages())
    {
        QAction *action = language_menu->addAction(languageName(code));
        action->setData(code);
        action->setCheckable(true);
        action->setChecked(code==currentLanguage());
        language_group->addAction(action);
    }
    QObject::connect(language_group,SIGNAL(triggered(QAction*)),this,SLOT(changeLanguage(QAction*)));
}

void MainWindow::changeLanguage(QAction* action)
{
    applyLanguage(action->data().toString());
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type()==QEvent::LanguageChange)
        retranslate();
    QMainWindow::changeEvent(event);
}

void MainWindow::retranslate()
{
    about_menu->setTitle(tr("About"));
    note_action->setText(tr("Notes"));
    language_menu->setTitle(tr("Language"));
    kanaToolBar->setWindowTitle(tr("Kana choice"));
    practiceToolBar->setWindowTitle(tr("Back to menu"));
    kana_choice_label->setText("<i>"+tr("Kana choice")+"</i>");
    kana_combo_box->setItemText(0,tr("Hiragana"));
    kana_combo_box->setItemText(1,tr("Katakana"));
    kana_back_button->setText(tr("Back"));
    practice_back_button->setText(tr("Back"));
}

/* function handling keyboard events */
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (stacked_widgets->currentWidget()==kana_widget)
    {
        if ( event->key() == Qt::Key_Left )
            kana_widget->moveToLeft();
        else if (event->key() == Qt::Key_Right)
            kana_widget->moveToRight();
        else if (event->key() == Qt::Key_Up)
        {
            kana_combo_box->setCurrentIndex(0);
        }
        else if (event->key() == Qt::Key_Down)
        {
            kana_combo_box->setCurrentIndex(1);
        }

        else if (event->key() == Qt::Key_Space)
            kana_widget->toggleRomaji();
    }
}


void MainWindow::showAboutDialog()
{
    QMessageBox::information(this,tr("About"),"Adonis Najimi \n DUT INFORMATIQUE \n NANCY CHARLEMAGNE");
}

void MainWindow::createKanaToolBar()
{
    kanaToolBar = addToolBar("");
    kanaToolBar->setMovable(false);
    kanaToolBar->setFloatable(false);
    kanaToolBar->setVisible(false);

    //this line prevents the bar from being removed
    kanaToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    kana_combo_box = new QComboBox();
    //index 0: hiragana, index 1: katakana (the texts are translated in retranslate() )
    kana_combo_box->addItem("");
    kana_combo_box->addItem("");


    // toolbar layout
    kana_back_button = new QPushButton;
    kana_back_button->setIcon(QIcon(":/images/ressources/images/retour.png"));

    kanaToolBar->addWidget(kana_back_button);
    QObject::connect(kana_back_button,SIGNAL(clicked()),this,SLOT(showMainMenu()));
    kana_choice_label = new QLabel;
    kanaToolBar->addWidget(kana_choice_label);
    kanaToolBar->addWidget(kana_combo_box);

    //connection of the list with the widget handling the kanas
    QObject::connect(kana_combo_box,SIGNAL(currentIndexChanged(int)),kana_widget,SLOT(setScript(int)));



}
void MainWindow::createPracticeToolBar()
{
    practiceToolBar = addToolBar("");
    practiceToolBar->setMovable(false);
    practiceToolBar->setFloatable(false);
    practiceToolBar->setVisible(false);

    //this line prevents the bar from being removed
    practiceToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    practice_back_button = new QPushButton;
      practice_back_button->setIcon(QIcon(":/images/ressources/images/retour.png"));
      QWidget *bg = new QWidget;
      QVBoxLayout *bg_layout = new QVBoxLayout;
      bg_layout->addWidget(practice_back_button,0,Qt::AlignCenter | Qt::AlignHCenter);
      bg->setLayout(bg_layout);
      practiceToolBar->addWidget(bg);

      QObject::connect(practice_back_button,SIGNAL(clicked()),this,SLOT(showMainMenu()));
}

