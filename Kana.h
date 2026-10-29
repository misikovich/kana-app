#ifndef KANA_H
#define KANA_H

#include <QString>
class Kana
{
public:
    Kana();
    Kana(QString romaji,QString japanese);
    Kana(QString romaji,QString japanese, QString image);
    QString getImage();
    QString getRomaji();
    QString getJapanese();
private:
    QString m_romaji;
    QString m_japanese;
    QString m_image;
};

#endif // KANA_H
