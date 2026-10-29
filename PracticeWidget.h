#ifndef PRACTICEWIDGET_H
#define PRACTICEWIDGET_H
#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QPointer>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QSettings>
#include <QShortcut>
#include "KanaList.h"
#include "KanaCard.h"
#include "FaderWidget.h"
#include "ScoreStorage.h"


class PracticeWidget : public QWidget
{
    Q_OBJECT
public:
    //quiz=false: type the romaji of 1, 2 or 4 kana
    //quiz=true: one kana and 4 romaji to choose from
    explicit PracticeWidget(bool quiz=false);
    //shows the settings page with the saved best scores
    void reset();
    int getScore();
    //stops the current game ( back to the main menu )
    void stopGame();
    void changeEvent(QEvent* event);
    void retranslate();
public slots:
    void startGame();
    void handleAnswer(QString);
    void handleChoice(int);
    void selectCard(KanaCard*);
    void onTimeUp(KanaCard*);
    void skip();
    void showSkipButton();
    void updateBestScore();
    void keepOneKanaChecked();
    void fadeInWidget(int index);

signals:
    void gameOver(); // allows the display of a new page in MainWindow
private:
    QWidget* createSettingsPage();
    QWidget* createGamePage();
    bool isUnlimited();
    Kana* pickKana();
    //counts a mistake, then replaces the kana of the card
    void mistake(KanaCard*);
    void replaceKana(KanaCard*);
    //quiz: puts the right romaji and 3 wrong ones on the answer buttons
    void fillChoices();
    void restartSkipTimer();
    void showFeedback(const QString &text);
    void updateScore();
    void endGame();

    bool quiz_mode;
    //QSettings group of the mode: "practice/" or "quiz/"
    QString settings_prefix;
    KanaList* kana_list;
    QStackedWidget *pages;
    QPointer<FaderWidget> faderWidget;

    //settings page
    QLabel *kana_title;
    QCheckBox *hiragana_check;
    QCheckBox *katakana_check;
    QLabel *time_title;
    QButtonGroup *time_group; // button id = seconds, 0 = unlimited
    QLabel *count_title;
    QButtonGroup *count_group; // button id = number of cards
    QLabel *best_label;
    QPushButton *start_button;

    //game page
    QLabel *score_label;
    QGridLayout *cards_layout;
    QList<KanaCard*> cards;
    KanaCard *selected_card;
    QLineEdit *answer_input;
    //quiz answers ( button id = position 0-3 )
    QButtonGroup *choice_group;
    QString choices[4];
    QLabel *feedback_label;
    QPropertyAnimation *feedback_animation;
    QPushButton *skip_button;
    QGraphicsOpacityEffect *skip_effect;
    QPropertyAnimation *skip_animation;
    //fades the skip button in after 10 s in unlimited mode
    QTimer *skip_timer;

    int score;
    int mistakes;
    int time_limit; // seconds per kana, 0 = unlimited
    int level; // makes the score increase more easily
    // tells whether the questions were entered correctly in a row
    bool streak;
    bool game_in_progress;
};

#endif // PRACTICEWIDGET_H
