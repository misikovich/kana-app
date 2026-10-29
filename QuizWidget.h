#ifndef QUIZWIDGET_H
#define QUIZWIDGET_H
#include <QWidget>
#include <QTime>
#include <QtGlobal>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "KanaList.h"
#include <QCoreApplication>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include "FaderWidget.h"
#include <QPointer>
#include <QStackedWidget>
#include <QTimer>
#include <QLCDNumber>
#include "CountdownWidget.h"


class QuizWidget : public QWidget
{
    Q_OBJECT
public:
    QuizWidget();
    void createMenu();
    void createStartButton();
    bool checkAnswer(QString);
    void colorGreen();
    void colorRed();
    void startCountdown(int start=10);
    void nextQuestion();
    int computeScore(int x);
    void showAnswer();
    void reset(int); // best score as parameter
    int getScore();
    void stopGame();
    void changeEvent(QEvent* event);
    void retranslate();
public slots:
    void handleAnswer(QString);
    void fadeInWidget(int index);
    void onTimeUp();
    void showLevelChoice();
    void level1();
    void level2();
    void level3();
    void updateScore();

signals:
    void gameOver(); // allows the display of a new page in MainWindow
private:
    KanaList* kana_list;
    QString answer;
    QString solution;
    QLineEdit *answer_input;
    int score;
    int level; // makes the score increase more easily
    QStackedWidget *level_stack;
    //level menu, created on the first click on "Start"
    QWidget *level_choice_widget;
    QLabel *kana_label;
    QLabel *answer_preview;
    QLabel *score_label;
    //used for the animation:
    QPointer<FaderWidget> faderWidget;
    //list of the OK and answer qlabel widgets
    QStackedWidget *stacked_widgets;
    //timer for each kana
    CountdownWidget *countdown;
    //maximum number of kanas that scroll by
    int max_kana;
    //index of the current kana
    int kana_index;
    // tells whether the questions were entered correctly in a row
    bool streak;
    // enables the bonuses
    bool bonus;
    //button used to start the quiz
    QPushButton *start_button;
    //mistake counter
    int mistakes;
    //label showing OK! or :( depending on the answer
    QLabel *ok_label;
    //text shown on the start button
    QLabel *welcome_text;
    //level menu buttons ( nullptr until the menu is created )
    QPushButton *level_buttons[3];
    //best score shown before the game starts
    int best_score;
    //true once the level is chosen: the label then shows the game score
    bool game_in_progress;
};

#endif // QUIZWIDGET_H
