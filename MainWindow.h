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
#include "QuizWidget.h"
#include <QPixmap>
#include "FaderWidget.h"
#include <QWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include "QuizEndWidget.h"
#include "ScoreStorage.h"
#include "Language.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    void createMenu();
    void createKanaToolBar();
    void createQuizToolBar();
    void keyPressEvent(QKeyEvent* event);
    //updates the texts when the language changes
    void changeEvent(QEvent* event);
    void retranslate();
public slots:
    void showAboutDialog();
    void showLearning();
    void showQuiz();
    void showMainMenu();
    //enables the animation:
    void fadeInWidget(int index);
    void showQuizEnd();
    void changeLanguage(QAction* action);




private:
    QToolBar *kanaToolBar;
    QToolBar *quizToolBar;

    KanaWidget *kana_widget;
    QuizWidget *quiz_widget;
    QuizEndWidget *quiz_end_widget;
    QComboBox *kana_combo_box;
    QStackedWidget *stacked_widgets;
    //texts to retranslate
    QMenu *about_menu;
    QAction *note_action;
    QMenu *language_menu;
    QLabel *kana_choice_label;
    QPushButton *kana_back_button;
    QPushButton *quiz_back_button;
    //enables the animation:
    QPointer<FaderWidget> faderWidget;

};

#endif // MAINWINDOW_H
