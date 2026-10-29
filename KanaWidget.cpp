#include "KanaWidget.h"
//widget to display the list of kanas
KanaWidget::KanaWidget()
{
    setMinimumWidth(300);
    hiragana=true;
    romaji_visible=true;
    kana_list = new KanaList();
    sound = new QSoundEffect(this);
    romaji_label = new QLabel();
    japanese_label = new QLabel();
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    QFont japanese_font = QFont("Ubuntu",180);
    japanese_label->setFont(japanese_font);
    QFont romaji_font = QFont("Ubuntu",130);
    romaji_label->setFont(romaji_font);

    main_layout->addWidget(japanese_label);
    main_layout->addWidget(romaji_label);

    QHBoxLayout* arrows_layout = new QHBoxLayout;

    previous_button = new QPushButton;
    next_button = new QPushButton;
    hide_romaji_button = new QPushButton;

    pronounce_button = new QPushButton;


    next_button->setStyleSheet(" QPushButton {"
                                       "     background-color: #C1E21B;"
                                       "     border-style: outset;"
                                       "     border-width: 2px;"
                                       "     border-radius: 10px;"
                                       "     border-color: beige;"
                                       "     font: bold 14px;"
                                       "     padding: 6px;"
                                       " }"
                                       " QPushButton:pressed {"
                                       "     background-color: #AAC417;"
                                       "     border-style: inset;"
                                       " }");
    previous_button->setStyleSheet(" QPushButton {"
                                       "     background-color: #C1E21B;"
                                       "     border-style: outset;"
                                       "     border-width: 2px;"
                                       "     border-radius: 10px;"
                                       "     border-color: beige;"
                                       "     font: bold 14px;"
                                       "     padding: 6px;"
                                       " }"
                                       " QPushButton:pressed {"
                                       "     background-color: #AAC417;"
                                       "     border-style: inset;"
                                       " }");

    hide_romaji_button->setStyleSheet(" QPushButton {"
                                       "     background-color: #22BBD6;"
                                       "     border-style: outset;"
                                       "     border-width: 2px;"
                                       "     border-radius: 10px;"
                                       "     border-color: beige;"
                                       "     font: bold 14px;"
                                       "     min-width: 10em;"
                                       "     padding: 6px;"
                                       " }"
                                       " QPushButton:pressed {"
                                       "     background-color: #2e5d7b;"
                                       "     border-style: inset;"
                                       " }");
    pronounce_button->setStyleSheet(" QPushButton {"
                                       "     background-color: #22BBD6;"
                                       "     border-style: outset;"
                                       "     border-width: 2px;"
                                       "     border-radius: 10px;"
                                       "     border-color: beige;"
                                       "     font: bold 14px;"
                                       "     min-width: 10em;"
                                       "     padding: 6px;"
                                       " }"
                                       " QPushButton:pressed {"
                                       "     background-color: #2e5d7b;"
                                       "     border-style: inset;"
                                       " }");


    main_layout->addWidget(pronounce_button);


    main_layout->addWidget(hide_romaji_button);
    arrows_layout->addWidget(previous_button);
    arrows_layout->addWidget(next_button);

    main_layout->addLayout(arrows_layout);
    // hide button connection
    QObject::connect(hide_romaji_button,SIGNAL(clicked()),this,SLOT(toggleRomaji()));
    // arrow button connections
    QObject::connect(previous_button,SIGNAL(clicked()),this,SLOT(moveToLeft()));
    QObject::connect(next_button,SIGNAL(clicked()),this,SLOT(moveToRight()));


    //pronunciation connection
    QObject::connect(pronounce_button,SIGNAL(clicked()),this,SLOT(pronounce()));

    retranslate();
    showCurrentKana();
}

void KanaWidget::changeEvent(QEvent* event)
{
    if (event->type()==QEvent::LanguageChange)
        retranslate();
    QWidget::changeEvent(event);
}

void KanaWidget::retranslate()
{
    previous_button->setText(tr("Previous"));
    next_button->setText(tr("Next"));
    pronounce_button->setText(tr("Pronounce"));
    // the button text depends on the active state of the spoiler
    if (romaji_visible)
        hide_romaji_button->setText(tr("Hide romaji"));
    else
        hide_romaji_button->setText(tr("Show romaji"));
}




void KanaWidget::showCurrentKana()
{
    /* retrieve the current kana in order to display it */
    Kana* selected_kana= getCurrentKana();
    /* update the japanese label */
    japanese_label->setText("<center>"+selected_kana->getJapanese()+"</center>");
    /* update the romaji label */
    if(romaji_visible)
        romaji_label->setText("<center>"+selected_kana->getRomaji()+"</center>");
    else
        romaji_label->setText("");

}

// enables/disables the spoiler on each call
void KanaWidget::toggleRomaji()
{
    romaji_visible=!romaji_visible;
    // depending on the active state of the spoiler we change the button text
    retranslate();
    showCurrentKana();
}

// loads the kana on the left
void KanaWidget::moveToLeft()
{
    kana_list->moveSelection(true);
    //update
    showCurrentKana();
}

// loads the kana on the right
void KanaWidget::moveToRight()
{
    kana_list->moveSelection(false);
    //update
    showCurrentKana();
}

/** returns the current hiragana kana if hiragana == true
 * otherwise returns the current katakana **/
Kana* KanaWidget::getCurrentKana()
{

    return kana_list->getSelected(hiragana);
}


// will eventually allow selecting a kanji by adding a choice
// index of the drop-down list: 0 hiragana, 1 katakana
void KanaWidget::setScript(int choice)
{
    hiragana=(choice==0);
    showCurrentKana();
}

// pronounces the current kana
void KanaWidget::pronounce()
{
    sound->setSource(QUrl("qrc:/sons/ressources/sons/"+getCurrentKana()->getRomaji()+".wav"));
    sound->play();
}
