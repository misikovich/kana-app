#include "PracticeEndWidget.h"

PracticeEndWidget::PracticeEndWidget()
{
    score =0;
    end_label = new QLabel;

    end_label->setProperty("role","title");
    score_label = new QLabel;
    QFont score_font = font();
    score_font.setPixelSize(30);
    score_label->setFont(score_font);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch(2);
    layout->addWidget(end_label,0,Qt::AlignCenter);
    layout->addSpacing(8);
    layout->addWidget(score_label,0,Qt::AlignCenter);
    layout->addStretch(3);
    setLayout(layout);
    retranslate();

}

PracticeEndWidget::PracticeEndWidget(int s)
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

void PracticeEndWidget::setScore(int s)
{
    score=s;
    retranslate();
}

void PracticeEndWidget::changeEvent(QEvent* event)
{
    if (event->type()==QEvent::LanguageChange)
        retranslate();
    QWidget::changeEvent(event);
}

void PracticeEndWidget::retranslate()
{
    end_label->setText("<center>"+tr("THE END!")+"</center>");
    score_label->setText("<center>"+tr("Your score: %1").arg(score)+"</center>");
}
