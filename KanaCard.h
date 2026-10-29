#ifndef KANACARD_H
#define KANACARD_H
#include <QFrame>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>
#include <QVBoxLayout>
#include <QMouseEvent>
#include "Kana.h"
#include "KanaSquare.h"

//one kana shown in the practice, with its own countdown
//the player clicks a card to select it, then types its romaji
class KanaCard : public QFrame
{
    Q_OBJECT
public:
    explicit KanaCard(QWidget *parent=nullptr);
    //shows a new kana, seconds=0 means unlimited time
    void setKana(Kana* kana, int seconds);
    Kana* getKana();
    int getRemaining();
    //milliseconds since the current kana was shown
    qint64 getElapsed();
    void setSelected(bool selected);
    //answer typed by the player ( rich text, colored )
    void setTyped(const QString &text);
    void stop();
signals:
    void clicked(KanaCard*);
    void timeUp(KanaCard*);
protected:
    void mousePressEvent(QMouseEvent *event);
private slots:
    void decrement();
private:
    void showRemaining();
private:
    Kana* kana;
    KanaSquare *kana_square;
    QLabel *typed_label;
    QLabel *time_label;
    QTimer *timer;
    QElapsedTimer shown_since;
    int remaining;
};

#endif // KANACARD_H
