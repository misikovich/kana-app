#ifndef COUNTDOWNWIDGET_H
#define COUNTDOWNWIDGET_H

#include <QTimer>
#include "FaderWidget.h"
#include <QStackedWidget>
#include <QLabel>
#include <QHBoxLayout>
class CountdownWidget : public QWidget
{
    Q_OBJECT
public:
    CountdownWidget();
    CountdownWidget(int start,int time);
    void stop();
    void start(int time=1000);
    void reset();
    void setStartValue(int);
    int getLastValue();
signals:
    void finished();
public slots:
    void decrement();


private:
    int m_start;
    QTimer *timer;
    int start_value;
    //QStackedWidget used for the animation
    QStackedWidget *stacked_widget;
    int last_value;

};

#endif // COUNTDOWNWIDGET_H
