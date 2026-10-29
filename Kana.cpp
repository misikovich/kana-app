#include "Kana.h"

Kana::Kana()
{
}

Kana::Kana(QString romaji, QString japanese) : m_romaji(romaji), m_japanese(japanese)
{
}

Kana::Kana(QString romaji, QString japanese, QString image) : m_romaji(romaji), m_japanese(japanese), m_image(image)
{
}

QString Kana::getImage()
{
    return m_image;
}

QString Kana::getRomaji()
{
    return m_romaji;
}

QString Kana::getJapanese()
{
    return m_japanese;
}
