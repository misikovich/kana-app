#ifndef LISTEKANA_H
#define LISTEKANA_H
#include <QVector>
#include "Kana.h"
#define HIRAGANA_SIZE 46
#define KATAKANA_SIZE 46

#include <QRandomGenerator>
class ListeKana
{
public:
    ListeKana();
    Kana* getSelection();
    bool deplaceSelection(bool);
    Kana* getSelection(bool hiragana);
    Kana* getShuffledKana();

private:
    QVector<Kana*> liste_hiragana,liste_katakana;

    int sel;
};

#endif // LISTEKANA_H
