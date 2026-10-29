#include "PracticeWidget.h"

//practice display: a settings page, then the game page
PracticeWidget::PracticeWidget()
{
    kana_list = new KanaList;
    selected_card=nullptr;
    score=0;
    mistakes=0;
    time_limit=10;
    level=1;
    streak=false;
    game_in_progress=false;

    pages = new QStackedWidget;
    pages->addWidget(createSettingsPage());
    pages->addWidget(createGamePage());
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(pages);
    QObject::connect(pages, SIGNAL(currentChanged(int)), this, SLOT(fadeInWidget(int)));

    retranslate();
}

QWidget* PracticeWidget::createSettingsPage()
{
    QSettings settings;
    QWidget *page = new QWidget;

    //kana choice: at least one of the two stays checked
    hiragana_check = new QCheckBox;
    katakana_check = new QCheckBox;
    hiragana_check->setChecked(settings.value("practice/hiragana",true).toBool());
    katakana_check->setChecked(settings.value("practice/katakana",true).toBool());
    if (!hiragana_check->isChecked() && !katakana_check->isChecked())
        hiragana_check->setChecked(true);
    QObject::connect(hiragana_check,SIGNAL(toggled(bool)),this,SLOT(keepOneKanaChecked()));
    QObject::connect(katakana_check,SIGNAL(toggled(bool)),this,SLOT(keepOneKanaChecked()));
    QHBoxLayout *kana_row = new QHBoxLayout;
    kana_row->addWidget(hiragana_check);
    kana_row->addWidget(katakana_check);
    kana_row->addStretch();

    //time per kana, as a row of toggle buttons
    time_group = new QButtonGroup(this);
    QHBoxLayout *time_row = new QHBoxLayout;
    int seconds[4]={10,20,30,0};
    for (int s : seconds)
    {
        QPushButton *button = new QPushButton;
        button->setCheckable(true);
        time_group->addButton(button,s);
        time_row->addWidget(button);
    }
    time_row->addStretch();
    QAbstractButton *saved_time = time_group->button(settings.value("practice/time",10).toInt());
    (saved_time ? saved_time : time_group->button(10))->setChecked(true);
    QObject::connect(time_group,SIGNAL(idClicked(int)),this,SLOT(updateBestScore()));

    //number of kana shown at the same time
    count_group = new QButtonGroup(this);
    QHBoxLayout *count_row = new QHBoxLayout;
    int counts[3]={1,2,4};
    for (int c : counts)
    {
        QPushButton *button = new QPushButton(QString::number(c));
        button->setCheckable(true);
        count_group->addButton(button,c);
        count_row->addWidget(button);
    }
    count_row->addStretch();
    QAbstractButton *saved_count = count_group->button(settings.value("practice/cards",1).toInt());
    (saved_count ? saved_count : count_group->button(1))->setChecked(true);

    kana_title = new QLabel;
    time_title = new QLabel;
    count_title = new QLabel;
    QFormLayout *form = new QFormLayout;
    form->addRow(kana_title,kana_row);
    form->addRow(time_title,time_row);
    form->addRow(count_title,count_row);

    best_label = new QLabel;
    start_button = new QPushButton;
    start_button->setDefault(true);
    QObject::connect(start_button,SIGNAL(clicked()),this,SLOT(startGame()));

    QHBoxLayout *bottom_row = new QHBoxLayout;
    bottom_row->addWidget(best_label);
    bottom_row->addStretch();
    bottom_row->addWidget(start_button);

    //the settings stay compact in the middle of the page
    QWidget *panel = new QWidget;
    panel->setMaximumWidth(520);
    //makes the chosen time and number of kana stand out
    panel->setStyleSheet("QPushButton { min-width: 3em; padding: 4px 10px; border: 1px solid #aaaaaa;"
                         " border-radius: 4px; background-color: #fafafa; }"
                         "QPushButton:hover { border-color: #22BBD6; }"
                         "QPushButton:checked { background-color: #22BBD6; color: white; border-color: #1a8fa5; }");
    QVBoxLayout *panel_layout = new QVBoxLayout(panel);
    panel_layout->addLayout(form);
    panel_layout->addSpacing(12);
    panel_layout->addLayout(bottom_row);

    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->addStretch();
    layout->addWidget(panel,0,Qt::AlignHCenter);
    layout->addStretch();
    return page;
}

QWidget* PracticeWidget::createGamePage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);

    score_label = new QLabel;
    layout->addWidget(score_label);

    cards_layout = new QGridLayout;
    layout->addLayout(cards_layout,1);

    //OK! or the right answer, fading out
    feedback_label = new QLabel;
    feedback_label->setFont(QFont("Ubuntu",20));
    feedback_label->setAlignment(Qt::AlignCenter);
    QGraphicsOpacityEffect *feedback_effect = new QGraphicsOpacityEffect(feedback_label);
    feedback_effect->setOpacity(0);
    feedback_label->setGraphicsEffect(feedback_effect);
    feedback_animation = new QPropertyAnimation(feedback_effect,"opacity",this);
    feedback_animation->setDuration(1500);
    feedback_animation->setStartValue(1.0);
    feedback_animation->setEndValue(0.0);
    feedback_animation->setEasingCurve(QEasingCurve::InQuad);
    layout->addWidget(feedback_label);

    answer_input = new QLineEdit;
    answer_input->setMaxLength(3);
    QObject::connect(answer_input,SIGNAL(textEdited(QString)),this,SLOT(handleAnswer(QString)));

    //only in unlimited time: fades in after 10 s on the same kana
    skip_button = new QPushButton;
    skip_effect = new QGraphicsOpacityEffect(skip_button);
    skip_button->setGraphicsEffect(skip_effect);
    skip_animation = new QPropertyAnimation(skip_effect,"opacity",this);
    skip_animation->setDuration(800);
    skip_animation->setStartValue(0.0);
    skip_animation->setEndValue(1.0);
    QObject::connect(skip_button,SIGNAL(clicked()),this,SLOT(skip()));
    QShortcut *skip_shortcut = new QShortcut(QKeySequence(Qt::Key_Escape),page);
    QObject::connect(skip_shortcut,SIGNAL(activated()),this,SLOT(skip()));
    skip_timer = new QTimer(this);
    skip_timer->setSingleShot(true);
    QObject::connect(skip_timer,SIGNAL(timeout()),this,SLOT(showSkipButton()));

    QHBoxLayout *input_row = new QHBoxLayout;
    input_row->addWidget(answer_input,1);
    input_row->addWidget(skip_button);
    layout->addLayout(input_row);
    return page;
}

void PracticeWidget::changeEvent(QEvent* event)
{
    if (event->type()==QEvent::LanguageChange)
        retranslate();
    QWidget::changeEvent(event);
}

void PracticeWidget::retranslate()
{
    kana_title->setText(tr("Kana:"));
    hiragana_check->setText("あ  "+tr("Hiragana"));
    katakana_check->setText("ア  "+tr("Katakana"));
    time_title->setText(tr("Time per kana:"));
    for (QAbstractButton *button : time_group->buttons())
    {
        int s = time_group->id(button);
        button->setText(s ? tr("%1 s").arg(s) : "∞");
        button->setToolTip(s ? tr("%1 seconds").arg(s) : tr("Unlimited time"));
    }
    count_title->setText(tr("Kana shown:"));
    start_button->setText(tr("Start"));
    answer_input->setPlaceholderText(cards.size()>1 ? tr("Click a kana, then type its romaji")
                                                    : tr("Type the romaji"));
    skip_button->setText(tr("Skip"));
    updateBestScore();
    updateScore();
}

//back to the settings page, with the saved best score
void PracticeWidget::reset()
{
    stopGame();
    pages->setCurrentIndex(0);
    updateBestScore();
}

void PracticeWidget::stopGame()
{
    game_in_progress=false;
    for (KanaCard *card : cards)
        card->stop();
    skip_timer->stop();
}

int PracticeWidget::getScore()
{
    return score;
}

bool PracticeWidget::isUnlimited()
{
    return time_limit==0;
}

void PracticeWidget::updateBestScore()
{
    bool unlimited = time_group->checkedId()==0;
    best_label->setText(tr("Best score: %1").arg(loadScore(scoreFilePath(unlimited))));
}

//the last checked kana box cannot be unchecked
void PracticeWidget::keepOneKanaChecked()
{
    if (!hiragana_check->isChecked() && !katakana_check->isChecked())
        qobject_cast<QCheckBox*>(sender())->setChecked(true);
}

void PracticeWidget::startGame()
{
    time_limit = time_group->checkedId();
    int count = count_group->checkedId();
    QSettings settings;
    settings.setValue("practice/hiragana",hiragana_check->isChecked());
    settings.setValue("practice/katakana",katakana_check->isChecked());
    settings.setValue("practice/time",time_limit);
    settings.setValue("practice/cards",count);

    //less time gives more points
    level = time_limit==10 ? 3 : time_limit==20 ? 2 : 1;
    score=0;
    mistakes=0;
    streak=false;

    //new cards for each game: their size depends on how many there are
    qDeleteAll(cards);
    cards.clear();
    selected_card=nullptr;
    int font_size = count==1 ? 150 : count==2 ? 110 : 80;
    int columns = count==4 ? 2 : count;
    for (int i=0;i<count;i++)
    {
        KanaCard *card = new KanaCard(font_size);
        cards_layout->addWidget(card,i/columns,i%columns);
        cards.append(card);
        QObject::connect(card,SIGNAL(clicked(KanaCard*)),this,SLOT(selectCard(KanaCard*)));
        QObject::connect(card,SIGNAL(timeUp(KanaCard*)),this,SLOT(onTimeUp(KanaCard*)));
    }
    for (KanaCard *card : cards)
        card->setKana(pickKana(),time_limit);

    game_in_progress=true;
    skip_button->setVisible(isUnlimited());
    retranslate();
    pages->setCurrentIndex(1);
    selectCard(cards.first());
}

//a random kana of the chosen scripts, not already shown on another card
Kana* PracticeWidget::pickKana()
{
    bool hiragana = hiragana_check->isChecked();
    bool katakana = katakana_check->isChecked();
    Kana *kana = nullptr;
    for (int tries=0;tries<20;tries++)
    {
        kana = kana_list->getShuffledKana(hiragana,katakana);
        bool shown=false;
        for (KanaCard *card : cards)
            if (card->getKana()==kana)
                shown=true;
        if (!shown)
            break;
    }
    return kana;
}

void PracticeWidget::selectCard(KanaCard *card)
{
    if (!game_in_progress)
        return;
    if (selected_card && selected_card!=card)
        selected_card->setTyped("");
    selected_card=card;
    for (KanaCard *c : cards)
        c->setSelected(c==card);
    answer_input->clear();
    answer_input->setFocus();
    restartSkipTimer();
}

//the skip button is hidden, then fades in once the selected kana has been shown for 10 s
void PracticeWidget::restartSkipTimer()
{
    skip_timer->stop();
    skip_animation->stop();
    skip_effect->setOpacity(0);
    skip_button->setEnabled(false);
    if (!isUnlimited() || !selected_card)
        return;
    skip_timer->start(qMax<qint64>(0,10000-selected_card->getElapsed()));
}

void PracticeWidget::showSkipButton()
{
    skip_button->setEnabled(true);
    skip_animation->start();
}

void PracticeWidget::skip()
{
    if (game_in_progress && isUnlimited() && skip_button->isEnabled())
        mistake(selected_card);
}

void PracticeWidget::onTimeUp(KanaCard *card)
{
    mistake(card);
}

void PracticeWidget::mistake(KanaCard *card)
{
    if (!game_in_progress)
        return;
    mistakes++;
    //the streak of correct answers is broken
    streak=false;
    //shows the right answer
    showFeedback("<font color='red'>"+card->getKana()->getJapanese()+" = "
                 +card->getKana()->getRomaji()+"</font>");

    // if the number of mistakes is >5 everything is stopped
    if (mistakes>5)
    {
        endGame();
        return;
    }
    replaceKana(card);
    updateScore();
}

void PracticeWidget::replaceKana(KanaCard *card)
{
    card->setKana(pickKana(),time_limit);
    if (card==selected_card)
    {
        answer_input->clear();
        restartSkipTimer();
    }
}

void PracticeWidget::endGame()
{
    stopGame();
    saveBestScore(scoreFilePath(isUnlimited()),score);
    emit gameOver();
}

void PracticeWidget::showFeedback(const QString &text)
{
    feedback_label->setText(text);
    feedback_animation->stop();
    feedback_animation->start();
}

void PracticeWidget::updateScore()
{
    //display of the score and of the mistake count
    QString score_text(tr("Score: %1").arg(score));
    if (mistakes!=0)
        score_text.append("   "+tr("Mistakes: %1").arg(mistakes));
    score_label->setText(score_text);
}

//checked on each keystroke: green while on track, red otherwise
void PracticeWidget::handleAnswer(QString answer)
{
    if (!game_in_progress || !selected_card)
        return;
    answer = answer.trimmed().toLower();

    //romaji have no digits: 1-4 select a card from the keyboard
    if (answer.size()==1 && answer[0].isDigit())
    {
        int index = answer.toInt()-1;
        answer_input->clear();
        if (index>=0 && index<cards.size())
            selectCard(cards[index]);
        return;
    }

    QString solution = selected_card->getKana()->getRomaji();
    if (answer==solution)
    {
        //the points of the question depend on the time left, doubled during a streak
        int gain = isUnlimited() ? 1 : selected_card->getRemaining()%10+level;
        if (streak)
            gain*=2;
        score+=gain;
        streak=true;
        showFeedback("<font color='green'>OK!</font>");
        replaceKana(selected_card);
        updateScore();
    }
    else if (solution.startsWith(answer))
        selected_card->setTyped("<font color='green'>"+answer+"</font>");
    else if (isUnlimited() && answer.size()>=solution.size())
        //without a timer, a complete wrong answer is the mistake
        mistake(selected_card);
    else
        selected_card->setTyped("<font color='red'>"+answer+"</font>");
}

//animation handling when the page changes
void PracticeWidget::fadeInWidget(int index)
{
    if (faderWidget)
        faderWidget->close();
    faderWidget = new FaderWidget(pages->widget(index));
    faderWidget->start();
}
