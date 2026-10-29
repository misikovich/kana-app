#ifndef QUIZENDWIDGET_H
#define QUIZENDWIDGET_H
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
class QuizEndWidget : public QWidget
{
    Q_OBJECT
public:
    QuizEndWidget();
    QuizEndWidget(int);
    void setScore(int);
    void changeEvent(QEvent* event);
    void retranslate();
private:
    //score to display
    int score;
    //score label
    QLabel *score_label;
    QLabel *end_label;
};

#endif // QUIZENDWIDGET_H
