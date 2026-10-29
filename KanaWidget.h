#ifndef KANAWIDGET_H
#define KANAWIDGET_H
#include <QVector>
#include "Kana.h"
#include "KanaList.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QWidget>
#include <QtGui>
#include <QHBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QUrl>
#include <QSoundEffect>
#include "KanaSquare.h"

/* class allowing to display the list of kanas */
class KanaWidget : public QWidget
{
    Q_OBJECT
public:
    KanaWidget();
    void showCurrentKana();
    void changeEvent(QEvent* event);
    void retranslate();

public slots:
    void toggleRomaji();
    void moveToLeft();
    void moveToRight();
    void pronounce();
    void setScript(int);
signals:

private:
    bool romaji_visible;
    bool hiragana;
    KanaList* kana_list;
    KanaSquare* japanese_square;
    QLabel* romaji_label;
    //position in the list, like 12 / 46
    QLabel* position_label;
    Kana* getCurrentKana();
    //show_romaji button whose text changes when it is pressed
    QPushButton* hide_romaji_button;
    QPushButton* previous_button;
    QPushButton* next_button;
    QPushButton* pronounce_button;
    //playback of the kana sound
    QSoundEffect* sound;
};

#endif // KANAWIDGET_H
