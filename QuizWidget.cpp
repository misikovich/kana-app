#include "QuizWidget.h"


//quiz display
QuizWidget::QuizWidget()
{
    kana_list = new KanaList;
    score=0;
    mistakes=0;
    level=1;
    streak=false;
    bonus=false;
    level_choice_widget=nullptr;
    level_buttons[0]=level_buttons[1]=level_buttons[2]=nullptr;
    best_score=0;
    game_in_progress=false;
    QFont japanese_font = QFont("Ubuntu",180);
    QFont romaji_font = QFont("Ubuntu",120);
    kana_label=new QLabel();
    kana_label->setPixmap(QPixmap(":/images/ressources/images/kana_quizz.png"));
    kana_label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    answer_preview = new QLabel();
    answer_preview->setFont(romaji_font);
    kana_label->setFont(japanese_font);
    answer_input = new QLineEdit;
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(kana_label);
    layout->addWidget(answer_input);

    answer_input->setMaxLength(3);
    answer_preview->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    QObject::connect(answer_input,SIGNAL(textEdited(QString)),answer_preview,SLOT(setText(QString)));
    QObject::connect(answer_input,SIGNAL(textEdited(QString)),this,SLOT(handleAnswer(QString)));

    score_label=new QLabel;
    //reading from a score file
    layout->addWidget(score_label);

    //fade-out animation of the answer
    stacked_widgets = new QStackedWidget;
    stacked_widgets->addWidget(answer_preview);

    // OK! label which becomes ":(" on a wrong answer
    ok_label = new QLabel("<font color='green'><center>OK!</center></font>");
    ok_label->setFont(romaji_font);
    stacked_widgets->addWidget(ok_label);

    layout->addWidget(stacked_widgets);
    QObject::connect(stacked_widgets, SIGNAL(currentChanged(int)), this, SLOT(fadeInWidget(int)));


    //countdown handling
    countdown = new CountdownWidget(10,1000);
    layout->addWidget(countdown);
    QObject::connect(countdown,SIGNAL(finished()),this,SLOT(onTimeUp()));

    //shown once the level is chosen
    countdown->setVisible(false);
    answer_input->setVisible(false);




    createStartButton();
    retranslate();

}

void QuizWidget::changeEvent(QEvent* event)
{
    if (event->type()==QEvent::LanguageChange)
        retranslate();
    QWidget::changeEvent(event);
}

void QuizWidget::retranslate()
{
    start_button->setText(tr("Start"));
    welcome_text->setText("<center>"+tr("Have fun learning kana!")+"</center>");
    //the label is not in a layout: its size follows the text length
    welcome_text->adjustSize();
    int seconds[3]={10,20,30};
    for (int i=0;i<3;i++)
        if (level_buttons[i])
            level_buttons[i]->setText(tr("%1 seconds").arg(seconds[i]));
    if (game_in_progress)
        updateScore();
    else
        score_label->setText(tr("Best score: %1").arg(best_score));
}
//widget initialization: display of the start button and reset of the timer
void QuizWidget::reset(int best)
{
    best_score=best;
    game_in_progress=false;
    retranslate();
    stacked_widgets->setCurrentWidget(start_button);
    countdown->stop();
    countdown->setVisible(false);
    kana_label->setPixmap(QPixmap(":/images/ressources/images/kana_quizz.png"));
    answer_input->setVisible(false);
    score=0;
    mistakes=0;
    streak=false;
    bonus=false;

}

//stops the current game ( back to the main menu )
void QuizWidget::stopGame()
{
    countdown->stop();
}


// slot triggered when clicking on the level 1,2,3 button
void QuizWidget::startCountdown(int start)
{
    game_in_progress=true;
    countdown->setStartValue(start);
    countdown->setVisible(true);
    answer_input->setVisible(true);
}

//shows the answer when correct or when the timer has run out
void QuizWidget::showAnswer()
{

}

int QuizWidget::getScore()
{
    return score;
}

// slot when the timer has reached its end, handles the mistake count and the label initialization
//this method is only triggered when the countdown reaches its end!
void QuizWidget::onTimeUp()
{


    //the mistake count is increased when the timer has reached its end
    mistakes++;
    //the streak of correct answers is broken
    streak=false;

    // if the number of mistakes is >5 everything is stopped
    if (mistakes>5)
    {
        countdown->stop();
        emit gameOver();
        return;
    }

    nextQuestion();
}

void QuizWidget::updateScore()
{
    //display of the score and of the mistake count
    QString score_text(tr("Score: %1").arg(score));

    if (mistakes!=0)
        score_text.append("   "+tr("Mistakes: %1").arg(mistakes));
    score_label->setText(score_text);
}





// shows the levels
void QuizWidget::showLevelChoice()
{
    //the level menu is created only once
    if (level_choice_widget)
    {
        stacked_widgets->setCurrentWidget(level_choice_widget);
        return;
    }

    QWidget *level_choice_page  = new QWidget;
    level_choice_widget = level_choice_page;
    QVBoxLayout *layout = new QVBoxLayout(level_choice_page);
    QPushButton *button1 = new QPushButton;
    QPushButton *button2 = new QPushButton;
    QPushButton *button3 = new QPushButton;
    level_buttons[0]=button1;
    level_buttons[1]=button2;
    level_buttons[2]=button3;
    retranslate();


    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);

    stacked_widgets->addWidget(level_choice_page);
    stacked_widgets->setCurrentWidget(level_choice_page);


    //look for a better way to code this:
    QObject::connect(button1,SIGNAL(clicked()),this,SLOT(level1()));
    QObject::connect(button2,SIGNAL(clicked()),this,SLOT(level2()));
    QObject::connect(button3,SIGNAL(clicked()),this,SLOT(level3()));

}

// look for a way to retrieve the signal source
void QuizWidget::level1()
{
    startCountdown(10);
    stacked_widgets->setCurrentWidget(answer_preview);
    level=3;

    nextQuestion();
}

void QuizWidget::level2()
{
    startCountdown(20);
    stacked_widgets->setCurrentWidget(answer_preview);
    level=2;

    nextQuestion();

}

void QuizWidget::level3()
{
    startCountdown(30);
    stacked_widgets->setCurrentWidget(answer_preview);
    level=1;

    nextQuestion();



}

//streak or not, using a boolean or just the counter!
int QuizWidget::computeScore(int score)
{
    int gain = ((countdown->getLastValue()%10))+(level);
    //the bonus doubles the points of the question, not the total score
    if (bonus)
    {
        gain*=2;
    }
    return score+gain;
}

// go to the next question ( the next kana ) + timer initialization
void QuizWidget::nextQuestion()
{
    //the score is updated on every new question
    updateScore();


    // reset of the message previously typed by the user
    answer_preview->setText("");

    // a kana is picked at random and displayed
    Kana* random_kana = kana_list->getShuffledKana();
    solution = random_kana->getRomaji();
    kana_label->setText("<center>"+random_kana->getJapanese()+"</center>");

    //the text field is cleared before writing in it again
    answer_input->setText("");

    // creation of a timer for each question, the question changes when it runs out

    countdown->reset();
    countdown->start();
}

//shows OK and handles the display animation + score increase
void QuizWidget::handleAnswer(QString answer)
{
    //animation for each letter
    stacked_widgets->setCurrentWidget(ok_label);
    stacked_widgets->setCurrentWidget(answer_preview);

    //show the complete kana when the timer runs out or the kana is correct

    if (checkAnswer(answer))
    {
       //handling of the streak boolean then score increase,
       //before nextQuestion() resets the countdown
       bonus=streak;
       score=computeScore(score);
       nextQuestion();
       //score update after the computation
       updateScore();

       //show OK
       stacked_widgets->setCurrentWidget(ok_label);
       streak=true;
    }

}



// checks the answer and colors the letters depending on whether they are correct
bool QuizWidget::checkAnswer(QString answer)
{
    //fix the bug when the solution has 3 letters

    if (solution.size()==3)
    {
        if (answer.size()==2 && answer[0]==solution[0] && answer[1]==solution[1] )
        {
            colorGreen();
        }
        else if (answer.size()==1 && answer[0]==solution[0] )
        {
            colorGreen();
        }
        else
        {
            colorRed();
        }

    }
    else if (solution.size()==2)
    {
        if (answer.size()==1 && answer[0]==solution[0])
        {
            colorGreen();
        }
        else
        {
            colorRed();
        }
     }
    else if (solution.size()==1)
        if (solution==answer)
            colorGreen();
        else colorRed();
    else if (solution==answer)
        colorGreen();

    return solution==answer;
}



//colors the answer in red
void QuizWidget::colorRed()
{
    answer_preview->setText("<font color='red'>"+answer_preview->text()+"</font>");
}
// colors the answer in green
void QuizWidget::colorGreen()
{

    answer_preview->setText("<font color='green'>"+answer_preview->text()+"</font>");
}


//animation handling when the current widget of the widget stack changes
void QuizWidget::fadeInWidget(int index)
{
    if (faderWidget)
        faderWidget->close();
    faderWidget = new FaderWidget(stacked_widgets->widget(index));
    faderWidget->start();
}
void QuizWidget::createMenu()
{
    QWidget *level1_page,*level2_page,*level3_page;
    level1_page = new QWidget;
    level2_page = new QWidget;
    level3_page = new QWidget;
    level_stack = new QStackedWidget;
    level_stack->addWidget(level1_page);
    level_stack->addWidget(level2_page);
    level_stack->addWidget(level3_page);

    QPushButton *button_1 = new QPushButton(tr("Level %1").arg(1));
    QPushButton *button_2 = new QPushButton(tr("Level %1").arg(2));
    QPushButton *button_3 = new QPushButton(tr("Level %1").arg(3));
    QVBoxLayout *layout = new QVBoxLayout(this);


    //button style, should be moved to a function
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


void QuizWidget::createStartButton()
{
    start_button = new QPushButton;
    QObject::connect(start_button,SIGNAL(clicked()),this,SLOT(showLevelChoice()));
    stacked_widgets->addWidget(start_button);


    welcome_text = new QLabel(start_button);

    start_button->setStyleSheet(" QPushButton {"
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
