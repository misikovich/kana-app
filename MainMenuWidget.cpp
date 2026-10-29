#include "MainMenuWidget.h"
//selection menu
MainMenuWidget::MainMenuWidget()
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    learn_button  =    new QPushButton;
    practice_button = new QPushButton;
    quiz_button = new QPushButton;

    QLabel *welcome_label = new QLabel("- Kana Learning -");
    welcome_label->setFont(QFont("Ubuntu",40));
    welcome_label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    QLabel *author_label = new QLabel("Najimi's + Misikovich's + Vadim's + Claude's App");
    author_label->setFont(QFont("Ubuntu",16));
    author_label->setWordWrap(true);
    author_label->setAlignment(Qt::AlignLeft);

    main_layout->addWidget(author_label);
    main_layout->addWidget(welcome_label);

    practice_button->setStyleSheet(" QPushButton {"
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
    learn_button->setStyleSheet(" QPushButton {"
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

    quiz_button->setStyleSheet(" QPushButton {"
                                "     background-color: #2fae5b;"
                                "     border-style: outset;"
                                "     border-width: 2px;"
                                "     border-radius: 10px;"
                                "     border-color: beige;"
                                "     font: bold 14px;"
                                "     min-width: 10em;"
                                "     padding: 6px;"
                                " }"
                                " QPushButton:pressed {"
                                "     background-color: #23854a;"
                                "     border-style: inset;"
                                " }");

    practice_button->setMinimumSize(80,80);
    quiz_button->setMinimumSize(80,80);
    learn_button->setMinimumSize(80,80);

    main_layout->addWidget(learn_button);
    main_layout->addWidget(practice_button);
    main_layout->addWidget(quiz_button);

    retranslate();


}

QPushButton* MainMenuWidget::getButtonPractice()
{
    return practice_button;
}

QPushButton* MainMenuWidget::getButtonQuiz()
{
    return quiz_button;
}

QPushButton* MainMenuWidget::getButtonLearn()
{
    return learn_button;
}


void MainMenuWidget::changeEvent(QEvent* event)
{
    if (event->type()==QEvent::LanguageChange)
        retranslate();
    QWidget::changeEvent(event);
}

void MainMenuWidget::retranslate()
{
    learn_button->setText(tr("Learn"));
    practice_button->setText(tr("Practice"));
    quiz_button->setText(tr("Quiz"));
}

void MainMenuWidget::createIcons()
{

}
