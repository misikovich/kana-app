#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QEvent>

class MainMenuWidget : public QWidget
{
    Q_OBJECT
public:
    MainMenuWidget();
    QPushButton* getButtonLearn();
    QPushButton* getButtonPractice();
    QPushButton* getButtonQuiz();
    void changeEvent(QEvent* event);
    void retranslate();
signals:
private:
    //a menu entry: a kana in its square, a title and one line of description
    QPushButton* createModeButton(const QString &kana, QLabel **title, QLabel **description);
    QPushButton* practice_button;
    QPushButton* quiz_button;
    QPushButton* learn_button;
    QLabel *learn_title, *learn_description;
    QLabel *practice_title, *practice_description;
    QLabel *quiz_title, *quiz_description;
};

#endif // MAINMENUWIDGET_H
