#ifndef LANGUAGE_H
#define LANGUAGE_H
#include <QString>
#include <QStringList>

//available language codes ( "en" is the source language, without translation )
QStringList availableLanguages();

//name of the language in that language, for the menu ( e.g. "Русский" )
QString languageName(QString code);

//language chosen during a previous launch, otherwise the system one if it is available, otherwise "en"
QString initialLanguage();

//installs the translation of the language passed as parameter and remembers the choice
//the widgets then receive a QEvent::LanguageChange
void applyLanguage(QString code);

//currently applied language
QString currentLanguage();

#endif // LANGUAGE_H
