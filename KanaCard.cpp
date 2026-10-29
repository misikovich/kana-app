#include "KanaCard.h"
#include <QStyle>

KanaCard::KanaCard(QWidget *parent) : QFrame(parent)
{
    kana=nullptr;
    remaining=0;
    setObjectName("card");
    setCursor(Qt::PointingHandCursor);

    kana_square = new KanaSquare;
    typed_label = new QLabel;
    QFont typed_font = font();
    typed_font.setPixelSize(26);
    typed_font.setWeight(QFont::DemiBold);
    typed_label->setFont(typed_font);
    typed_label->setAlignment(Qt::AlignCenter);
    typed_label->setMinimumHeight(QFontMetrics(typed_font).height());
    //seconds left, in the corner
    time_label = new QLabel;
    time_label->setProperty("role","muted");
    time_label->setAlignment(Qt::AlignRight);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(14,10,14,10);
    layout->addWidget(time_label);
    layout->addWidget(kana_square,1);
    layout->addWidget(typed_label);

    timer = new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(decrement()));
    setSelected(false);
}

void KanaCard::setKana(Kana* new_kana, int seconds)
{
    kana=new_kana;
    kana_square->setText(kana->getJapanese());
    typed_label->setText("");
    shown_since.start();
    remaining=seconds;
    time_label->setVisible(seconds>0);
    showRemaining();
    if (seconds>0)
        timer->start(1000);
    else
        timer->stop();
}

Kana* KanaCard::getKana()
{
    return kana;
}

int KanaCard::getRemaining()
{
    return remaining;
}

qint64 KanaCard::getElapsed()
{
    return shown_since.elapsed();
}

void KanaCard::setSelected(bool selected)
{
    //the style sheet draws the border of the selected card
    setProperty("selected",selected);
    style()->unpolish(this);
    style()->polish(this);
}

void KanaCard::setTyped(const QString &text)
{
    typed_label->setText(text);
}

void KanaCard::stop()
{
    timer->stop();
}

void KanaCard::mousePressEvent(QMouseEvent *event)
{
    emit clicked(this);
    QFrame::mousePressEvent(event);
}

void KanaCard::decrement()
{
    remaining--;
    showRemaining();
    //like the old countdown, the card runs out one tick after showing 0
    if (remaining<0)
    {
        timer->stop();
        emit timeUp(this);
    }
}

//the last 3 seconds are shown in red
void KanaCard::showRemaining()
{
    time_label->setText(tr("%1 s").arg(qMax(0,remaining)));
    time_label->setProperty("role",remaining<=3 ? "urgent" : "muted");
    time_label->style()->unpolish(time_label);
    time_label->style()->polish(time_label);
}
