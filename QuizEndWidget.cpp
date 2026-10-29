#include "QuizEndWidget.h"

QuizEndWidget::QuizEndWidget()
{
    score =0;
    end_label = new QLabel;

    end_label->setFont(QFont("Ubuntu",40));
    score_label = new QLabel;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(end_label,0,Qt::AlignCenter);
    layout->addWidget(score_label,0,Qt::AlignCenter);
    setLayout(layout);
    retranslate();

}

QuizEndWidget::QuizEndWidget(int s)
{
    score=s;
    end_label = new QLabel;
    score_label = new QLabel;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(end_label);
    layout->addWidget(score_label);
    setLayout(layout);
    retranslate();
}

void QuizEndWidget::setScore(int s)
{
    score=s;
    retranslate();
}

void QuizEndWidget::changeEvent(QEvent* event)
{
    if (event->type()==QEvent::LanguageChange)
        retranslate();
    QWidget::changeEvent(event);
}

void QuizEndWidget::retranslate()
{
    end_label->setText("<center>"+tr("THE END!")+"</center>");
    score_label->setText("<center>"+tr("Your score: %1").arg(score)+"</center>");
}
