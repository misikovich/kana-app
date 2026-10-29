#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtWidgets>
#include "KanaWidget.h"
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QToolBar>
#include "MainMenuWidget.h"
#include "PracticeWidget.h"
#include <QPixmap>
#include "FaderWidget.h"
#include <QWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include "PracticeEndWidget.h"
#include "ScoreStorage.h"
#include "Language.h"
#include "Theme.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    void createMenu();
    void createKanaToolBar();
    void createPracticeToolBar();
    void keyPressEvent(QKeyEvent* event);
    //updates the texts when the language changes
    void changeEvent(QEvent* event);
    void retranslate();
public slots:
    void showAboutDialog();
    void showLearning();
    void showPractice();
    void showQuiz();
    void showMainMenu();
    //enables the animation:
    void fadeInWidget(int index);
    void showPracticeEnd();
    void changeLanguage(QAction* action);
    void changeTheme(QAction* action);
    void setScript(int script);




private:
    QToolBar *kanaToolBar;
    QToolBar *practiceToolBar;

    KanaWidget *kana_widget;
    PracticeWidget *practice_widget;
    //same widget in quiz mode ( 4 answers to choose from )
    PracticeWidget *quiz_widget;
    PracticeEndWidget *practice_end_widget;
    //index 0: hiragana, index 1: katakana
    QButtonGroup *script_group;
    QStackedWidget *stacked_widgets;
    //texts to retranslate
    QMenu *about_menu;
    QAction *note_action;
    QMenu *language_menu;
    QMenu *theme_menu;
    QActionGroup *theme_group;
    QPushButton *kana_back_button;
    QPushButton *practice_back_button;
    //enables the animation:
    QPointer<FaderWidget> faderWidget;

};

#endif // MAINWINDOW_H
