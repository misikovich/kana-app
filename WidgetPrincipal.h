#ifndef WIDGETPRINCIPAL_H
#define WIDGETPRINCIPAL_H
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QEvent>
class WidgetPrincipal : public QWidget
{
    Q_OBJECT
public:
    WidgetPrincipal();
    QPushButton* getButtonLearn();
    QPushButton* getButtonQuizz();
    void createIcons();
    void changeEvent(QEvent* event);
    void retraduit();

signals:

private:
    QPushButton* button_quizz;
    QPushButton* button_learn;
};

#endif // WIDGETPRINCIPAL_H
