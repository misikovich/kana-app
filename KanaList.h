#ifndef KANALIST_H
#define KANALIST_H
#include <QVector>
#include "Kana.h"
#define HIRAGANA_SIZE 46
#define KATAKANA_SIZE 46

#include <QRandomGenerator>
class KanaList
{
public:
    KanaList();
    Kana* getSelected();
    bool moveSelection(bool);
    Kana* getSelected(bool hiragana);
    Kana* getShuffledKana();

private:
    QVector<Kana*> hiragana_list,katakana_list;

    int selected;
};

#endif // KANALIST_H
