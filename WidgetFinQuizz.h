#ifndef WIDGETFINQUIZZ_H
#define WIDGETFINQUIZZ_H
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
class WidgetFinQuizz : public QWidget
{
    Q_OBJECT
public:
    WidgetFinQuizz();
    WidgetFinQuizz(int);
    void setScore(int);
    void changeEvent(QEvent* event);
    void retraduit();
private:
    //score à afficher
    int score;
    //label score
    QLabel *score_label;
    QLabel *fin;
};

#endif // WIDGETFINQUIZZ_H
