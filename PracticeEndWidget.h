#ifndef PRACTICEENDWIDGET_H
#define PRACTICEENDWIDGET_H
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
class PracticeEndWidget : public QWidget
{
    Q_OBJECT
public:
    PracticeEndWidget();
    PracticeEndWidget(int);
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

#endif // PRACTICEENDWIDGET_H
