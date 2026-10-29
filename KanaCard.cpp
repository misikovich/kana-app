#include "KanaCard.h"

KanaCard::KanaCard(int font_size, QWidget *parent) : QFrame(parent)
{
    kana=nullptr;
    remaining=0;
    setObjectName("card");
    setCursor(Qt::PointingHandCursor);

    kana_label = new QLabel;
    kana_label->setFont(QFont("Ubuntu",font_size));
    kana_label->setAlignment(Qt::AlignCenter);
    typed_label = new QLabel;
    typed_label->setFont(QFont("Ubuntu",qMax(16,font_size/4)));
    typed_label->setAlignment(Qt::AlignCenter);
    time_label = new QLabel;
    time_label->setFont(QFont("Ubuntu",14));
    time_label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(kana_label,1);
    layout->addWidget(typed_label);
    layout->addWidget(time_label);

    timer = new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(decrement()));
    setSelected(false);
}

void KanaCard::setKana(Kana* new_kana, int seconds)
{
    kana=new_kana;
    kana_label->setText(kana->getJapanese());
    typed_label->setText("");
    shown_since.start();
    remaining=seconds;
    time_label->setVisible(seconds>0);
    time_label->setText(QString::number(remaining));
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
    //the object name keeps the border off the child labels
    setStyleSheet(selected ? "QFrame#card { border: 3px solid #22BBD6; border-radius: 10px; }"
                           : "QFrame#card { border: 1px solid #999999; border-radius: 10px; }");
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
    time_label->setText(QString::number(qMax(0,remaining)));
    //like the old countdown, the card runs out one tick after showing 0
    if (remaining<0)
    {
        timer->stop();
        emit timeUp(this);
    }
}
