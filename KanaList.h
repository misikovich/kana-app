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
    //position of the selected kana, from 0 to count()-1
    int getSelectedIndex();
    int count();
    Kana* getSelected(bool hiragana);
    //random kana among the chosen scripts ( at least one must be true )
    Kana* getShuffledKana(bool hiragana=true, bool katakana=true);

private:
    QVector<Kana*> hiragana_list,katakana_list;

    int selected;
};

#endif // KANALIST_H
