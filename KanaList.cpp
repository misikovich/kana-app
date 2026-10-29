#include "KanaList.h"

KanaList::KanaList()
{
    selected=0;

    /* initialisation of the string tables containing the romajis and
     * the matching hiraganas
     */

    QString hiragana_table[]={"あ","い","う","え","お",
                                "か","き","く","け","こ",
                                "さ","し","す","せ","そ",
                                "た","ち","つ","て","と",
                                "な","に","ぬ","ね","の",
                                "は","ひ","ふ","へ","ほ",
                                "ま","み","む","め","も",
                                "や",   "ゆ",     "よ",
                                "ら","り","る","れ","ろ",
                                "わ",               "を",
                               "ん"};

    QString hiragana_romaji_table[]={"a","i","u","e","o",
                                       "ka","ki","ku","ke","ko",
                                       "sa","shi","su","se","so",
                                       "ta","chi","tsu","te","to",
                                       "na","ni","nu","ne","no",
                                       "ha","hi","fu","he","ho",
                                       "ma","mi","mu","me","mo",
                                       "ya",     "yu",     "yo",
                                       "ra","ri","ru","re","ro",
                                       "wa",               "wo",
                                       "n"};

    // we remove we and wi, which have no audio files: "ウィ" - wi , "ウェ" - we

    QString katakana_table[]={"ア","イ","ウ","エ","オ",
                               "カ","キ","ク","ケ","コ",
                               "サ","シ","ス","セ","ソ",
                               "タ","チ","ツ","テ","ト",
                               "ナ","ニ","ヌ","ネ","ノ",
                               "ハ","ヒ","フ","ヘ","ホ",
                               "マ","ミ","ム","メ","モ",
                               "ヤ",    "ユ",     "ヨ",
                               "ラ","リ","ル","レ","ロ",
                               "ワ"              ,"ヲ",
                               "ン"};

    /* initialisation of the list to display */
    for(int i(0);i<HIRAGANA_SIZE;i++)
        hiragana_list.push_back(new Kana(hiragana_romaji_table[i],hiragana_table[i]));

    for(int i(0);i<KATAKANA_SIZE;i++)
        katakana_list.push_back(new Kana(hiragana_romaji_table[i],katakana_table[i]));
}


/* function returning the selected hiragana */
Kana* KanaList::getSelected()
{
    return hiragana_list[selected];
}

/* function returning the matching hiragana
 * if the boolean is true, otherwise the matching katakana */
Kana* KanaList::getSelected(bool hiragana)
{
    if (hiragana)
        return hiragana_list[selected];
    else
        return katakana_list[selected];
}

/*moves through the list, returns false if it is impossible to go further
 *the boolean parameter tells whether we want to move right (false)
 * or left (true)
 */

bool KanaList::moveSelection(bool to_left)
{

    int size = hiragana_list.size();
    bool can_move=( (0<selected && to_left) || (!to_left && selected<(size-1)) );
    if (can_move)
        if (to_left)
            selected--;
        else
            selected++;

    return can_move;
}


Kana* KanaList::getShuffledKana()
{
    int random_index = QRandomGenerator::global()->bounded(HIRAGANA_SIZE);
    bool is_katakana = QRandomGenerator::global()->bounded(2);
    if (!is_katakana)
        return hiragana_list[random_index];
    else
        return katakana_list[random_index];
}
