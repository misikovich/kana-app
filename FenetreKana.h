#ifndef FENETREKANA_H
#define FENETREKANA_H
#include <QMainWindow>
#include <QtWidgets>
#include "WidgetKana.h"
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QToolBar>
#include "WidgetPrincipal.h"
#include "WidgetQuizz.h"
#include <QPixmap>
#include "FaderWidget.h"
#include <QWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include "WidgetFinQuizz.h"
#include "GestionScore.h"
class FenetreKana : public QMainWindow
{
    Q_OBJECT
public:
    FenetreKana();
    void creationMenu();
    void creationToolBarKana();
    void creationToolBarQuizz();
    void keyPressEvent(QKeyEvent* event);
public slots:
    void affichePopupNote();
    void lanceWidgetLearning();
    void lanceWidgetQuizz();
    void retourFenetrePrincipale();
    //permet l'animation :
    void fadeInWidget(int index);
    void lanceFinQuiz();




private:
    QToolBar *toolBarKana;
    QToolBar *toolBarQuizz;

    WidgetKana *widget_kana;
    WidgetQuizz *widget_quizz;
    WidgetFinQuizz *widget_fin_quizz;
    QComboBox *liste_deroulante_kana;
    QStackedWidget *stacked_widgets;
    //permet l'animation :
    QPointer<FaderWidget> faderWidget;

};

#endif // FENETREKANA_H
