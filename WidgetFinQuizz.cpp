#include "WidgetFinQuizz.h"

WidgetFinQuizz::WidgetFinQuizz()
{
    score =0;
    fin = new QLabel;

    fin->setFont(QFont("Ubuntu",40));
    score_label = new QLabel;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fin,0,Qt::AlignCenter);
    layout->addWidget(score_label,0,Qt::AlignCenter);
    setLayout(layout);
    retraduit();

}

WidgetFinQuizz::WidgetFinQuizz(int s)
{
    score=s;
    fin = new QLabel;
    score_label = new QLabel;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(fin);
    layout->addWidget(score_label);
    setLayout(layout);
    retraduit();
}

void WidgetFinQuizz::setScore(int s)
{
    score=s;
    retraduit();
}

void WidgetFinQuizz::changeEvent(QEvent* event)
{
    if (event->type()==QEvent::LanguageChange)
        retraduit();
    QWidget::changeEvent(event);
}

void WidgetFinQuizz::retraduit()
{
    fin->setText("<center>"+tr("FIN !")+"</center>");
    score_label->setText("<center>"+tr("Votre score : %1").arg(score)+"</center>");
}
