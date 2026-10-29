#include "KanaWidget.h"
#include "Theme.h"
//widget to display the list of kanas
KanaWidget::KanaWidget()
{
    setMinimumWidth(300);
    hiragana=true;
    romaji_visible=true;
    kana_list = new KanaList();
    sound = new QSoundEffect(this);

    japanese_square = new KanaSquare;
    japanese_square->setMinimumSize(160,160);
    japanese_square->setMaximumHeight(360);

    romaji_label = new QLabel();
    QFont romaji_font = font();
    romaji_font.setPixelSize(44);
    romaji_font.setWeight(QFont::DemiBold);
    romaji_label->setFont(romaji_font);
    romaji_label->setAlignment(Qt::AlignCenter);
    //keeps its height when the romaji is hidden
    romaji_label->setMinimumHeight(QFontMetrics(romaji_font).height());

    position_label = new QLabel;
    position_label->setProperty("role","muted");
    position_label->setAlignment(Qt::AlignCenter);

    previous_button = new QPushButton;
    next_button = new QPushButton;
    hide_romaji_button = new QPushButton;
    pronounce_button = new QPushButton;
    pronounce_button->setProperty("role","primary");
    previous_button->setProperty("role","ghost");
    next_button->setProperty("role","ghost");
    //the arrow keys already move through the list
    for (QPushButton *button : {previous_button,next_button,hide_romaji_button,pronounce_button})
        button->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout* buttons_layout = new QHBoxLayout;
    buttons_layout->setSpacing(8);
    buttons_layout->addWidget(previous_button);
    buttons_layout->addStretch();
    buttons_layout->addWidget(pronounce_button);
    buttons_layout->addWidget(hide_romaji_button);
    buttons_layout->addStretch();
    buttons_layout->addWidget(next_button);

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(24,8,24,24);
    main_layout->addStretch(1);
    main_layout->addWidget(japanese_square,4);
    main_layout->addSpacing(12);
    main_layout->addWidget(romaji_label);
    main_layout->addWidget(position_label);
    main_layout->addStretch(1);
    main_layout->addLayout(buttons_layout);

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
    previous_button->setText("‹  "+tr("Previous"));
    next_button->setText(tr("Next")+"  ›");
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
    japanese_square->setText(selected_kana->getJapanese());
    /* the romaji keeps its place when hidden */
    romaji_label->setText(romaji_visible ? selected_kana->getRomaji() : "");
    int index = kana_list->getSelectedIndex();
    position_label->setText(QString("%1 / %2").arg(index+1).arg(kana_list->count()));
    previous_button->setEnabled(index>0);
    next_button->setEnabled(index<kana_list->count()-1);
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
