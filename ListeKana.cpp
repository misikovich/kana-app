#include "ListeKana.h"

ListeKana::ListeKana()
{
    sel=0;

    /* initialisation des tableaux de chaines contenant les romajis et
     * les hiraganas correspondant
     */

    QString tableau_hiragana[]={"あ","い","う","え","お",
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

    QString tableau_romaji_hiragana[]={"a","i","u","e","o",
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

    // on enlève we et wi qui n'ont pas de fichiers audio : "ウィ" - wi , "ウェ" - we

    QString tableau_katakana[]={"ア","イ","ウ","エ","オ",
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

    /* initialisation de la liste à afficher */
    for(int i(0);i<HIRAGANA_SIZE;i++)
        liste_hiragana.push_back(new Kana(tableau_romaji_hiragana[i],tableau_hiragana[i]));

    for(int i(0);i<KATAKANA_SIZE;i++)
        liste_katakana.push_back(new Kana(tableau_romaji_hiragana[i],tableau_katakana[i]));
}


/* fonction retournant l'hiragana selectionnée ¨*/
Kana* ListeKana::getSelection()
{
    return liste_hiragana[sel];
}

/* fonction retournant l'hiragana correspondant
 * si le booléen vaut true sinon le katakana correspondant ¨*/
Kana* ListeKana::getSelection(bool hiragana)
{
    if (hiragana)
        return liste_hiragana[sel];
    else
        return liste_katakana[sel];
}

/*se déplace dans la liste, renvoie false si impossible d'aller plus loin
 *le bool en paramètre indique si on veut se déplacer à droite (false)
 * ou à gauche (true)
 */

bool ListeKana::deplaceSelection(bool gauche)
{

    int taille = liste_hiragana.size();
    bool deplacement_possible=( (0<sel && gauche) || (!gauche && sel<(taille-1)) );
    if (deplacement_possible)
        if (gauche)
            sel--;
        else
            sel++;

    return deplacement_possible;
}


Kana* ListeKana::getShuffledKana()
{
    int hasard = QRandomGenerator::global()->bounded(HIRAGANA_SIZE);
    bool katakana = QRandomGenerator::global()->bounded(2);
    if (!katakana)
        return liste_hiragana[hasard];
    else
        return liste_katakana[hasard];
}
