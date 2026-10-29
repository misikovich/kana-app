#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QEvent>
class MainMenuWidget : public QWidget
{
    Q_OBJECT
public:
    MainMenuWidget();
    QPushButton* getButtonLearn();
    QPushButton* getButtonQuiz();
    void createIcons();
    void changeEvent(QEvent* event);
    void retranslate();

signals:

private:
    QPushButton* quiz_button;
    QPushButton* learn_button;
};

#endif // MAINMENUWIDGET_H
