#include "WidgetFinQuizz.h"

WidgetFinQuizz::WidgetFinQuizz()
{
    score =0;
    QLabel *fin = new QLabel("<center>FIN!</center>");

    fin->setFont(QFont("Ubuntu",40));
     score_label = new QLabel("<center>Votre Score :"+QString::number(score)+"</center>");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fin,0,Qt::AlignCenter);
    layout->addWidget(score_label,0,Qt::AlignCenter);
    setLayout(layout);

}

WidgetFinQuizz::WidgetFinQuizz(int score)
{
    QLabel *fin = new QLabel("<center>FIN!</center>");
    score_label = new QLabel("<center>Votre Score :"+QString::number(score)+"</center>");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(fin);
    layout->addWidget(score_label);
    setLayout(layout);
}

void WidgetFinQuizz::setScore(int s)
{
    score=s;
    score_label->setText("<center>Votre Score :"+QString::number(score)+"</center>");
}


