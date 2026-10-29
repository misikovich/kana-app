#include "WidgetPrincipal.h"
//menu de selection
WidgetPrincipal::WidgetPrincipal()
{
    QVBoxLayout *layout_base = new QVBoxLayout(this);
    button_learn  =    new QPushButton("Learn");
    button_quizz = new QPushButton("Quizz");

    QLabel *bienvenue = new QLabel("- Kana Learning -");
    bienvenue->setFont(QFont("Ubuntu",40));
    bienvenue->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    QLabel *nom_auteur = new QLabel("Najimi's + Misikovich's + Vadim's + Claude's App");
    nom_auteur->setFont(QFont("Ubuntu",16));
    nom_auteur->setWordWrap(true);
    nom_auteur->setAlignment(Qt::AlignLeft);

    layout_base->addWidget(nom_auteur);
    layout_base->addWidget(bienvenue);

    button_quizz->setStyleSheet(" QPushButton {"
                                "     background-color: #0c8fe2;"
                                "     border-style: outset;"
                                "     border-width: 2px;"
                                "     border-radius: 10px;"
                                "     border-color: beige;"
                                "     font: bold 14px;"
                                "     min-width: 10em;"
                                "     padding: 6px;"
                                " }"
                                " QPushButton:pressed {"
                                "     background-color: #2e5d7b;"
                                "     border-style: inset;"
                                " }");
    button_learn->setStyleSheet(" QPushButton {"
                                "     background-color: red;"
                                "     border-style: outset;"
                                "     border-width: 2px;"
                                "     border-radius: 10px;"
                                "     border-color: beige;"
                                "     font: bold 14px;"
                                "     min-width: 10em;"
                                "     padding: 6px;"
                                " }"
                                " QPushButton:pressed {"
                                "     background-color: rgb(224, 0, 0);"
                                "     border-style: inset;"
                                " }");

    button_quizz->setMinimumSize(80,80);
    button_learn->setMinimumSize(80,80);

    layout_base->addWidget(button_learn);
    layout_base->addWidget(button_quizz);



}

QPushButton* WidgetPrincipal::getButtonQuizz()
{
    return button_quizz;
}

QPushButton* WidgetPrincipal::getButtonLearn()
{
    return button_learn;
}


void WidgetPrincipal::createIcons()
{

}
