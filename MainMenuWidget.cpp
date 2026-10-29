#include "MainMenuWidget.h"
#include "KanaSquare.h"
//selection menu
MainMenuWidget::MainMenuWidget()
{
    //"kana" written in two workbook squares
    QHBoxLayout *squares_row = new QHBoxLayout;
    squares_row->setSpacing(0);
    for (QString kana : {"か","な"})
    {
        KanaSquare *square = new KanaSquare;
        square->setText(kana);
        square->setFixedSize(76,76);
        squares_row->addWidget(square);
    }
    squares_row->addStretch();

    QLabel *welcome_label = new QLabel("Kana Learning");
    welcome_label->setProperty("role","title");

    QLabel *author_label = new QLabel("Najimi's + Misikovich's + Vadim's + Claude's App");
    author_label->setProperty("role","muted");
    author_label->setWordWrap(true);

    //the first kana of the first three rows: a, ka, sa
    learn_button = createModeButton("あ",&learn_title,&learn_description);
    practice_button = createModeButton("か",&practice_title,&practice_description);
    quiz_button = createModeButton("さ",&quiz_title,&quiz_description);

    //a narrow column in the middle of the window, left aligned
    QWidget *column = new QWidget;
    column->setFixedWidth(440);
    QVBoxLayout *column_layout = new QVBoxLayout(column);
    column_layout->setContentsMargins(0,0,0,0);
    column_layout->setSpacing(10);
    column_layout->addLayout(squares_row);
    column_layout->addSpacing(8);
    column_layout->addWidget(welcome_label);
    column_layout->addSpacing(18);
    column_layout->addWidget(learn_button);
    column_layout->addWidget(practice_button);
    column_layout->addWidget(quiz_button);
    column_layout->addSpacing(18);
    column_layout->addWidget(author_label);

    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(24,24,24,24);
    main_layout->addStretch(2);
    QHBoxLayout *center_row = new QHBoxLayout;
    center_row->addStretch(1);
    center_row->addWidget(column);
    center_row->addStretch(1);
    main_layout->addLayout(center_row);
    main_layout->addStretch(3);

    retranslate();
}

QPushButton* MainMenuWidget::createModeButton(const QString &kana, QLabel **title, QLabel **description)
{
    QPushButton *button = new QPushButton;
    button->setProperty("role","mode");
    button->setMinimumHeight(84);
    button->setCursor(Qt::PointingHandCursor);

    KanaSquare *square = new KanaSquare;
    square->setText(kana);
    square->setFixedSize(52,52);
    *title = new QLabel;
    (*title)->setProperty("role","heading");
    *description = new QLabel;
    (*description)->setProperty("role","muted");
    (*description)->setWordWrap(true);

    QVBoxLayout *texts = new QVBoxLayout;
    texts->setSpacing(2);
    texts->addStretch();
    texts->addWidget(*title);
    texts->addWidget(*description);
    texts->addStretch();

    QHBoxLayout *layout = new QHBoxLayout(button);
    layout->setContentsMargins(16,12,16,12);
    layout->setSpacing(16);
    layout->addWidget(square);
    layout->addLayout(texts,1);

    //the clicks go to the button
    for (QWidget *child : {static_cast<QWidget*>(square),static_cast<QWidget*>(*title),static_cast<QWidget*>(*description)})
        child->setAttribute(Qt::WA_TransparentForMouseEvents);
    return button;
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
    learn_title->setText(tr("Learn"));
    learn_description->setText(tr("Browse the kana and hear each one"));
    practice_title->setText(tr("Practice"));
    practice_description->setText(tr("Type the romaji of the kana shown"));
    quiz_title->setText(tr("Quiz"));
    quiz_description->setText(tr("Pick the right romaji out of four"));
}
