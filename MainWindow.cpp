#include "MainWindow.h"

MainWindow::MainWindow()
{
    kana_widget = new KanaWidget();
    setCentralWidget(kana_widget);
    practice_widget= new PracticeWidget();
    quiz_widget= new PracticeWidget(true);
    MainMenuWidget *main_menu_widget= new MainMenuWidget();

    QObject::connect(main_menu_widget->getButtonLearn(),SIGNAL(clicked()),this,SLOT(showLearning()));
    QObject::connect(main_menu_widget->getButtonPractice(),SIGNAL(clicked()),this,SLOT(showPractice()));
    QObject::connect(main_menu_widget->getButtonQuiz(),SIGNAL(clicked()),this,SLOT(showQuiz()));


    setWindowTitle("Kana Learning");
    practice_end_widget = new PracticeEndWidget;
    stacked_widgets = new QStackedWidget;
    stacked_widgets->addWidget(main_menu_widget);
    stacked_widgets->addWidget(kana_widget);
    stacked_widgets->addWidget(practice_widget);
    stacked_widgets->addWidget(practice_end_widget);
    stacked_widgets->addWidget(quiz_widget);
    setCentralWidget(stacked_widgets);
    QObject::connect(stacked_widgets, SIGNAL(currentChanged(int)), this, SLOT(fadeInWidget(int)));

    QObject::connect(practice_widget, SIGNAL(gameOver()), this, SLOT(showPracticeEnd()));
    QObject::connect(quiz_widget, SIGNAL(gameOver()), this, SLOT(showPracticeEnd()));

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

void MainWindow::showQuiz()
{
    stacked_widgets->setCurrentIndex(4);
    quiz_widget->reset();
    practiceToolBar->setVisible(true);
}

void MainWindow::showMainMenu()
{
    //stops the current game so that the timer does not end the game in the background
    practice_widget->stopGame();
    quiz_widget->stopGame();
    stacked_widgets->setCurrentIndex(0);
    kanaToolBar->setVisible(false);
    practiceToolBar->setVisible(false);
    //takes back the keyboard focus and avoids conflicts between the different widgets
    setFocus();
}

void MainWindow::showPracticeEnd()
{
    //display of the score ( PracticeWidget saves the best score )
    //the game that just ended is the practice or the quiz
    PracticeWidget *game = qobject_cast<PracticeWidget*>(sender());
    practice_end_widget->setScore(game ? game->getScore() : 0);
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

    //theme menu: follow the system, light or dark
    theme_menu = menuBar()->addMenu("");
    theme_group = new QActionGroup(this);
    for (const QString &code : availableThemes())
    {
        QAction *action = theme_menu->addAction("");
        action->setData(code);
        action->setCheckable(true);
        action->setChecked(code==currentTheme());
        theme_group->addAction(action);
    }
    QObject::connect(theme_group,SIGNAL(triggered(QAction*)),this,SLOT(changeTheme(QAction*)));
}

void MainWindow::changeTheme(QAction* action)
{
    applyTheme(action->data().toString());
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
    theme_menu->setTitle(tr("Theme"));
    for (QAction *action : theme_group->actions())
    {
        QString code = action->data().toString();
        action->setText(code=="light" ? tr("Light") : code=="dark" ? tr("Dark") : tr("System"));
    }
    script_group->button(0)->setText("あ  "+tr("Hiragana"));
    script_group->button(1)->setText("ア  "+tr("Katakana"));
    kana_back_button->setText("‹  "+tr("Back"));
    practice_back_button->setText("‹  "+tr("Back"));
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
            setScript(0);
        }
        else if (event->key() == Qt::Key_Down)
        {
            setScript(1);
        }

        else if (event->key() == Qt::Key_Space)
            kana_widget->toggleRomaji();
    }
}


void MainWindow::showAboutDialog()
{
    QMessageBox::information(this,tr("About"),"Adonis Najimi \n DUT INFORMATIQUE \n NANCY CHARLEMAGNE");
}

void MainWindow::setScript(int script)
{
    script_group->button(script)->setChecked(true);
    kana_widget->setScript(script);
}

//a back button, alone on the left of a toolbar
static QPushButton* createBackButton(QToolBar *toolbar)
{
    QPushButton *button = new QPushButton;
    button->setProperty("role","ghost");
    button->setFocusPolicy(Qt::NoFocus);
    toolbar->addWidget(button);
    return button;
}

void MainWindow::createKanaToolBar()
{
    kanaToolBar = addToolBar("");
    kanaToolBar->setMovable(false);
    kanaToolBar->setFloatable(false);
    kanaToolBar->setVisible(false);

    //this line prevents the bar from being removed
    kanaToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    kana_back_button = createBackButton(kanaToolBar);
    QObject::connect(kana_back_button,SIGNAL(clicked()),this,SLOT(showMainMenu()));

    //hiragana / katakana, as two toggle buttons on the right
    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    kanaToolBar->addWidget(spacer);
    script_group = new QButtonGroup(this);
    for (int i=0;i<2;i++)
    {
        QPushButton *button = new QPushButton;
        button->setCheckable(true);
        button->setFocusPolicy(Qt::NoFocus);
        script_group->addButton(button,i);
        kanaToolBar->addWidget(button);
    }
    script_group->button(0)->setChecked(true);

    //connection of the choice with the widget handling the kanas
    QObject::connect(script_group,SIGNAL(idClicked(int)),kana_widget,SLOT(setScript(int)));
}
void MainWindow::createPracticeToolBar()
{
    practiceToolBar = addToolBar("");
    practiceToolBar->setMovable(false);
    practiceToolBar->setFloatable(false);
    practiceToolBar->setVisible(false);

    //this line prevents the bar from being removed
    practiceToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    practice_back_button = createBackButton(practiceToolBar);
    QObject::connect(practice_back_button,SIGNAL(clicked()),this,SLOT(showMainMenu()));
}
