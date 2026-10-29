#include "CountdownWidget.h"

//timer animation
CountdownWidget::CountdownWidget()
{
    last_value=0;

}


// start : first number of the countdown, time : delay between each decrement
CountdownWidget::CountdownWidget(int start,int time)
{
    m_start=start;
    start_value=start;
    last_value=0;

//creation of the labels used for the animation
    QLabel *label1 = new QLabel(QString::number(m_start));
    label1->setAlignment(Qt::AlignHCenter);

    //QLabel label2(QString::number(m_start));

    stacked_widget = new QStackedWidget;

    stacked_widget->addWidget(label1);

    QFont font = QFont("Ubuntu",20);
    label1->setFont(font);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(stacked_widget);



    // timer initialization
    timer=new QTimer();
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(decrement()));


}

void CountdownWidget::setStartValue(int value)
{
    m_start=value;
    reset();
}

void CountdownWidget::stop()
{
    timer->stop();
}

int CountdownWidget::getLastValue()
{
    return last_value;
}

void CountdownWidget::reset()
{
    QLabel *label = (QLabel*)(stacked_widget->widget(0));
    label->setText(QString::number(m_start));
    last_value=m_start;
}

void CountdownWidget::start(int time)
{
    timer->start(time);
}

void CountdownWidget::decrement()
{
    //get the current number from the label
    QLabel *label = (QLabel*)(stacked_widget->widget(0));
    int counter = label->text().toInt();
    counter--;
    last_value=counter;
    // update the label
    label->setText(QString::number(counter));
    //animation: switch the widget
    stacked_widget->setCurrentIndex(0);

    /* if the counter is negative ( so it has reached its end )
     * the countdown is stopped and reinitialized to the
     * initial value */

    if (counter<0)
    {
        stop();
        emit finished();
        reset();
    }

}



